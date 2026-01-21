
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

BOOST_AUTO_TEST_SUITE_END()
