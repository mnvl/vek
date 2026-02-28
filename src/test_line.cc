
#include <boost/test/unit_test.hpp>
#include "line.h"

BOOST_AUTO_TEST_SUITE(line_test)

// --- Construction ---

BOOST_AUTO_TEST_CASE(construct_2d)
{
	rove::line<2> l(rove::vec<2>(1, 2), rove::vec<2>(4, 6));
	BOOST_REQUIRE((l.A - rove::vec<2>(1, 2)).length() < rove::EPSILON);
	BOOST_REQUIRE((l.B - rove::vec<2>(4, 6)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(construct_3d)
{
	rove::line<3> l(rove::vec<3>(0, 0, 0), rove::vec<3>(1, 2, 3));
	BOOST_REQUIRE((l.A - rove::vec<3>(0, 0, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((l.B - rove::vec<3>(1, 2, 3)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(construct_via_construct_method)
{
	rove::line<2> l;
	l.construct(rove::vec<2>(3, 4), rove::vec<2>(7, 8));
	BOOST_REQUIRE((l.A - rove::vec<2>(3, 4)).length() < rove::EPSILON);
	BOOST_REQUIRE((l.B - rove::vec<2>(7, 8)).length() < rove::EPSILON);
}

// --- direction() ---

BOOST_AUTO_TEST_CASE(direction_2d)
{
	rove::line<2> l(rove::vec<2>(1, 2), rove::vec<2>(4, 5));
	rove::vec<2> d = l.direction();
	BOOST_REQUIRE((d - rove::vec<2>(3, 3)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(direction_3d)
{
	rove::line<3> l(rove::vec<3>(1, 0, 0), rove::vec<3>(1, 0, 5));
	rove::vec<3> d = l.direction();
	BOOST_REQUIRE((d - rove::vec<3>(0, 0, 5)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(direction_is_b_minus_a)
{
	using rove::random;
	for (int i = 0; i < 100; i++)
	{
		rove::vec<3> a(random(-10, 10), random(-10, 10), random(-10, 10));
		rove::vec<3> b(random(-10, 10), random(-10, 10), random(-10, 10));
		rove::line<3> l(a, b);
		BOOST_REQUIRE((l.direction() - (b - a)).length() < rove::EPSILON);
	}
}

// --- get_ray() ---

BOOST_AUTO_TEST_CASE(get_ray_origin)
{
	rove::line<3> l(rove::vec<3>(1, 2, 3), rove::vec<3>(4, 5, 6));
	rove::ray<3> r = l.get_ray();
	BOOST_REQUIRE((r.r0 - rove::vec<3>(1, 2, 3)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(get_ray_direction)
{
	rove::line<3> l(rove::vec<3>(1, 2, 3), rove::vec<3>(4, 5, 6));
	rove::ray<3> r = l.get_ray();
	BOOST_REQUIRE((r.a - rove::vec<3>(3, 3, 3)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(get_ray_2d)
{
	rove::line<2> l(rove::vec<2>(0, 0), rove::vec<2>(3, 4));
	rove::ray<2> r = l.get_ray();
	BOOST_REQUIRE((r.r0 - rove::vec<2>(0, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((r.a - rove::vec<2>(3, 4)).length() < rove::EPSILON);
}

// --- get_aabb() ---

BOOST_AUTO_TEST_CASE(get_aabb_2d_normal_order)
{
	rove::line<2> l(rove::vec<2>(1, 2), rove::vec<2>(5, 7));
	rove::aabb<2> b = l.get_aabb();
	BOOST_REQUIRE(b.lo.x <= 1.0f + rove::EPSILON);
	BOOST_REQUIRE(b.lo.y <= 2.0f + rove::EPSILON);
	BOOST_REQUIRE(b.hi.x >= 5.0f - rove::EPSILON);
	BOOST_REQUIRE(b.hi.y >= 7.0f - rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(get_aabb_2d_reversed)
{
	// A and B may be in any order; get_aabb must normalize lo/hi
	rove::line<2> l(rove::vec<2>(5, 7), rove::vec<2>(1, 2));
	rove::aabb<2> b = l.get_aabb();
	BOOST_REQUIRE(b.lo.x <= 1.0f + rove::EPSILON);
	BOOST_REQUIRE(b.lo.y <= 2.0f + rove::EPSILON);
	BOOST_REQUIRE(b.hi.x >= 5.0f - rove::EPSILON);
	BOOST_REQUIRE(b.hi.y >= 7.0f - rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(get_aabb_3d)
{
	rove::line<3> l(rove::vec<3>(1, 2, 3), rove::vec<3>(4, 0, 6));
	rove::aabb<3> b = l.get_aabb();
	BOOST_REQUIRE(b.lo.x <= 1.0f + rove::EPSILON);
	BOOST_REQUIRE(b.lo.y <= 0.0f + rove::EPSILON);
	BOOST_REQUIRE(b.lo.z <= 3.0f + rove::EPSILON);
	BOOST_REQUIRE(b.hi.x >= 4.0f - rove::EPSILON);
	BOOST_REQUIRE(b.hi.y >= 2.0f - rove::EPSILON);
	BOOST_REQUIRE(b.hi.z >= 6.0f - rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(get_aabb_contains_both_endpoints)
{
	using rove::random;
	for (int i = 0; i < 50; i++)
	{
		rove::vec<3> a(random(-10, 10), random(-10, 10), random(-10, 10));
		rove::vec<3> b(random(-10, 10), random(-10, 10), random(-10, 10));
		rove::line<3> l(a, b);
		rove::aabb<3> box = l.get_aabb();
		BOOST_REQUIRE(box.contains(a));
		BOOST_REQUIRE(box.contains(b));
	}
}

// --- length_sq() ---

BOOST_AUTO_TEST_CASE(length_sq_horizontal)
{
	rove::line<2> l(rove::vec<2>(0, 0), rove::vec<2>(3, 4));
	BOOST_REQUIRE(rove::abs(l.length_sq() - 25.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(length_sq_3d)
{
	rove::line<3> l(rove::vec<3>(1, 1, 1), rove::vec<3>(3, 1, 1));
	BOOST_REQUIRE(rove::abs(l.length_sq() - 4.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(length_sq_unit)
{
	rove::line<3> l(rove::vec<3>(0, 0, 0), rove::vec<3>(1, 0, 0));
	BOOST_REQUIRE(rove::abs(l.length_sq() - 1.0f) < rove::EPSILON);
}

// --- contains() ---
//
// contains() uses strict inequalities: both endpoints A and B are excluded.
// The open segment (A, B) is tested.

BOOST_AUTO_TEST_CASE(contains_midpoint_2d)
{
	rove::line<2> l(rove::vec<2>(0, 0), rove::vec<2>(4, 0));
	BOOST_REQUIRE(l.contains(rove::vec<2>(2, 0)));
}

BOOST_AUTO_TEST_CASE(contains_interior_points_3d)
{
	rove::line<3> l(rove::vec<3>(0, 0, 0), rove::vec<3>(6, 0, 0));
	BOOST_REQUIRE(l.contains(rove::vec<3>(1, 0, 0)));
	BOOST_REQUIRE(l.contains(rove::vec<3>(3, 0, 0)));
	BOOST_REQUIRE(l.contains(rove::vec<3>(5, 0, 0)));
}

BOOST_AUTO_TEST_CASE(contains_endpoint_a_not_included)
{
	rove::line<2> l(rove::vec<2>(0, 0), rove::vec<2>(4, 0));
	// At A: dot product (u & v) == 0, not > 0
	BOOST_REQUIRE(!l.contains(rove::vec<2>(0, 0)));
}

BOOST_AUTO_TEST_CASE(contains_endpoint_b_not_included)
{
	rove::line<2> l(rove::vec<2>(0, 0), rove::vec<2>(4, 0));
	// At B: u.length_sq() == v.length_sq(), not strictly <
	BOOST_REQUIRE(!l.contains(rove::vec<2>(4, 0)));
}

BOOST_AUTO_TEST_CASE(contains_point_beyond_b)
{
	rove::line<2> l(rove::vec<2>(0, 0), rove::vec<2>(4, 0));
	BOOST_REQUIRE(!l.contains(rove::vec<2>(5, 0)));
	BOOST_REQUIRE(!l.contains(rove::vec<2>(10, 0)));
}

BOOST_AUTO_TEST_CASE(contains_point_before_a)
{
	rove::line<2> l(rove::vec<2>(0, 0), rove::vec<2>(4, 0));
	BOOST_REQUIRE(!l.contains(rove::vec<2>(-1, 0)));
	BOOST_REQUIRE(!l.contains(rove::vec<2>(-5, 0)));
}

BOOST_AUTO_TEST_CASE(contains_point_off_line)
{
	rove::line<2> l(rove::vec<2>(0, 0), rove::vec<2>(4, 0));
	BOOST_REQUIRE(!l.contains(rove::vec<2>(2, 1)));
	BOOST_REQUIRE(!l.contains(rove::vec<2>(2, -1)));
}

BOOST_AUTO_TEST_CASE(contains_diagonal)
{
	rove::line<2> l(rove::vec<2>(0, 0), rove::vec<2>(4, 4));
	BOOST_REQUIRE(l.contains(rove::vec<2>(2, 2)));
	BOOST_REQUIRE(!l.contains(rove::vec<2>(6, 6)));
}

BOOST_AUTO_TEST_CASE(contains_3d_diagonal)
{
	rove::line<3> l(rove::vec<3>(0, 0, 0), rove::vec<3>(3, 3, 3));
	BOOST_REQUIRE(l.contains(rove::vec<3>(1, 1, 1)));
	BOOST_REQUIRE(!l.contains(rove::vec<3>(4, 4, 4)));
	// Off-axis point
	BOOST_REQUIRE(!l.contains(rove::vec<3>(1, 1, 2)));
}

// --- distance_sq() ---
//
// Segment from (0,0,0) to (4,0,0) for all tests unless noted.

BOOST_AUTO_TEST_CASE(distance_sq_point_on_segment)
{
	rove::line<3> l(rove::vec<3>(0, 0, 0), rove::vec<3>(4, 0, 0));
	BOOST_REQUIRE(rove::abs(l.distance_sq(rove::vec<3>(2, 0, 0))) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(distance_sq_point_perpendicular_to_interior)
{
	rove::line<3> l(rove::vec<3>(0, 0, 0), rove::vec<3>(4, 0, 0));
	// Above midpoint at height 3: closest on segment is (2,0,0), dist^2 = 9
	BOOST_REQUIRE(rove::abs(l.distance_sq(rove::vec<3>(2, 3, 0)) - 9.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(distance_sq_point_beyond_b)
{
	rove::line<3> l(rove::vec<3>(0, 0, 0), rove::vec<3>(4, 0, 0));
	// t > 1: clamped to B=(4,0,0). Point (5,0,0): dist^2 = 1
	BOOST_REQUIRE(rove::abs(l.distance_sq(rove::vec<3>(5, 0, 0)) - 1.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(distance_sq_point_before_a)
{
	rove::line<3> l(rove::vec<3>(0, 0, 0), rove::vec<3>(4, 0, 0));
	// t < 0: clamped to A=(0,0,0). Point (-1,0,0): dist^2 = 1
	BOOST_REQUIRE(rove::abs(l.distance_sq(rove::vec<3>(-1, 0, 0)) - 1.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(distance_sq_point_past_corner)
{
	rove::line<3> l(rove::vec<3>(0, 0, 0), rove::vec<3>(4, 0, 0));
	// t > 1: closest is B=(4,0,0). Point (5,3,0): dist^2 = 1+9 = 10
	BOOST_REQUIRE(rove::abs(l.distance_sq(rove::vec<3>(5, 3, 0)) - 10.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(distance_sq_point_before_corner)
{
	rove::line<3> l(rove::vec<3>(0, 0, 0), rove::vec<3>(4, 0, 0));
	// t < 0: closest is A=(0,0,0). Point (-1,2,0): dist^2 = 1+4 = 5
	BOOST_REQUIRE(rove::abs(l.distance_sq(rove::vec<3>(-1, 2, 0)) - 5.0f) < rove::EPSILON);
}

// --- distance() ---

BOOST_AUTO_TEST_CASE(distance_point_on_segment)
{
	rove::line<3> l(rove::vec<3>(0, 0, 0), rove::vec<3>(4, 0, 0));
	BOOST_REQUIRE(rove::abs(l.distance(rove::vec<3>(2, 0, 0))) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(distance_point_perpendicular)
{
	rove::line<3> l(rove::vec<3>(0, 0, 0), rove::vec<3>(4, 0, 0));
	BOOST_REQUIRE(rove::abs(l.distance(rove::vec<3>(2, 3, 0)) - 3.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(distance_point_beyond_b)
{
	rove::line<3> l(rove::vec<3>(0, 0, 0), rove::vec<3>(4, 0, 0));
	BOOST_REQUIRE(rove::abs(l.distance(rove::vec<3>(5, 0, 0)) - 1.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(distance_point_before_a)
{
	rove::line<3> l(rove::vec<3>(0, 0, 0), rove::vec<3>(4, 0, 0));
	BOOST_REQUIRE(rove::abs(l.distance(rove::vec<3>(-1, 0, 0)) - 1.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(distance_2d_vertical_segment)
{
	rove::line<2> l(rove::vec<2>(0, 0), rove::vec<2>(0, 10));
	// Point to the right of interior: dist = x distance
	BOOST_REQUIRE(rove::abs(l.distance(rove::vec<2>(5, 5)) - 5.0f) < rove::EPSILON);
	// Point above B: closest is B=(0,10), dist = sqrt(3^2+5^2) = sqrt(34)
	BOOST_REQUIRE(rove::abs(l.distance(rove::vec<2>(3, 15)) - rove::vec<2>(3, 5).length()) < rove::EPSILON);
	// Point below A: closest is A=(0,0), dist = sqrt(4^2+3^2) = 5
	BOOST_REQUIRE(rove::abs(l.distance(rove::vec<2>(4, -3)) - 5.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(distance_sq_equals_distance_squared)
{
	using rove::random;
	for (int i = 0; i < 200; i++)
	{
		rove::line<3> l(
			rove::vec<3>(random(-5, 5), random(-5, 5), random(-5, 5)),
			rove::vec<3>(random(-5, 5), random(-5, 5), random(-5, 5)));
		rove::vec<3> p(random(-10, 10), random(-10, 10), random(-10, 10));

		rove::scalar d = l.distance(p);
		rove::scalar d_sq = l.distance_sq(p);
		BOOST_REQUIRE(rove::abs(d * d - d_sq) < 0.001f);
	}
}

BOOST_AUTO_TEST_SUITE_END()
