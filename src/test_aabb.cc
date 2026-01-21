
#include <boost/test/unit_test.hpp>
#include "ray.h"
#include "aabb.h"

BOOST_AUTO_TEST_SUITE(aabb)

// Construction tests
BOOST_AUTO_TEST_CASE(construct_from_point)
{
	rove::vec<3> point(1, 2, 3);
	rove::aabb<3> b(point);

	BOOST_REQUIRE((b.lo - point).length() < rove::EPSILON);
	BOOST_REQUIRE((b.hi - point).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(construct_from_corners)
{
	rove::vec<3> lo(1, 2, 3);
	rove::vec<3> hi(4, 5, 6);
	rove::aabb<3> b(lo, hi);

	BOOST_REQUIRE((b.lo - lo).length() < rove::EPSILON);
	BOOST_REQUIRE((b.hi - hi).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(construct_from_center_radius)
{
	rove::vec<3> centre(5, 5, 5);
	rove::scalar radius = 10.0f;
	rove::aabb<3> b(centre, radius);

	rove::vec<3> expected_lo(0, 0, 0);
	rove::vec<3> expected_hi(10, 10, 10);

	BOOST_REQUIRE((b.lo - expected_lo).length() < rove::EPSILON);
	BOOST_REQUIRE((b.hi - expected_hi).length() < rove::EPSILON);
}

// Null and normalize tests
BOOST_AUTO_TEST_CASE(null_initialization)
{
	rove::aabb<3> b;
	b.null();

	// After null(), lo should be max and hi should be min
	BOOST_REQUIRE(b.lo.x > 0);
	BOOST_REQUIRE(b.hi.x < 0);
}

BOOST_AUTO_TEST_CASE(normalize_swaps_corners)
{
	rove::aabb<3> b(rove::vec<3>(5, 5, 5), rove::vec<3>(0, 0, 0));
	b.normalize();

	BOOST_REQUIRE((b.lo - rove::vec<3>(0, 0, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((b.hi - rove::vec<3>(5, 5, 5)).length() < rove::EPSILON);
}

// Geometric property tests
BOOST_AUTO_TEST_CASE(diagonal_calculation)
{
	rove::aabb<3> b(rove::vec<3>(0, 0, 0), rove::vec<3>(3, 4, 5));
	rove::vec<3> diag = b.diagonal();

	BOOST_REQUIRE((diag - rove::vec<3>(3, 4, 5)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(centre_calculation)
{
	rove::aabb<3> b(rove::vec<3>(0, 0, 0), rove::vec<3>(4, 6, 8));
	rove::vec<3> c = b.centre();

	BOOST_REQUIRE((c - rove::vec<3>(2, 3, 4)).length() < rove::EPSILON);
}

// Contains tests
BOOST_AUTO_TEST_CASE(contains_point_inside)
{
	rove::aabb<3> b(rove::vec<3>(0, 0, 0), rove::vec<3>(10, 10, 10));

	BOOST_REQUIRE(b.contains(rove::vec<3>(5, 5, 5)));
	BOOST_REQUIRE(b.contains(rove::vec<3>(0, 0, 0))); // boundary
	BOOST_REQUIRE(b.contains(rove::vec<3>(10, 10, 10))); // boundary
}

BOOST_AUTO_TEST_CASE(contains_point_outside)
{
	rove::aabb<3> b(rove::vec<3>(0, 0, 0), rove::vec<3>(10, 10, 10));

	BOOST_REQUIRE(!b.contains(rove::vec<3>(15, 5, 5)));
	BOOST_REQUIRE(!b.contains(rove::vec<3>(-1, 5, 5)));
	BOOST_REQUIRE(!b.contains(rove::vec<3>(5, 11, 5)));
}

BOOST_AUTO_TEST_CASE(contains_aabb_fully_inside)
{
	rove::aabb<3> b1(rove::vec<3>(0, 0, 0), rove::vec<3>(10, 10, 10));
	rove::aabb<3> b2(rove::vec<3>(2, 2, 2), rove::vec<3>(8, 8, 8));

	BOOST_REQUIRE(b1.contains(b2));
}

BOOST_AUTO_TEST_CASE(contains_aabb_partially_outside)
{
	rove::aabb<3> b1(rove::vec<3>(0, 0, 0), rove::vec<3>(10, 10, 10));
	rove::aabb<3> b2(rove::vec<3>(5, 5, 5), rove::vec<3>(15, 15, 15));

	BOOST_REQUIRE(!b1.contains(b2));
}

// Extend tests
BOOST_AUTO_TEST_CASE(extend_by_point)
{
	rove::aabb<3> b(rove::vec<3>(0, 0, 0), rove::vec<3>(5, 5, 5));
	b.extend(rove::vec<3>(10, 3, 3));

	BOOST_REQUIRE((b.lo - rove::vec<3>(0, 0, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((b.hi - rove::vec<3>(10, 5, 5)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(extend_by_point_negative)
{
	rove::aabb<3> b(rove::vec<3>(0, 0, 0), rove::vec<3>(5, 5, 5));
	b.extend(rove::vec<3>(-2, 3, 3));

	BOOST_REQUIRE((b.lo - rove::vec<3>(-2, 0, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((b.hi - rove::vec<3>(5, 5, 5)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(extend_by_aabb)
{
	rove::aabb<3> b1(rove::vec<3>(0, 0, 0), rove::vec<3>(5, 5, 5));
	rove::aabb<3> b2(rove::vec<3>(3, 3, 3), rove::vec<3>(8, 8, 8));

	b1.extend(b2);

	BOOST_REQUIRE((b1.lo - rove::vec<3>(0, 0, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((b1.hi - rove::vec<3>(8, 8, 8)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(extend_null_aabb)
{
	rove::aabb<3> b;
	b.null();

	b.extend(rove::vec<3>(1, 2, 3));
	b.extend(rove::vec<3>(4, 5, 6));

	BOOST_REQUIRE((b.lo - rove::vec<3>(1, 2, 3)).length() < rove::EPSILON);
	BOOST_REQUIRE((b.hi - rove::vec<3>(4, 5, 6)).length() < rove::EPSILON);
}

// Intersection tests
BOOST_AUTO_TEST_CASE(test_intersection_overlapping)
{
	rove::aabb<3> b1(rove::vec<3>(0, 0, 0), rove::vec<3>(5, 5, 5));
	rove::aabb<3> b2(rove::vec<3>(3, 3, 3), rove::vec<3>(8, 8, 8));

	BOOST_REQUIRE(b1.test_intersection(b2));
	BOOST_REQUIRE(b2.test_intersection(b1));
}

BOOST_AUTO_TEST_CASE(test_intersection_touching)
{
	rove::aabb<3> b1(rove::vec<3>(0, 0, 0), rove::vec<3>(5, 5, 5));
	rove::aabb<3> b2(rove::vec<3>(5, 0, 0), rove::vec<3>(10, 5, 5));

	BOOST_REQUIRE(b1.test_intersection(b2));
}

BOOST_AUTO_TEST_CASE(test_intersection_separated)
{
	rove::aabb<3> b1(rove::vec<3>(0, 0, 0), rove::vec<3>(5, 5, 5));
	rove::aabb<3> b2(rove::vec<3>(10, 10, 10), rove::vec<3>(15, 15, 15));

	BOOST_REQUIRE(!b1.test_intersection(b2));
}

// Ray tracing tests
BOOST_AUTO_TEST_CASE(trace_through_corners)
{
	rove::aabb<3> b(rove::vec<3>(0, 0, 0), 20.0f);
	rove::ray<3> r(rove::vec<3>(-20, -20, -20), rove::vec<3>(40, 40, 40));

	rove::scalar t0, t1;

	b.trace(r, &t0, &t1);

	BOOST_REQUIRE (rove::abs(t0 - 0.25f) < rove::EPSILON);
	BOOST_REQUIRE (rove::abs(t1 - 0.75f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(trace_from_inside)
{
	rove::aabb<3> b(rove::vec<3>(0, 0, 0), rove::vec<3>(10, 10, 10));
	rove::ray<3> r(rove::vec<3>(5, 5, 5), rove::vec<3>(1, 0, 0));

	rove::scalar t0, t1;
	BOOST_REQUIRE(b.trace(r, &t0, &t1, -100, 100));
}

BOOST_AUTO_TEST_CASE(trace_axis_aligned)
{
	rove::aabb<3> b(rove::vec<3>(0, 0, 0), rove::vec<3>(10, 10, 10));
	rove::ray<3> r(rove::vec<3>(-5, 5, 5), rove::vec<3>(1, 0, 0));

	rove::scalar t0, t1;
	BOOST_REQUIRE(b.trace(r, &t0, &t1, 0, 100));

	// Ray should enter at x=0 (t0=5) and exit at x=10 (t1=15)
	BOOST_REQUIRE(rove::abs(t0 - 5.0f) < 0.01f);
	BOOST_REQUIRE(rove::abs(t1 - 15.0f) < 0.01f);
}

BOOST_AUTO_TEST_CASE(trace_miss)
{
	rove::aabb<3> b(rove::vec<3>(0, 0, 0), rove::vec<3>(10, 10, 10));
	rove::ray<3> r(rove::vec<3>(-5, 20, 5), rove::vec<3>(1, 0, 0));

	BOOST_REQUIRE(!b.trace(r));
}

BOOST_AUTO_TEST_CASE(trace_with_t_range)
{
	rove::aabb<3> b(rove::vec<3>(0, 0, 0), rove::vec<3>(10, 10, 10));
	rove::ray<3> r(rove::vec<3>(-5, 5, 5), rove::vec<3>(1, 0, 0));

	rove::scalar t0, t1;
	// With t_min=0, t_max=1, ray doesn't reach the box
	BOOST_REQUIRE(!b.trace(r, &t0, &t1, 0, 1));

	// With larger t_max, it should hit
	BOOST_REQUIRE(b.trace(r, &t0, &t1, 0, 20));
}

// World to local transformation tests
BOOST_AUTO_TEST_CASE(world_to_local_vector)
{
	rove::aabb<3> b(rove::vec<3>(0, 0, 0), rove::vec<3>(10, 20, 30));
	rove::vec<3> world_vec(5, 10, 15);
	rove::vec<3> local_vec;

	b.world_to_local_vector(local_vec, world_vec);

	BOOST_REQUIRE(rove::abs(local_vec.x - 0.5f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(local_vec.y - 0.5f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(local_vec.z - 0.5f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(world_to_local_point)
{
	rove::aabb<3> b(rove::vec<3>(10, 10, 10), rove::vec<3>(20, 30, 40));
	rove::vec<3> world_point(15, 20, 25);
	rove::vec<3> local_point;

	b.world_to_local_point(local_point, world_point);

	BOOST_REQUIRE(rove::abs(local_point.x - 0.5f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(local_point.y - 0.5f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(local_point.z - 0.5f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(world_to_local_ray)
{
	rove::aabb<3> b(rove::vec<3>(0, 0, 0), rove::vec<3>(10, 10, 10));
	rove::ray<3> world_ray(rove::vec<3>(5, 5, 5), rove::vec<3>(10, 0, 0));
	rove::ray<3> local_ray;

	b.world_to_local_ray(local_ray, world_ray);

	// Origin should be at (0.5, 0.5, 0.5) in local space
	BOOST_REQUIRE(rove::abs(local_ray.r0.x - 0.5f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(local_ray.r0.y - 0.5f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(local_ray.r0.z - 0.5f) < rove::EPSILON);

	// Direction should be scaled
	BOOST_REQUIRE(rove::abs(local_ray.a.x - 1.0f) < rove::EPSILON);
}

// 2D AABB tests
BOOST_AUTO_TEST_CASE(aabb_2d_basic)
{
	rove::aabb<2> b(rove::vec<2>(0, 0), rove::vec<2>(10, 10));

	BOOST_REQUIRE(b.contains(rove::vec<2>(5, 5)));
	BOOST_REQUIRE(!b.contains(rove::vec<2>(15, 5)));

	rove::vec<2> c = b.centre();
	BOOST_REQUIRE((c - rove::vec<2>(5, 5)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(aabb_2d_intersection)
{
	rove::aabb<2> b1(rove::vec<2>(0, 0), rove::vec<2>(5, 5));
	rove::aabb<2> b2(rove::vec<2>(3, 3), rove::vec<2>(8, 8));

	BOOST_REQUIRE(b1.test_intersection(b2));
}

BOOST_AUTO_TEST_SUITE_END()

