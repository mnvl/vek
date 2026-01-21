
#include <boost/test/unit_test.hpp>
#include "aabb.h"
#include "obb.h"
#include "plane.h"

BOOST_AUTO_TEST_SUITE(plane)

BOOST_AUTO_TEST_CASE(get_origin_1)
{
	rove::plane<> p1(rove::vec<3>(1, -3, 5), rove::vec<3>(-7, 11, -13));
	rove::plane<> p2(p1.get_origin(), p1.get_normal());

	BOOST_REQUIRE(abs(p1.A - p2.A) < rove::EPSILON);
	BOOST_REQUIRE(abs(p1.B - p2.B) < rove::EPSILON);
	BOOST_REQUIRE(abs(p1.C - p2.C) < rove::EPSILON);
	BOOST_REQUIRE(abs(p1.D - p2.D) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(classify_point_1)
{
	rove::plane<> pl(rove::vec<3>(0, -1, 0), rove::vec<3>(1, 1, 1));

	BOOST_REQUIRE(pl.classify(rove::vec<3>(0, -2, 0)) == rove::plane<>::NEGATIVE);
	BOOST_REQUIRE(pl.classify(rove::vec<3>(0, 0, 0)) == rove::plane<>::POSITIVE);
	BOOST_REQUIRE(pl.classify(rove::vec<3>(0, -1, 0)) == rove::plane<>::INTERSECTS);
}

BOOST_AUTO_TEST_CASE(classify_point_2)
{
	rove::plane<> pl(rove::vec<3>(0, 0, 0), rove::vec<3>(1, -1, 1));

	BOOST_REQUIRE(pl.classify(rove::vec<3>(-2, -1.5, -2)) == rove::plane<>::NEGATIVE);
	BOOST_REQUIRE(pl.classify(rove::vec<3>(-1, -2, -1)) == rove::plane<>::INTERSECTS);
}

BOOST_AUTO_TEST_CASE(classify_aabb_1)
{
	rove::plane<> pl(rove::vec<3>(-1, -1, -1), rove::vec<3>(1, 1, 1));

	BOOST_REQUIRE(pl.classify(rove::aabb<3>(rove::vec<3>(-2, -2, -2), rove::vec<3>(-1, -1.5, -1))) == rove::plane<>::NEGATIVE);
	BOOST_REQUIRE(pl.classify(rove::aabb<3>(rove::vec<3>(-2, -2, -2), rove::vec<3>(-1, -1, -1))) == rove::plane<>::INTERSECTS);
	BOOST_REQUIRE(pl.classify(rove::aabb<3>(rove::vec<3>(0, 0, 0), rove::vec<3>(1, 1, 1))) == rove::plane<>::POSITIVE);
	BOOST_REQUIRE(pl.classify(rove::aabb<3>(rove::vec<3>(-1, -1, -1), rove::vec<3>(1, 1, 1))) == rove::plane<>::INTERSECTS);
}

BOOST_AUTO_TEST_CASE(classify_aabb_2)
{
	rove::plane<> pl(rove::vec<3>(-1, -1, -1), rove::vec<3>(-1, -1, -1));

	BOOST_REQUIRE(pl.classify(rove::aabb<3>(rove::vec<3>(-2, -2, -2), rove::vec<3>(-1, -1.5, -1))) == rove::plane<>::POSITIVE);
	BOOST_REQUIRE(pl.classify(rove::aabb<3>(rove::vec<3>(-2, -2, -2), rove::vec<3>(-1, -1, -1))) == rove::plane<>::INTERSECTS);
	BOOST_REQUIRE(pl.classify(rove::aabb<3>(rove::vec<3>(0, 0, 0), rove::vec<3>(1, 1, 1))) == rove::plane<>::NEGATIVE);
	BOOST_REQUIRE(pl.classify(rove::aabb<3>(rove::vec<3>(-1, -1, -1), rove::vec<3>(1, 1, 1))) == rove::plane<>::INTERSECTS);
}

BOOST_AUTO_TEST_CASE(classify_aabb_3)
{
	rove::plane<> pl(rove::vec<3>(0, 0, 0), rove::vec<3>(1, -1, 1));

	BOOST_REQUIRE(pl.classify(rove::aabb<3>(rove::vec<3>(0, 4, 0), rove::vec<3>(1, 5, 1))) == rove::plane<>::NEGATIVE);
	BOOST_REQUIRE(pl.classify(rove::aabb<3>(rove::vec<3>(-1, -5, -1), rove::vec<3>(0, -4, 0))) == rove::plane<>::POSITIVE);
	BOOST_REQUIRE(pl.classify(rove::aabb<3>(rove::vec<3>(-2, -2, -2), rove::vec<3>(0, 0, 0))) == rove::plane<>::INTERSECTS);
	BOOST_REQUIRE(pl.classify(rove::aabb<3>(rove::vec<3>(-1, -1, -1), rove::vec<3>(1, 1, 1))) == rove::plane<>::INTERSECTS);
}

BOOST_AUTO_TEST_CASE(classify_aabb_4)
{
	rove::plane<> pl(rove::vec<3>(0, 2, 0), rove::vec<3>(1, 1, 1));

	BOOST_REQUIRE(pl.classify(rove::aabb<3>(rove::vec<3>(-1, -1, -1), rove::vec<3>(0, 0, 0))) == rove::plane<>::NEGATIVE);
}

BOOST_AUTO_TEST_CASE(classify_obb_1)
{
	rove::plane<> pl(rove::vec<3>(0, 0, 0), rove::vec<3>(1, 1, 1));

	BOOST_REQUIRE(pl.classify(
			rove::obb<3>(rove::vec<3>(0, 1, 0), rove::vec<3>(1, 0, 0), rove::vec<3>(0, 1, 0), rove::vec<3>(0, 0, 1)))
		== rove::plane<>::POSITIVE);

	BOOST_REQUIRE(pl.classify(
			rove::obb<3>(rove::vec<3>(-1, -2, -1), rove::vec<3>(1, 0, 0), rove::vec<3>(0, 1, 0), rove::vec<3>(0, 0, 1)))
		== rove::plane<>::NEGATIVE);
}

BOOST_AUTO_TEST_CASE(classify_obb_2)
{
	rove::plane<> pl(rove::vec<3>(0, 0, 0), rove::vec<3>(1, 1, 1));

	BOOST_REQUIRE(pl.classify(
			rove::obb<3>(
				rove::vec<3>(-1, -1, -1),
				rove::vec<3>(11, 10, 10),
				rove::vec<3>(10, 11, 10),
				rove::vec<3>(10, 10, 11)))
		== rove::plane<>::INTERSECTS);
}

BOOST_AUTO_TEST_CASE(classify_obb_3)
{
	rove::plane<> pl;
	pl.A =  0.00000000f;
	pl.B = -0.26937401f;
	pl.C = -0.26937401f;
	pl.D =  1.00618340f;

	rove::obb<3> bound;
	bound.origin.set(-5.6245117f, 0.00000000f, 23.716305f);
	bound.tangent.set(2.0000000f, 0.00000000f, 0.00000000f);
	bound.normal.set(0.00000000f, 1.0000000f, 0.00000000f);
	bound.binormal.set(0.00000000f, 0.00000000f, 2.0000000f);

	BOOST_REQUIRE (pl.classify(bound.origin) == rove::plane<>::NEGATIVE);
	BOOST_REQUIRE (pl.classify(bound.origin + bound.tangent) == rove::plane<>::NEGATIVE);
	BOOST_REQUIRE (pl.classify(bound.origin + bound.normal) == rove::plane<>::NEGATIVE);
	BOOST_REQUIRE (pl.classify(bound.origin + bound.binormal) == rove::plane<>::NEGATIVE);

	BOOST_REQUIRE (pl.classify(bound) == rove::plane<>::NEGATIVE);
}

BOOST_AUTO_TEST_CASE(raytrace_1)
{
	rove::plane<> pl(rove::vec<3>(0, 0, 0), rove::vec<3>(0, 1, 0));

	rove::ray<3> r(rove::vec<3>(0, 1, 0), rove::vec<3>(0, -10, 0));

	rove::scalar t;
	rove::vec<3> v;

	pl.trace(r, t, v);

	BOOST_REQUIRE((v - rove::vec<3>(0, 0, 0)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(raytrace_2)
{
	rove::plane<> pl(rove::vec<3>(0, 0, 0), rove::vec<3>(0, 1, 0));

	rove::ray<3> r(rove::vec<3>(7, 1, 11), rove::vec<3>(0, -10, 0));

	rove::scalar t;
	rove::vec<3> v;

	pl.trace(r, t, v);

	BOOST_REQUIRE((v - rove::vec<3>(7, 0, 11)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(raytrace_3)
{
	rove::plane<> pl(rove::vec<3>(1, 3, 5), rove::vec<3>(0, 1, 0));

	rove::ray<3> r(rove::vec<3>(7 + 1, 1 + 3, 11 + 5), rove::vec<3>(0, -10, 0));

	rove::scalar t;
	rove::vec<3> v;

	pl.trace(r, t, v);

	BOOST_REQUIRE((v - rove::vec<3>(7 + 1, 0 + 3, 11 + 5)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(raytrace_4)
{
	rove::plane<> pl(rove::vec<3>(1, 3, 5), rove::vec<3>(1, 1, 1));

	rove::ray<3> r(rove::vec<3>(1, 10, 5), rove::vec<3>(0, -10, 0));

	rove::scalar t;
	rove::vec<3> v;

	pl.trace(r, t, v);

	BOOST_REQUIRE((v - rove::vec<3>(1, 3, 5)).length() < rove::EPSILON);
}

// Additional comprehensive tests

// Plane construction tests
BOOST_AUTO_TEST_CASE(construct_from_origin_normal)
{
	rove::vec<3> origin(1, 2, 3);
	rove::vec<3> normal(0, 1, 0);
	rove::plane<> pl(origin, normal);

	rove::vec<3> recovered_normal = pl.get_normal();
	BOOST_REQUIRE((recovered_normal - normal).length() < rove::EPSILON);
	BOOST_REQUIRE(pl.contains(origin));
}

BOOST_AUTO_TEST_CASE(construct_from_d_normal)
{
	rove::vec<3> normal(0, 1, 0);
	rove::scalar d = -5.0f;
	rove::plane<> pl(d, normal);

	BOOST_REQUIRE(rove::abs(pl.A - normal.x) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(pl.B - normal.y) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(pl.C - normal.z) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(pl.D - d) < rove::EPSILON);
}

// Apply and contains tests
BOOST_AUTO_TEST_CASE(apply_to_point_on_plane)
{
	rove::plane<> pl(rove::vec<3>(0, 5, 0), rove::vec<3>(0, 1, 0));

	rove::scalar result = pl.apply(rove::vec<3>(0, 5, 0));
	BOOST_REQUIRE(rove::abs(result) < rove::EPSILON);

	result = pl.apply(rove::vec<3>(10, 5, 10));
	BOOST_REQUIRE(rove::abs(result) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(contains_point_on_plane)
{
	rove::plane<> pl(rove::vec<3>(0, 5, 0), rove::vec<3>(0, 1, 0));

	BOOST_REQUIRE(pl.contains(rove::vec<3>(0, 5, 0)));
	BOOST_REQUIRE(pl.contains(rove::vec<3>(100, 5, -50)));
	BOOST_REQUIRE(!pl.contains(rove::vec<3>(0, 6, 0)));
}

BOOST_AUTO_TEST_CASE(contains_ray_on_plane)
{
	rove::plane<> pl(rove::vec<3>(0, 0, 0), rove::vec<3>(0, 1, 0));

	rove::ray<3> r_on_plane(rove::vec<3>(0, 0, 0), rove::vec<3>(1, 0, 0));
	rove::ray<3> r_not_on_plane(rove::vec<3>(0, 1, 0), rove::vec<3>(1, 0, 0));

	BOOST_REQUIRE(pl.contains(r_on_plane));
	BOOST_REQUIRE(!pl.contains(r_not_on_plane));
}

// Parallel and intersection tests
BOOST_AUTO_TEST_CASE(parallel_ray)
{
	rove::plane<> pl(rove::vec<3>(0, 0, 0), rove::vec<3>(0, 1, 0));

	rove::ray<3> r_parallel(rove::vec<3>(5, 5, 5), rove::vec<3>(1, 0, 0));
	rove::ray<3> r_not_parallel(rove::vec<3>(5, 5, 5), rove::vec<3>(0, 1, 0));

	BOOST_REQUIRE(pl.parallel(r_parallel));
	BOOST_REQUIRE(!pl.parallel(r_not_parallel));
}

BOOST_AUTO_TEST_CASE(test_intersection_ray)
{
	rove::plane<> pl(rove::vec<3>(0, 0, 0), rove::vec<3>(0, 1, 0));

	rove::ray<3> r_intersects(rove::vec<3>(0, 5, 0), rove::vec<3>(0, -1, 0));
	rove::ray<3> r_parallel(rove::vec<3>(5, 5, 5), rove::vec<3>(1, 0, 0));

	BOOST_REQUIRE(pl.test_intersection(r_intersects));
	BOOST_REQUIRE(!pl.test_intersection(r_parallel));
}

// Trace with t parameter tests
BOOST_AUTO_TEST_CASE(trace_with_t_in_range)
{
	rove::plane<> pl(rove::vec<3>(0, 0, 0), rove::vec<3>(0, 1, 0));

	rove::ray<3> r(rove::vec<3>(0, 10, 0), rove::vec<3>(0, -1, 0));

	rove::scalar t;
	BOOST_REQUIRE(pl.trace(r, t, 0, 20));
	BOOST_REQUIRE(rove::abs(t - 10.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(trace_with_t_out_of_range)
{
	rove::plane<> pl(rove::vec<3>(0, 0, 0), rove::vec<3>(0, 1, 0));

	rove::ray<3> r(rove::vec<3>(0, 10, 0), rove::vec<3>(0, -1, 0));

	rove::scalar t;
	// t would be 10, but t_max is 5
	BOOST_REQUIRE(!pl.trace(r, t, 0, 5));
}

// Plane-plane intersection
BOOST_AUTO_TEST_CASE(plane_plane_intersection)
{
	rove::plane<> pl1(rove::vec<3>(0, 0, 0), rove::vec<3>(0, 1, 0));
	rove::plane<> pl2(rove::vec<3>(0, 0, 0), rove::vec<3>(1, 0, 0));

	rove::ray<3> intersection_line;
	BOOST_REQUIRE(pl1.query_intersection(pl2, intersection_line));

	// The intersection line should lie on both planes
	BOOST_REQUIRE(pl1.contains(intersection_line.r0));
	BOOST_REQUIRE(pl2.contains(intersection_line.r0));
}

BOOST_AUTO_TEST_CASE(plane_plane_parallel)
{
	rove::plane<> pl1(rove::vec<3>(0, 0, 0), rove::vec<3>(0, 1, 0));
	rove::plane<> pl2(rove::vec<3>(0, 5, 0), rove::vec<3>(0, 1, 0));

	rove::ray<3> intersection_line;
	// Parallel planes don't intersect
	BOOST_REQUIRE(!pl1.query_intersection(pl2, intersection_line));
}

// Get origin and normal tests
BOOST_AUTO_TEST_CASE(get_normal)
{
	rove::vec<3> normal(1, 2, 3);
	rove::plane<> pl(rove::vec<3>(0, 0, 0), normal);

	rove::vec<3> recovered_normal = pl.get_normal();
	BOOST_REQUIRE((recovered_normal - normal).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(is_correct)
{
	rove::plane<> pl1(rove::vec<3>(0, 0, 0), rove::vec<3>(0, 1, 0));
	BOOST_REQUIRE(pl1.is_correct());

	rove::plane<> pl2;
	pl2.A = 0;
	pl2.B = 0;
	pl2.C = 0;
	pl2.D = 5;
	BOOST_REQUIRE(!pl2.is_correct());
}

// Edge cases
BOOST_AUTO_TEST_CASE(raytrace_perpendicular)
{
	rove::plane<> pl(rove::vec<3>(0, 0, 0), rove::vec<3>(0, 1, 0));

	rove::ray<3> r(rove::vec<3>(0, 10, 0), rove::vec<3>(0, -1, 0));

	rove::scalar t = pl.trace(r);
	BOOST_REQUIRE(rove::abs(t - 10.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(raytrace_oblique_angle)
{
	rove::plane<> pl(rove::vec<3>(0, 0, 0), rove::vec<3>(0, 1, 0));

	rove::ray<3> r(rove::vec<3>(0, 10, 0), rove::vec<3>(1, -1, 0));

	rove::scalar t;
	rove::vec<3> intersection;
	BOOST_REQUIRE(pl.trace(r, t, intersection, 0, 100));

	// Check that intersection point is on the plane
	BOOST_REQUIRE(pl.contains(intersection));
}

BOOST_AUTO_TEST_CASE(classify_point_far_positive)
{
	rove::plane<> pl(rove::vec<3>(0, 0, 0), rove::vec<3>(0, 1, 0));

	BOOST_REQUIRE(pl.classify(rove::vec<3>(0, 100, 0)) == rove::plane<>::POSITIVE);
	BOOST_REQUIRE(pl.classify(rove::vec<3>(0, -100, 0)) == rove::plane<>::NEGATIVE);
}

BOOST_AUTO_TEST_CASE(classify_aabb_completely_on_one_side)
{
	rove::plane<> pl(rove::vec<3>(0, 10, 0), rove::vec<3>(0, 1, 0));

	rove::aabb<3> box_above(rove::vec<3>(-5, 15, -5), rove::vec<3>(5, 20, 5));
	rove::aabb<3> box_below(rove::vec<3>(-5, 0, -5), rove::vec<3>(5, 5, 5));

	BOOST_REQUIRE(pl.classify(box_above) == rove::plane<>::POSITIVE);
	BOOST_REQUIRE(pl.classify(box_below) == rove::plane<>::NEGATIVE);
}

BOOST_AUTO_TEST_SUITE_END()

