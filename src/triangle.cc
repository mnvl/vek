#include "triangle.h"

#include <algorithm>
#include <cmath>

namespace rove
{

namespace
{

template<class T>
struct point2
{
	T x;
	T y;
};

template<class T>
aabb<3, T> triangle_bounds(triangle<3, T> const &tri)
{
	aabb<3, T> result(tri.A);
	result.extend(tri.B);
	result.extend(tri.C);
	return result;
}

template<class T>
bool segment_intersects_triangle(vec<3, T> const &a, vec<3, T> const &b, triangle<3, T> const &tri)
{
	T const epsilon = static_cast<T>(EPSILON);
	vec<3, T> const direction = b - a;
	if (direction.length_sq() <= epsilon) {
		return tri.contains(a);
	}

	typename triangle<3, T>::plane_t const plane = tri.get_plane();
	if (std::abs(plane.get_normal() & direction) <= epsilon) {
		return false;
	}

	typename triangle<3, T>::ray_t const ray(a, direction);
	T t;
	vec<3, T> point;
	return tri.trace(ray, t, point, T(0), T(1));
}

template<class T>
point2<T> project_to_2d(vec<3, T> const &v, unsigned drop_axis)
{
	if (drop_axis == 0) {
		return point2<T>{v.y, v.z};
	}
	if (drop_axis == 1) {
		return point2<T>{v.x, v.z};
	}
	return point2<T>{v.x, v.y};
}

template<class T>
T orient_2d(point2<T> const &a, point2<T> const &b, point2<T> const &c)
{
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template<class T>
bool on_segment_2d(point2<T> const &a, point2<T> const &b, point2<T> const &p)
{
	T const epsilon = static_cast<T>(EPSILON);
	if (std::abs(orient_2d(a, b, p)) > epsilon) {
		return false;
	}

	return
		p.x >= std::min(a.x, b.x) - epsilon &&
		p.x <= std::max(a.x, b.x) + epsilon &&
		p.y >= std::min(a.y, b.y) - epsilon &&
		p.y <= std::max(a.y, b.y) + epsilon;
}

template<class T>
bool segments_intersect_2d(point2<T> const &a, point2<T> const &b, point2<T> const &c, point2<T> const &d)
{
	T const epsilon = static_cast<T>(EPSILON);
	T const o1 = orient_2d(a, b, c);
	T const o2 = orient_2d(a, b, d);
	T const o3 = orient_2d(c, d, a);
	T const o4 = orient_2d(c, d, b);

	bool const opposite_1 = (o1 > epsilon && o2 < -epsilon) || (o1 < -epsilon && o2 > epsilon);
	bool const opposite_2 = (o3 > epsilon && o4 < -epsilon) || (o3 < -epsilon && o4 > epsilon);
	if (opposite_1 && opposite_2) {
		return true;
	}

	return
		(std::abs(o1) <= epsilon && on_segment_2d(a, b, c)) ||
		(std::abs(o2) <= epsilon && on_segment_2d(a, b, d)) ||
		(std::abs(o3) <= epsilon && on_segment_2d(c, d, a)) ||
		(std::abs(o4) <= epsilon && on_segment_2d(c, d, b));
}

template<class T>
bool point_in_triangle_2d(point2<T> const &p, point2<T> const &a, point2<T> const &b, point2<T> const &c)
{
	T const epsilon = static_cast<T>(EPSILON);
	T const o1 = orient_2d(a, b, p);
	T const o2 = orient_2d(b, c, p);
	T const o3 = orient_2d(c, a, p);

	bool const has_negative = (o1 < -epsilon) || (o2 < -epsilon) || (o3 < -epsilon);
	bool const has_positive = (o1 > epsilon) || (o2 > epsilon) || (o3 > epsilon);
	return !(has_negative && has_positive);
}

template<class T>
bool coplanar_intersects(triangle<3, T> const &left, triangle<3, T> const &right, vec<3, T> const &normal)
{
	unsigned drop_axis = 0;
	T const abs_x = std::abs(normal.x);
	T const abs_y = std::abs(normal.y);
	T const abs_z = std::abs(normal.z);
	if (abs_y >= abs_x && abs_y >= abs_z) {
		drop_axis = 1;
	} else if (abs_z >= abs_x && abs_z >= abs_y) {
		drop_axis = 2;
	}

	point2<T> const la = project_to_2d(left.A, drop_axis);
	point2<T> const lb = project_to_2d(left.B, drop_axis);
	point2<T> const lc = project_to_2d(left.C, drop_axis);
	point2<T> const ra = project_to_2d(right.A, drop_axis);
	point2<T> const rb = project_to_2d(right.B, drop_axis);
	point2<T> const rc = project_to_2d(right.C, drop_axis);

	if (
		point_in_triangle_2d(la, ra, rb, rc) ||
		point_in_triangle_2d(lb, ra, rb, rc) ||
		point_in_triangle_2d(lc, ra, rb, rc) ||
		point_in_triangle_2d(ra, la, lb, lc) ||
		point_in_triangle_2d(rb, la, lb, lc) ||
		point_in_triangle_2d(rc, la, lb, lc))
	{
		return true;
	}

	point2<T> const left_edges_start[3] = {la, lb, lc};
	point2<T> const left_edges_end[3] = {lb, lc, la};
	point2<T> const right_edges_start[3] = {ra, rb, rc};
	point2<T> const right_edges_end[3] = {rb, rc, ra};

	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < 3; ++j) {
			if (segments_intersect_2d(
				left_edges_start[i], left_edges_end[i],
				right_edges_start[j], right_edges_end[j]))
			{
				return true;
			}
		}
	}

	return false;
}

template<class T>
bool triangle_intersects_impl(triangle<3, T> const &left, triangle<3, T> const &right)
{
	T const epsilon = static_cast<T>(EPSILON);
	aabb<3, T> const left_bounds = triangle_bounds(left);
	aabb<3, T> const right_bounds = triangle_bounds(right);
	if (!left_bounds.test_intersection(right_bounds)) {
		return false;
	}

	vec<3, T> const left_normal = (left.B - left.A) ^ (left.C - left.A);
	vec<3, T> const right_normal = (right.B - right.A) ^ (right.C - right.A);
	if (left_normal.length_sq() <= epsilon || right_normal.length_sq() <= epsilon) {
		return left_bounds.test_intersection(right_bounds);
	}

	typename triangle<3, T>::plane_t const left_plane = left.get_plane();
	if (left_plane.contains(right.A) && left_plane.contains(right.B) && left_plane.contains(right.C)) {
		return coplanar_intersects(left, right, left_normal);
	}

	return
		segment_intersects_triangle(left.A, left.B, right) ||
		segment_intersects_triangle(left.B, left.C, right) ||
		segment_intersects_triangle(left.C, left.A, right) ||
		segment_intersects_triangle(right.A, right.B, left) ||
		segment_intersects_triangle(right.B, right.C, left) ||
		segment_intersects_triangle(right.C, right.A, left) ||
		left.contains(right.A) ||
		left.contains(right.B) ||
		left.contains(right.C) ||
		right.contains(left.A) ||
		right.contains(left.B) ||
		right.contains(left.C);
}

} // namespace

template<class T>
bool triangle<3, T>::test_intersection(const triangle_t &t) const
{
	return triangle_intersects_impl(*this, t);
}

template bool triangle<3, scalar>::test_intersection(triangle<3, scalar> const &t) const;
template bool triangle<3, double>::test_intersection(triangle<3, double> const &t) const;

}
