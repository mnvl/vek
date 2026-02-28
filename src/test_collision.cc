
#include <boost/test/unit_test.hpp>
#include "collision.h"

BOOST_AUTO_TEST_SUITE (test_collision)

BOOST_AUTO_TEST_CASE (test_sphere_line_collision_1)
{
	rove::sphere<2> sph;
	sph.centre.set(10, 10);
	sph.radius = 1;

	rove::line<2> l;
	l.A.set(-100, 0);
	l.B.set(100, 0);

	rove::contact_info<2> ci;
	rove::collide(l, sph, rove::vec<2>(-2, -1), ci);

	BOOST_REQUIRE (ci.happened == true);
	BOOST_REQUIRE (rove::abs(ci.time - 9.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_sphere_line_collision_2)
{
	rove::sphere<2> sph;
	sph.centre.set(10, 0.5f);
	sph.radius = 1;

	rove::line<2> l;
	l.A.set(0, 0);
	l.B.set(0, 1);

	rove::contact_info<2> ci;
	rove::collide(l, sph, rove::vec<2>(-1, 0), ci);

	BOOST_REQUIRE(ci.happened == true);
	BOOST_REQUIRE(rove::abs(ci.time - 9.0f) < rove::EPSILON);
	BOOST_REQUIRE((ci.position - rove::vec<2>(0, 0.5f)).length_sq() < rove::EPSILON);
	BOOST_REQUIRE((ci.normal - rove::vec<2>(1, 0)).length_sq() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_sphere_line_collision_3)
{
	rove::line<2> l;
	l.A.set(1, -10);
	l.B.set(1, 10);

	rove::sphere<2> s;
	s.centre.set(10, 10);
	s.radius = 1;

	rove::contact_info<2> ci;
	rove::collide(l, s, rove::vec<2>(-1, -1), ci);

	BOOST_REQUIRE ((ci.position - rove::vec<2>(1, 2)).length() < rove::EPSILON);
	BOOST_REQUIRE ((ci.time - 8) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_sphere_obb_collision_1)
{
	rove::obb<2> obb;
	obb.origin.set(0, 0);
	obb.tangent.set(1, 0);
	obb.normal.set(0, 1);

	rove::sphere<2> sph;
	sph.centre.set(0.5, 10);
	sph.radius = 1;

	rove::contact_info<2> ci;
	rove::collide(obb, sph, rove::vec<2>(0, -1), ci);

	BOOST_REQUIRE ((ci.position - rove::vec<2>(0.5, 1)).length() < rove::EPSILON);
	BOOST_REQUIRE ((ci.normal - rove::vec<2>(0, 1)).length() < rove::EPSILON);
	BOOST_REQUIRE (rove::abs(ci.time - 8) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_sphere_obb_collision_2)
{
	rove::obb<2> obb;
	obb.origin.set(0, 0);
	obb.tangent.set(1, 0);
	obb.normal.set(0, 1);

	rove::sphere<2> sph;
	sph.centre.set(10, 0.5f);
	sph.radius = 1;

	rove::contact_info<2> ci;

	rove::collide(obb, sph, rove::vec<2>(-1.0f, -0.01f), ci);

	BOOST_REQUIRE ((ci.position - rove::vec<2>(1, 0.42f)).length() < rove::EPSILON);
	BOOST_REQUIRE ((ci.normal - rove::vec<2>(1, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE ((ci.time - 8) < rove::EPSILON);
}

// --- Sphere-Sphere collision ---
//
// collide(s1, s2, vel, ci): vel is the velocity of s2. s1 is stationary.
// Contact info is contact_info<2, S> regardless of sphere dimension.

BOOST_AUTO_TEST_CASE(test_sphere_sphere_no_collision_moving_away)
{
	// s2 starts to the right and moves further right
	rove::sphere<2> s1, s2;
	s1.centre.set(0, 0);  s1.radius = 1;
	s2.centre.set(10, 0); s2.radius = 1;

	rove::contact_info<2> ci;
	rove::collide(s1, s2, rove::vec<2>(1, 0), ci);

	BOOST_REQUIRE(ci.happened == false);
}

BOOST_AUTO_TEST_CASE(test_sphere_sphere_no_collision_perpendicular_miss)
{
	// s2 moves perpendicular to the line between centers — misses
	rove::sphere<2> s1, s2;
	s1.centre.set(0, 0);  s1.radius = 1;
	s2.centre.set(5, 0);  s2.radius = 1;

	rove::contact_info<2> ci;
	rove::collide(s1, s2, rove::vec<2>(0, 1), ci);

	BOOST_REQUIRE(ci.happened == false);
}

BOOST_AUTO_TEST_CASE(test_sphere_sphere_collision_head_on)
{
	// s2 at (-5,0) moving right toward s1 at origin. Radii=1 each.
	// Contact when |s1 - (s2 + vel*t)| = r1+r2=2
	// |(5,0) - (1,0)*t| = 2 → |5-t| = 2 → t = 3 (first contact)
	rove::sphere<2> s1, s2;
	s1.centre.set(0, 0);  s1.radius = 1;
	s2.centre.set(-5, 0); s2.radius = 1;

	rove::contact_info<2> ci;
	rove::collide(s1, s2, rove::vec<2>(1, 0), ci);

	BOOST_REQUIRE(ci.happened == true);
	BOOST_REQUIRE(rove::abs(ci.time - 3.0f) < rove::EPSILON);
	// s2 at contact: (-5,0) + (1,0)*3 = (-2,0)
	BOOST_REQUIRE((ci.position - rove::vec<2>(-2, 0)).length() < rove::EPSILON);
	// Normal from s1 to contact point: normalize((-2,0)-(0,0)) = (-1,0)
	BOOST_REQUIRE((ci.normal - rove::vec<2>(-1, 0)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(test_sphere_sphere_collision_at_angle)
{
	// s2 at (0, -5) moving upward toward s1 at origin. Radii=1 each.
	// t = 3 by symmetry with the above test
	rove::sphere<2> s1, s2;
	s1.centre.set(0, 0);  s1.radius = 1;
	s2.centre.set(0, -5); s2.radius = 1;

	rove::contact_info<2> ci;
	rove::collide(s1, s2, rove::vec<2>(0, 1), ci);

	BOOST_REQUIRE(ci.happened == true);
	BOOST_REQUIRE(rove::abs(ci.time - 3.0f) < rove::EPSILON);
	BOOST_REQUIRE((ci.position - rove::vec<2>(0, -2)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(test_sphere_sphere_collision_different_radii)
{
	// s1 radius=2, s2 radius=3, combined=5
	// s2 at (-10,0) moving right. Contact when dist = 5: |10-t|=5 → t=5
	rove::sphere<2> s1, s2;
	s1.centre.set(0, 0);  s1.radius = 2;
	s2.centre.set(-10, 0); s2.radius = 3;

	rove::contact_info<2> ci;
	rove::collide(s1, s2, rove::vec<2>(1, 0), ci);

	BOOST_REQUIRE(ci.happened == true);
	BOOST_REQUIRE(rove::abs(ci.time - 5.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(test_sphere_sphere_penetration)
{
	// Spheres already overlapping: s1 at (0,0) r=2, s2 at (2,0) r=2
	// dist=2, R=4, penetration_depth = 4-2 = 2 > EPSILON
	rove::sphere<2> s1, s2;
	s1.centre.set(0, 0); s1.radius = 2;
	s2.centre.set(2, 0); s2.radius = 2;

	rove::contact_info<2> ci;
	rove::collide(s1, s2, rove::vec<2>(0.1f, 0), ci);

	BOOST_REQUIRE(ci.happened == true);
	BOOST_REQUIRE(ci.penetrated == true);
	BOOST_REQUIRE(ci.time == 0);
	BOOST_REQUIRE(rove::abs(ci.penetration_depth - 2.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(test_sphere_sphere_no_collision_parallel)
{
	// s2 moves parallel to the axis between centers, but misses
	rove::sphere<2> s1, s2;
	s1.centre.set(0, 0);  s1.radius = 1;
	s2.centre.set(5, 5);  s2.radius = 1;

	rove::contact_info<2> ci;
	rove::collide(s1, s2, rove::vec<2>(-1, 0), ci);

	// Closest approach is at x=0 (between centers in x), y-distance remains 5 > r1+r2=2
	BOOST_REQUIRE(ci.happened == false);
}

BOOST_AUTO_TEST_SUITE_END()
