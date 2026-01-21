/**
 * @file aabb.h
 * @brief N-dimensional axis-aligned bounding box (AABB) class
 *
 * Provides axis-aligned bounding boxes for spatial queries and collision detection.
 * Commonly used specializations are 2D rectangles (N=2) and 3D boxes (N=3).
 */

#pragma once

#include <limits>
#include <iosfwd>
#include "scalar.h"
#include "vec.h"

#undef min
#undef max

namespace rove
{

template<int N,class T> class triangle;
template<int N,class T> class ray;

/**
 * @brief N-dimensional axis-aligned bounding box
 *
 * Represents a box aligned with the coordinate axes, defined by minimum (lo)
 * and maximum (hi) corner points. AABBs are commonly used for:
 * - Broad-phase collision detection
 * - Spatial partitioning (octrees, BVH)
 * - View frustum culling
 *
 * @tparam N Dimensionality (2 for rectangle, 3 for box)
 * @tparam T Scalar type (typically float or double)
 *
 * @note The lo corner should have smaller coordinates than hi for a valid AABB.
 *       Use normalize() to ensure this invariant after direct manipulation.
 */
template<int N,class T = scalar>
class aabb {
public:
	// types and constants
	typedef T scalar_t;                         ///< Scalar component type
	typedef std::numeric_limits<scalar_t> limits_t;  ///< Numeric limits for scalar type
	static size_t const ARITY = N;              ///< Number of dimensions
	typedef vec<ARITY,scalar_t> vec_t;          ///< Associated vector type
	static size_t const vertices_count = 1 << ARITY;  ///< Number of corner vertices (2^N)
	static size_t const edges_count = vertices_count + (1 << (ARITY-1));  ///< Number of edges
	typedef ray<ARITY,scalar_t> ray_t;          ///< Associated ray type

	vec_t lo;   ///< Minimum corner (lower bounds in all dimensions)
	vec_t hi;   ///< Maximum corner (upper bounds in all dimensions)

	/// Default constructor (uninitialized)
	aabb()
	{
	}

	/**
	 * @brief Construct degenerate AABB from single point
	 *
	 * Creates a zero-volume AABB where lo and hi are both set to the given point.
	 *
	 * @param centre The point to use as both corners
	 */
	aabb(vec_t const &centre):
		lo(centre),
		hi(centre)
	{
	}

	/**
	 * @brief Construct AABB from two corner points
	 *
	 * @param l Minimum corner (lo)
	 * @param h Maximum corner (hi)
	 * @note If l > h in any dimension, call normalize() to fix
	 */
	aabb(vec_t const &l,vec_t const &h):
		lo(l),
		hi(h)
	{
	}

	/**
	 * @brief Construct AABB from center and size
	 *
	 * Creates a box centered at the given point with the specified half-extent.
	 *
	 * @param centre Center point of the AABB
	 * @param radius Half the side length (applied to all dimensions)
	 */
	aabb(vec_t const &centre, rove::scalar radius)
	{
		vec_t delta;
		delta.set_all(radius / 2);

		lo = centre - delta;
		hi = centre + delta;
	}

	/**
	 * @brief Reset AABB to "null" state for accumulation
	 *
	 * Sets lo to maximum possible values and hi to minimum possible values.
	 * This prepares the AABB for building via extend() calls - the first
	 * extend() will set proper bounds, subsequent calls expand as needed.
	 */
	void null() {
		for(size_t n=0; n<ARITY; n++) {
			lo.i[n] = +limits_t::max();
			hi.i[n] = -limits_t::max();
		}
	}

	/**
	 * @brief Transform vector from world to local AABB coordinates
	 *
	 * Scales a direction vector to AABB-local space where the AABB
	 * has unit dimensions. Does not account for translation.
	 *
	 * @param[out] v Output vector in local coordinates
	 * @param[in] v0 Input vector in world coordinates
	 */
	void world_to_local_vector(vec_t &v,vec_t const &v0) const {
		v = v0 / (hi - lo);
	}

	/**
	 * @brief Transform point from world to local AABB coordinates
	 *
	 * Transforms a point to AABB-local space where lo maps to origin
	 * and hi maps to (1,1,...). Useful for computing barycentric-like
	 * coordinates within the box.
	 *
	 * @param[out] v Output point in local coordinates [0,1]^N for interior points
	 * @param[in] v0 Input point in world coordinates
	 */
	void world_to_local_point(vec_t &v,vec_t const &v0) const {
		world_to_local_vector(v, v0 - lo);
	}

	/**
	 * @brief Transform ray from world to local AABB coordinates
	 *
	 * Transforms both the ray origin and direction to AABB-local space.
	 *
	 * @param[out] r Output ray in local coordinates
	 * @param[in] r0 Input ray in world coordinates
	 */
	void world_to_local_ray(ray_t &r,ray_t const &r0) const {
		world_to_local_point(r.r0,r0.r0);
		world_to_local_vector(r.a,r0.a);
	}

	/**
	 * @brief Ensure lo <= hi in all dimensions
	 *
	 * Swaps lo and hi coordinates where necessary to maintain the
	 * invariant that lo[i] <= hi[i] for all dimensions i.
	 */
	void normalize() {
		minmax(lo,hi);
	}

	/**
	 * @brief Get AABB diagonal vector
	 *
	 * Returns the vector from lo to hi corner.
	 *
	 * @return Vector representing AABB dimensions (hi - lo)
	 */
	vec_t diagonal() const {
		return hi - lo;
	}

	/**
	 * @brief Get AABB center point
	 *
	 * @return Center point of the AABB
	 */
	vec_t centre() const {
		return (lo + hi) / scalar_t(2);
	}

	/**
	 * @brief Extend AABB to include a point
	 *
	 * Grows the AABB if necessary so that it contains the given point.
	 *
	 * @param v Point to include
	 */
	void extend(vec_t const &v) {
		for(size_t n=0; n<ARITY; n++) {
			if(lo.i[n] > v.i[n]) lo.i[n] = v.i[n];
			if(hi.i[n] < v.i[n]) hi.i[n] = v.i[n];
		}
	}

	/**
	 * @brief Extend AABB to include another AABB
	 *
	 * Grows this AABB to be the union of both boxes.
	 *
	 * @param b AABB to include
	 */
	void extend(aabb const &b) {
		extend(b.lo);
		extend(b.hi);
	}

	/**
	 * @brief Test if point is inside AABB
	 *
	 * @param right Point to test
	 * @return true if point is inside or on the boundary
	 */
	bool contains(vec_t const &right) const {
		return lo <= right && right <= hi;
	}

	/**
	 * @brief Test if another AABB is fully contained
	 *
	 * @param right AABB to test
	 * @return true if right is completely inside this AABB
	 */
	bool contains(aabb const &right) const {
		return contains(right.lo) && contains(right.hi);
	}

	/**
	 * @brief Test intersection with another AABB
	 *
	 * @param right AABB to test against
	 * @return true if the AABBs overlap (including touching)
	 */
	bool test_intersection(aabb const &right) const {
		return right.hi >= lo && hi >= right.lo;
	}

	/**
	 * @brief Ray trace against AABB with intersection times
	 *
	 * Finds where a ray enters and exits the AABB.
	 *
	 * @param r Ray to trace
	 * @param[out] t0 Time of entry (where ray enters AABB)
	 * @param[out] t1 Time of exit (where ray leaves AABB)
	 * @param t_min Minimum t value to consider (default 0)
	 * @param t_max Maximum t value to consider (default 1)
	 * @return true if ray intersects AABB in [t_min, t_max]
	 */
	bool trace(ray_t const &r, scalar_t *t0, scalar_t *t1, scalar_t t_min = 0, scalar_t t_max = 1) const;

	/**
	 * @brief Ray trace against AABB (intersection test only)
	 *
	 * @param r Ray to trace
	 * @param t_min Minimum t value to consider (default 0)
	 * @param t_max Maximum t value to consider (default 1)
	 * @return true if ray intersects AABB in [t_min, t_max]
	 */
	bool trace(ray_t const &r, scalar_t t_min = 0, scalar_t t_max = 1) const;

	/**
	 * @brief Serialize AABB for Boost.Serialization
	 */
	template<class Archive>
	void serialize(Archive &archive, unsigned const /*file_version*/)
	{
		archive & lo & hi;
	}
};

}

/**
 * @brief Stream output operator for AABB
 *
 * Outputs AABB in format "lo x hi"
 */
template<int N, class T>
std::ostream &operator <<(std::ostream &lhs, rove::aabb<N,T> &rhs)
{
	return lhs << rhs.lo << " x " << rhs.hi;
}
