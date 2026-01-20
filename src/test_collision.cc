
#include <boost/test/unit_test.hpp>
#include "collision.h"

BOOST_AUTO_TEST_SUITE (test_collision)

BOOST_AUTO_TEST_CASE (test_sphere_line_collision_1)
{
	vek::sphere<2> sph;
	sph.centre.set(10, 10);
	sph.radius = 1;

	vek::line<2> l;
	l.A.set(-100, 0);
	l.B.set(100, 0);

	vek::contact_info<2> ci;
	vek::collide(l, sph, vek::vec<2>(-2, -1), ci);

	BOOST_REQUIRE (ci.happened == true);
	BOOST_REQUIRE (vek::abs(ci.time - 9.0f) < vek::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_sphere_line_collision_2)
{
	vek::sphere<2> sph;
	sph.centre.set(10, 0.5f);
	sph.radius = 1;

	vek::line<2> l;
	l.A.set(0, 0);
	l.B.set(0, 1);

	vek::contact_info<2> ci;
	vek::collide(l, sph, vek::vec<2>(-1, 0), ci);

	BOOST_REQUIRE(ci.happened == true);
	BOOST_REQUIRE(vek::abs(ci.time - 9.0f) < vek::EPSILON);
	BOOST_REQUIRE((ci.position - vek::vec<2>(0, 0.5f)).length_sq() < vek::EPSILON);
	BOOST_REQUIRE((ci.normal - vek::vec<2>(1, 0)).length_sq() < vek::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_sphere_line_collision_3)
{
	vek::line<2> l;
	l.A.set(1, -10);
	l.B.set(1, 10);

	vek::sphere<2> s;
	s.centre.set(10, 10);
	s.radius = 1;

	vek::contact_info<2> ci;
	vek::collide(l, s, vek::vec<2>(-1, -1), ci);

	BOOST_REQUIRE ((ci.position - vek::vec<2>(1, 2)).length() < vek::EPSILON);
	BOOST_REQUIRE ((ci.time - 8) < vek::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_sphere_obb_collision_1)
{
	vek::obb<2> obb;
	obb.origin.set(0, 0);
	obb.tangent.set(1, 0);
	obb.normal.set(0, 1);

	vek::sphere<2> sph;
	sph.centre.set(0.5, 10);
	sph.radius = 1;

	vek::contact_info<2> ci;
	vek::collide(obb, sph, vek::vec<2>(0, -1), ci);

	BOOST_REQUIRE ((ci.position - vek::vec<2>(0.5, 1)).length() < vek::EPSILON);
	BOOST_REQUIRE ((ci.normal - vek::vec<2>(0, 1)).length() < vek::EPSILON);
	BOOST_REQUIRE (vek::abs(ci.time - 8) < vek::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_sphere_obb_collision_2)
{
	vek::obb<2> obb;
	obb.origin.set(0, 0);
	obb.tangent.set(1, 0);
	obb.normal.set(0, 1);

	vek::sphere<2> sph;
	sph.centre.set(10, 0.5f);
	sph.radius = 1;

	vek::contact_info<2> ci;

	vek::collide(obb, sph, vek::vec<2>(-1.0f, -0.01f), ci);

	BOOST_REQUIRE ((ci.position - vek::vec<2>(1, 0.42f)).length() < vek::EPSILON);
	BOOST_REQUIRE ((ci.normal - vek::vec<2>(1, 0)).length() < vek::EPSILON);
	BOOST_REQUIRE ((ci.time - 8) < vek::EPSILON);
}

BOOST_AUTO_TEST_SUITE_END()
