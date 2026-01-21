/**
 * @file obb.h
 * @brief Oriented bounding box (OBB) classes for 2D and 3D
 *
 * Provides oriented bounding boxes that can be rotated arbitrarily in space.
 * OBBs offer tighter bounds than AABBs for rotated objects at the cost of
 * more expensive intersection tests.
 */

#pragma once

#include <assert.h>
#include "scalar.h"
#include "vec.h"
#include "aabb.h"
#include "ray.h"
#include "line.h"
#include "triangle.h"
#include "matrix.h"

namespace rove
{

template<class T> class plane;
template<int N,class T> class triangle;

template<int N,class T=scalar> class obb;

/**
 * @brief 2D Oriented bounding box
 *
 * A 2D box defined by an origin point and two edge vectors (tangent and normal).
 * The box vertices are:
 * - origin
 * - origin + tangent
 * - origin + tangent + normal
 * - origin + normal
 *
 * @tparam T Scalar type (typically float or double)
 */
template<class T>
class obb<2, T> {
public:
	typedef T scalar_t;                         ///< Scalar component type
	static size_t const ARITY = 2;              ///< Number of dimensions
	typedef vec<ARITY,scalar_t> vec_t;          ///< Associated vector type
	typedef matrix<ARITY+1,ARITY+1,scalar_t> matrix_t;  ///< Transformation matrix type
	typedef aabb<ARITY,scalar_t> aabb_t;        ///< Associated AABB type
	static size_t const SIDES_COUNT = 4;        ///< Number of sides
	static size_t const VERTICES_COUNT = 4;     ///< Number of vertices

	vec_t origin;   ///< Origin corner of the OBB
	vec_t tangent;  ///< First edge vector (width direction)
	vec_t normal;   ///< Second edge vector (height direction)

	/// Default constructor (uninitialized)
	obb() {
	}

	/**
	 * @brief Construct OBB from axis-aligned bounding box
	 *
	 * Creates an axis-aligned OBB that matches the given AABB.
	 *
	 * @param rhs AABB to convert
	 */
	obb(aabb_t const &rhs) {
		origin = rhs.lo;
		tangent.set(rhs.hi.x - rhs.lo.x, 0);
		normal.set(0, rhs.hi.y - rhs.lo.y);
	}

	/**
	 * @brief Transform vector from world to local OBB coordinates
	 *
	 * Converts a direction vector to local coordinates where tangent and
	 * normal are unit basis vectors.
	 *
	 * @param[out] v Output vector in local coordinates
	 * @param[in] v0 Input vector in world coordinates
	 */
	void world_to_local_vector(vec_t &v, vec_t const &v0) const {
		v.x = (v0 & tangent) / tangent.length_sq();
		v.y = (v0 & normal)	 / normal.length_sq();
	}

	/**
	 * @brief Transform vector from world to local OBB coordinates
	 *
	 * @param v0 Input vector in world coordinates
	 * @return Vector in local coordinates
	 */
	vec_t world_to_local_vector(vec_t const &v0) const {
		vec_t v;
		world_to_local_vector(v, v0);
		return v;
	}

	/**
	 * @brief Transform point from world to local OBB coordinates
	 *
	 * Converts a point to local coordinates where origin maps to (0,0)
	 * and opposite corner maps to (1,1).
	 *
	 * @param[out] v Output point in local coordinates
	 * @param[in] v0 Input point in world coordinates
	 */
	void world_to_local_point(vec_t &v,vec_t const &v0) const {
		world_to_local_vector(v, v0 - origin);
	}

	/**
	 * @brief Transform point from world to local OBB coordinates
	 *
	 * @param v0 Input point in world coordinates
	 * @return Point in local coordinates
	 */
	vec_t world_to_local_point(vec_t const &v0) const {
		vec_t v;
		world_to_local_point(v, v0);
		return v;
	}

	/**
	 * @brief Test if point is inside OBB
	 *
	 * @param v Point to test
	 * @return true if point is inside or on the boundary
	 */
	bool contains(vec_t const &v) const {
		vec_t v1;
		world_to_local_point(v1, v);
		return v1.x>=0 && v1.x<=1
		    && v1.y>=0 && v1.y<=1;
	}

	/**
	 * @brief Transform OBB by a matrix
	 *
	 * Applies a transformation matrix to the OBB.
	 *
	 * @param tf Transformation matrix
	 */
	void transform(matrix_t const &tf) {
		vec_t O = origin, Ox = tangent + origin, Oy = normal + origin;
		mul(origin, O, tf);
		mul(tangent, Ox, tf);
		mul(normal, Oy, tf);
		tangent -= origin;
		normal -= origin;
	}

	/**
	 * @brief Get vertex position by index
	 *
	 * @param vertex_number Vertex index (0-3)
	 * @return Position of the vertex
	 */
	vec_t get_vertex(size_t vertex_number) const {
		switch (vertex_number) {
			case 0: return origin;
			case 1: return origin + tangent;
			case 2: return origin + tangent + normal;
			case 3: return origin + normal;
			default: assert(0); return vec_t(0, 0);
		}
	}

	/**
	 * @brief Get side as line segment
	 *
	 * @param side_number Side index (0-3)
	 * @return Line segment representing the side
	 */
	line<ARITY, scalar_t> get_side(size_t side_number) const {
		line<ARITY, scalar_t> side;
		side.A = get_vertex(side_number);
		side.B = get_vertex((1 + side_number) % SIDES_COUNT);
		return side;
	}

	/**
	 * @brief Get outward normal for a side
	 *
	 * @param side_number Side index (0-3)
	 * @return Normalized outward-facing normal vector
	 */
	vec_t get_normal(size_t side_number) const {
		switch (side_number) {
			case 0: return rove::normalize(-normal);
			case 1: return rove::normalize(tangent);
			case 2: return rove::normalize(normal);
			case 3: return rove::normalize(-tangent);
			default: assert(0); return vec_t(0, 0);
		}
	}

	/**
	 * @brief Get axis-aligned bounding box containing this OBB
	 *
	 * @param[out] b Output AABB that bounds this OBB
	 */
	void get_aabb(aabb_t &b) const {
		b.lo = b.hi = origin;
		b.extend(origin + tangent);
		b.extend(origin + normal);
		b.extend(origin + tangent + normal);
	}
};

/**
 * @brief 3D Oriented bounding box
 *
 * A 3D box defined by an origin point and three edge vectors (tangent, normal, binormal).
 * The eight vertices are all combinations of origin + {0|tangent} + {0|normal} + {0|binormal}.
 *
 * @tparam T Scalar type (typically float or double)
 */
template<class T>
class obb<3,T> {
public:
	typedef T scalar_t;                         ///< Scalar component type
	static size_t const ARITY = 3;              ///< Number of dimensions
	typedef vec<ARITY,scalar_t> vec_t;          ///< Associated vector type
	typedef matrix<ARITY, ARITY, scalar_t> matrix3_t;    ///< 3x3 matrix type
	typedef matrix<ARITY+1, ARITY+1, scalar_t> matrix_t; ///< 4x4 transformation matrix type
	typedef aabb<ARITY,scalar_t> aabb_t;        ///< Associated AABB type
	static size_t const SIDES_COUNT = 6;        ///< Number of faces
	static size_t const VERTICES_COUNT = 8;     ///< Number of vertices
	static size_t const SEGMENTS_COUNT = 4;     ///< Number of octants for subdivision

	vec_t origin;    ///< Origin corner of the OBB
	vec_t tangent;   ///< First edge vector (X-like direction)
	vec_t normal;    ///< Second edge vector (Y-like direction)
	vec_t binormal;  ///< Third edge vector (Z-like direction)

	/// Default constructor (uninitialized)
	obb() {
	}

	/**
	 * @brief Construct OBB from axis-aligned bounding box
	 *
	 * Creates an axis-aligned OBB that matches the given AABB.
	 *
	 * @param a AABB to convert
	 */
	obb(const aabb_t &a)
	{
		operator =(a);
	}

	/**
	 * @brief Construct OBB from origin and three edge vectors
	 *
	 * @param o Origin point
	 * @param i Tangent (first edge vector)
	 * @param j Normal (second edge vector)
	 * @param k Binormal (third edge vector)
	 */
	obb(vec_t const &o, vec_t const &i, vec_t const &j, vec_t const &k):
		origin(o),
		tangent(i),
		normal(j),
		binormal(k)
	{
	}

	/**
	 * @brief Assignment from AABB
	 *
	 * @param a AABB to convert
	 * @return Reference to this OBB
	 */
	obb &operator =(const aabb_t &a) {
		origin = a.lo;
		tangent.set(a.hi.x - a.lo.x, 0, 0);
		normal.set(0, a.hi.y - a.lo.y, 0);
		binormal.set(0, 0, a.hi.z - a.lo.z);
		return *this;
	}

	/**
	 * @brief Get center point of the OBB
	 *
	 * @return Center point
	 */
	vec_t get_centre() const {
		return origin + (tangent + normal + binormal) / scalar_t(2);
	}

	/**
	 * @brief Get bounding sphere radius
	 *
	 * @return Distance from center to furthest corner
	 */
	scalar_t get_radius() const;

	/**
	 * @brief Get volume of the OBB
	 *
	 * @return Volume (product of edge lengths)
	 */
	scalar_t get_volume() const {
		return tangent.length() * normal.length() * binormal.length();
	}

	/**
	 * @brief Get axis-aligned bounding box containing this OBB
	 *
	 * @param[out] b Output AABB that bounds this OBB
	 */
	void get_aabb(aabb_t &b) const {
		b.lo = b.hi = origin;
		b.extend(origin + tangent);
		b.extend(origin + normal);
		b.extend(origin + binormal);
		b.extend(origin + tangent + normal);
		b.extend(origin + tangent + binormal);
		b.extend(origin + normal + binormal);
		b.extend(origin + tangent + normal + binormal);
	}

	/**
	 * @brief Transform OBB by a 4x4 matrix
	 *
	 * Applies a transformation matrix to the OBB.
	 *
	 * @param tf Transformation matrix
	 */
	void transform(const matrix_t &tf) {
		// get vertex point coordinates
		vec_t O = origin,
			 Ox = tangent + origin,
			 Oy = normal + origin,
			 Oz = binormal + origin;

		// transform these coordinates
		mul(origin, O, tf);
		mul(tangent, Ox, tf);
		mul(normal, Oy, tf);
		mul(binormal, Oz, tf);

		// compute tangent, normal, binormal values relative to origin
		tangent -= origin;
		normal -= origin;
		binormal -= origin;
	}

	/**
	 * @brief Get vertex position by index
	 *
	 * Vertices are numbered 0-7 using binary encoding:
	 * bit 0 = include tangent, bit 1 = include normal, bit 2 = include binormal
	 *
	 * @param vertex_number Vertex index (0-7)
	 * @param[out] position Output vertex position
	 */
	void get_vertex(size_t vertex_number, vec_t &position) const {
		position = origin +	scalar_t(vertex_number & 1) * tangent;
		if (vertex_number & 2) position += normal;
		if (vertex_number & 4) position += binormal;
	}

	/**
	 * @brief Get an octant (sub-box) of this OBB
	 *
	 * Subdivides the OBB into 8 equal sub-boxes (octants).
	 *
	 * @param segment_number Octant index (0-7)
	 * @param[out] segment_obb Output sub-OBB
	 */
	void get_segment(size_t segment_number, obb &segment_obb) {
		segment_obb.tangent = tangent / scalar_t(2);
		segment_obb.normal = normal / scalar_t(2);
		segment_obb.binormal = binormal	/ scalar_t(2);
		segment_obb.origin = origin;

		if(segment_number & 1) segment_obb.origin += segment_obb.tangent;
		if(segment_number & 2) segment_obb.origin += segment_obb.normal;
		if(segment_number & 4) segment_obb.origin += segment_obb.binormal;
	}

	/**
	 * @brief Transform vector from world to local OBB coordinates
	 *
	 * Converts a direction vector to local coordinates where tangent,
	 * normal, and binormal form the basis.
	 *
	 * @param[out] v Output vector in local coordinates
	 * @param[in] v0 Input vector in world coordinates
	 */
	void world_to_local_vector(vec_t &v, vec_t const &v0) const;

	/**
	 * @brief Transform vector from local to world OBB coordinates
	 *
	 * @param[out] v Output vector in world coordinates
	 * @param[in] v0 Input vector in local coordinates
	 */
	void local_to_world_vector(vec_t &v, vec_t const &v0) const {
		v = v0.x * tangent + v0.y * normal + v0.z * binormal;
	}

	/**
	 * @brief Transform point from world to local OBB coordinates
	 *
	 * @param[out] v Output point in local coordinates [0,1]^3 for interior
	 * @param[in] v0 Input point in world coordinates
	 */
	void world_to_local_point(vec_t &v,vec_t const &v0) const {
		world_to_local_vector(v, v0 - origin);
	}

	/**
	 * @brief Transform ray from world to local OBB coordinates
	 *
	 * @param[out] r Output ray in local coordinates
	 * @param[in] r0 Input ray in world coordinates
	 */
	void world_to_local_ray(ray<ARITY, scalar_t> &r,const ray<ARITY, scalar_t> &r0) const {
		world_to_local_point(r.r0,r0.r0);
		world_to_local_vector(r.a,r0.a);
	}

	/**
	 * @brief Transform plane from world to local OBB coordinates
	 *
	 * @param[out] p Output plane in local coordinates
	 * @param[in] p0 Input plane in world coordinates
	 */
	void world_to_local_plane(plane<scalar_t> &p, plane<scalar_t> const &p0) const;

	/**
	 * @brief Test if point is inside OBB
	 *
	 * @param v Point to test
	 * @return true if point is inside or on the boundary
	 */
	bool contains(vec_t const &v) const {
		vec_t v1;
		world_to_local_point(v1, v);
		return v1.x>=0 && v1.x<=1
		    && v1.y>=0 && v1.y<=1
		    && v1.z>=0 && v1.z<=1;
	}

	/**
	 * @brief Test if triangle is fully contained in OBB
	 *
	 * @param tri Triangle to test
	 * @return true if all triangle vertices are inside the OBB
	 */
	bool contains(const triangle<ARITY, scalar_t> &tri) const {
		return contains(tri.A) && contains(tri.B) && contains(tri.C);
	}

	/**
	 * @brief Test ray intersection with OBB
	 *
	 * @param r Ray to test
	 * @param t_min Minimum t value (default 1)
	 * @param t_max Maximum t value (default 1)
	 * @return true if ray intersects OBB
	 */
	 bool trace(const ray<ARITY, scalar_t> &r, scalar_t t_min = 1,
			 scalar_t t_max = 1) const;

	/**
	 * @brief Test if another OBB is fully contained
	 *
	 * @param r OBB to test
	 * @return true if r is completely inside this OBB
	 */
	bool contains(const obb &r) const;

	/**
	 * @brief Test intersection with triangle
	 *
	 * Uses separating axis theorem (SAT) for accurate intersection test.
	 *
	 * @param tri Triangle to test
	 * @return true if OBB and triangle intersect
	 */
	bool test_intersection(const triangle<ARITY, scalar_t> &tri) const {
		return !test_nonintersection(tri);
	}

	/**
	 * @brief Test intersection with another OBB
	 *
	 * Uses separating axis theorem (SAT) for accurate intersection test.
	 *
	 * @param box OBB to test
	 * @return true if the OBBs intersect
	 */
	bool test_intersection(const obb &box) const {
		return !test_nonintersection(box) && !box.test_nonintersection(*this);
	}

	/**
	 * @brief Test collision with moving triangle
	 *
	 * Checks if this OBB will collide with a moving triangle during
	 * the specified time interval.
	 *
	 * @param tri Triangle to test
	 * @param vel Relative velocity of the triangle
	 * @param t_min Start of time interval (default 0)
	 * @param t_max End of time interval (default 1)
	 * @return true if collision occurs in [t_min, t_max]
	 */
	bool test_collision(triangle<ARITY, scalar_t> const &tri, vec_t const &vel, scalar_t t_min = 0,
		scalar_t t_max = 1) const
	{
		return !test_noncollision(tri, vel, t_min, t_max);
	}

	/**
	 * @brief Test collision with moving OBB
	 *
	 * Checks if this OBB will collide with another moving OBB during
	 * the specified time interval.
	 *
	 * @param box OBB to test
	 * @param vel Relative velocity of the other box
	 * @param t_min Start of time interval (default 0)
	 * @param t_max End of time interval (default 1)
	 * @return true if collision occurs in [t_min, t_max]
	 */
	bool test_collision(obb const &box, vec_t const &vel, scalar_t t_min = 0,
		scalar_t t_max = 1) const
	{
		return !test_noncollision(box, vel, t_min, t_max)
		    && !box.test_noncollision(*this, -vel, t_min, t_max);
	}

	/**
	 * @brief Check if OBB basis vectors are linearly independent
	 *
	 * @return true if tangent, normal, and binormal are linearly independent
	 */
	bool is_basis_linearly_independent() const;

private:
	// check for non-intersection with triangle
	bool test_nonintersection(const triangle<ARITY, scalar_t> &r) const;

	// check for one-sided non-intersection with obb
	bool test_nonintersection(const obb &r) const;

	// check for one-sided non-collision with triangle
	bool test_noncollision(const triangle<ARITY, scalar_t> &r, vec_t const &vel, scalar_t t_min = 0,
		scalar_t t_max = 1) const;

	// check for one-sided non-collision with obb
	bool test_noncollision(const obb &r, vec_t const &vel, scalar_t t_min = 0,
		scalar_t t_max = 1) const;
};

}
