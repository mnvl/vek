
#include <algorithm>
#include <cmath>

#include "aabb.h"
#include "obb.h"
#include "capsule.h"
#include "triangle.h"

namespace rove
{

namespace
{

template<class T>
T clamp01(T value)
{
	if (value < T(0)) return T(0);
	if (value > T(1)) return T(1);
	return value;
}

template<class T>
T point_triangle_distance_sq(vec<3, T> const &point, triangle<3, T> const &tri)
{
	vec<3, T> const ab = tri.B - tri.A;
	vec<3, T> const ac = tri.C - tri.A;
	vec<3, T> const ap = point - tri.A;

	T const d1 = ab & ap;
	T const d2 = ac & ap;
	if (d1 <= T(0) && d2 <= T(0)) {
		return (point - tri.A).length_sq();
	}

	vec<3, T> const bp = point - tri.B;
	T const d3 = ab & bp;
	T const d4 = ac & bp;
	if (d3 >= T(0) && d4 <= d3) {
		return (point - tri.B).length_sq();
	}

	T const vc = d1 * d4 - d3 * d2;
	if (vc <= T(0) && d1 >= T(0) && d3 <= T(0)) {
		T const v = d1 / (d1 - d3);
		vec<3, T> const projection = tri.A + ab * v;
		return (point - projection).length_sq();
	}

	vec<3, T> const cp = point - tri.C;
	T const d5 = ab & cp;
	T const d6 = ac & cp;
	if (d6 >= T(0) && d5 <= d6) {
		return (point - tri.C).length_sq();
	}

	T const vb = d5 * d2 - d1 * d6;
	if (vb <= T(0) && d2 >= T(0) && d6 <= T(0)) {
		T const w = d2 / (d2 - d6);
		vec<3, T> const projection = tri.A + ac * w;
		return (point - projection).length_sq();
	}

	T const va = d3 * d6 - d5 * d4;
	if (va <= T(0) && (d4 - d3) >= T(0) && (d5 - d6) >= T(0)) {
		vec<3, T> const bc = tri.C - tri.B;
		T const w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		vec<3, T> const projection = tri.B + bc * w;
		return (point - projection).length_sq();
	}

	vec<3, T> const normal = ab ^ ac;
	T const normal_length_sq = normal.length_sq();
	if (normal_length_sq <= static_cast<T>(EPSILON)) {
		T dA = (point - tri.A).length_sq();
		T dB = (point - tri.B).length_sq();
		T dC = (point - tri.C).length_sq();
		return std::min(dA, std::min(dB, dC));
	}

	T const distance_to_plane = (normal & ap);
	return (distance_to_plane * distance_to_plane) / normal_length_sq;
}

template<class T>
T segment_segment_distance_sq(line<3, T> const &lhs, line<3, T> const &rhs)
{
	vec<3, T> const d1 = lhs.B - lhs.A;
	vec<3, T> const d2 = rhs.B - rhs.A;
	vec<3, T> const r = lhs.A - rhs.A;

	T const a = d1 & d1;
	T const e = d2 & d2;
	T const f = d2 & r;
	T const epsilon = static_cast<T>(EPSILON);

	T s = T(0);
	T t = T(0);

	if (a <= epsilon && e <= epsilon) {
		return (lhs.A - rhs.A).length_sq();
	}

	if (a <= epsilon) {
		t = clamp01(f / e);
	} else {
		T const c = d1 & r;
		if (e <= epsilon) {
			s = clamp01(-c / a);
		} else {
			T const b = d1 & d2;
			T const denom = a * e - b * b;
			if (std::abs(denom) > epsilon) {
				s = clamp01((b * f - c * e) / denom);
			}
			T const t_nom = b * s + f;
			if (t_nom < T(0)) {
				t = T(0);
				s = clamp01(-c / a);
			} else if (t_nom > e) {
				t = T(1);
				s = clamp01((b - c) / a);
			} else {
				t = t_nom / e;
			}
		}
	}

	vec<3, T> const closest_lhs = lhs.A + d1 * s;
	vec<3, T> const closest_rhs = rhs.A + d2 * t;
	return (closest_lhs - closest_rhs).length_sq();
}

template<class T>
bool segment_intersects_triangle(line<3, T> const &segment, triangle<3, T> const &tri)
{
	vec<3, T> const direction = segment.B - segment.A;
	if (direction.length_sq() <= static_cast<T>(EPSILON)) {
		return tri.contains(segment.A);
	}

	ray<3, T> const ray(segment.A, direction);
	T t;
	vec<3, T> point;
	return tri.trace(ray, t, point, T(0), T(1));
}

template<class T>
bool capsule_triangle_intersects(capsule<3, T> const &capsule_shape, triangle<3, T> const &tri)
{
	if (capsule_shape.radius < T(0)) {
		return false;
	}

	if (segment_intersects_triangle(capsule_shape.axe, tri)) {
		return true;
	}

	T min_distance_sq = point_triangle_distance_sq(capsule_shape.axe.A, tri);
	min_distance_sq = std::min(min_distance_sq, point_triangle_distance_sq(capsule_shape.axe.B, tri));

	line<3, T> const edges[3] = {
		line<3, T>(tri.A, tri.B),
		line<3, T>(tri.B, tri.C),
		line<3, T>(tri.C, tri.A),
	};

	for (size_t i = 0; i < 3; ++i) {
		min_distance_sq = std::min(min_distance_sq, segment_segment_distance_sq(capsule_shape.axe, edges[i]));
	}

	vec<3, T> const axis = capsule_shape.axe.B - capsule_shape.axe.A;
	vec<3, T> const normal = (tri.B - tri.A) ^ (tri.C - tri.A);
	T const normal_length_sq = normal.length_sq();
	if (axis.length_sq() > static_cast<T>(EPSILON) && normal_length_sq > static_cast<T>(EPSILON)) {
		T const dist_a = normal & (capsule_shape.axe.A - tri.A);
		T const dist_b = normal & (capsule_shape.axe.B - tri.A);
		T t = T(0);
		if (std::abs(dist_a - dist_b) > static_cast<T>(EPSILON)) {
			t = clamp01(dist_a / (dist_a - dist_b));
		} else {
			t = T(0.5);
		}

		vec<3, T> const axis_point = capsule_shape.axe.A + axis * t;
		min_distance_sq = std::min(min_distance_sq, point_triangle_distance_sq(axis_point, tri));
	}

	return min_distance_sq <= capsule_shape.radius * capsule_shape.radius;
}

} // namespace

template<int N, class T> void
capsule<N, T>::get_aabb(aabb<ARITY, scalar_t> &bounds) const
{
	vec_t diagonal(radius, radius, radius);

	bounds.lo = bounds.hi = axe.A;
	bounds.extend(axe.B);

	bounds.extend(axe.A - diagonal);
	bounds.extend(axe.A + diagonal);

	bounds.extend(axe.B - diagonal);
	bounds.extend(axe.B + diagonal);
}

template<int N, class T>
bool capsule<N, T>::test_intersection(triangle<ARITY, scalar_t> const &triangle_shape) const
{
	static_assert(N == 3, "capsule::test_intersection(triangle) is only implemented for 3D capsules");
	return capsule_triangle_intersects(*this, triangle_shape);
}

template class capsule<3>;
template bool capsule<3, double>::test_intersection(triangle<3, double> const &triangle_shape) const;

}
