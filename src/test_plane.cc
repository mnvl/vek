
#include <boost/test/unit_test.hpp>
#include "aabb.h"
#include "obb.h"
#include "plane.h"

BOOST_AUTO_TEST_SUITE(plane)

BOOST_AUTO_TEST_CASE(get_origin_1)
{
	vek::plane<> p1(vek::vec<3>(1, -3, 5), vek::vec<3>(-7, 11, -13));
	vek::plane<> p2(p1.get_origin(), p1.get_normal());

	BOOST_REQUIRE(abs(p1.A - p2.A) < vek::EPSILON);
	BOOST_REQUIRE(abs(p1.B - p2.B) < vek::EPSILON);
	BOOST_REQUIRE(abs(p1.C - p2.C) < vek::EPSILON);
	BOOST_REQUIRE(abs(p1.D - p2.D) < vek::EPSILON);
}

BOOST_AUTO_TEST_CASE(classify_point_1)
{
	vek::plane<> pl(vek::vec<3>(0, -1, 0), vek::vec<3>(1, 1, 1));

	BOOST_REQUIRE(pl.classify(vek::vec<3>(0, -2, 0)) == vek::plane<>::NEGATIVE);
	BOOST_REQUIRE(pl.classify(vek::vec<3>(0, 0, 0)) == vek::plane<>::POSITIVE);
	BOOST_REQUIRE(pl.classify(vek::vec<3>(0, -1, 0)) == vek::plane<>::INTERSECTS);
}

BOOST_AUTO_TEST_CASE(classify_point_2)
{
	vek::plane<> pl(vek::vec<3>(0, 0, 0), vek::vec<3>(1, -1, 1));

	BOOST_REQUIRE(pl.classify(vek::vec<3>(-2, -1.5, -2)) == vek::plane<>::NEGATIVE);
	BOOST_REQUIRE(pl.classify(vek::vec<3>(-1, -2, -1)) == vek::plane<>::INTERSECTS);
}

BOOST_AUTO_TEST_CASE(classify_aabb_1)
{
	vek::plane<> pl(vek::vec<3>(-1, -1, -1), vek::vec<3>(1, 1, 1));

	BOOST_REQUIRE(pl.classify(vek::aabb<3>(vek::vec<3>(-2, -2, -2), vek::vec<3>(-1, -1.5, -1))) == vek::plane<>::NEGATIVE);
	BOOST_REQUIRE(pl.classify(vek::aabb<3>(vek::vec<3>(-2, -2, -2), vek::vec<3>(-1, -1, -1))) == vek::plane<>::INTERSECTS);
	BOOST_REQUIRE(pl.classify(vek::aabb<3>(vek::vec<3>(0, 0, 0), vek::vec<3>(1, 1, 1))) == vek::plane<>::POSITIVE);
	BOOST_REQUIRE(pl.classify(vek::aabb<3>(vek::vec<3>(-1, -1, -1), vek::vec<3>(1, 1, 1))) == vek::plane<>::INTERSECTS);
}

BOOST_AUTO_TEST_CASE(classify_aabb_2)
{
	vek::plane<> pl(vek::vec<3>(-1, -1, -1), vek::vec<3>(-1, -1, -1));

	BOOST_REQUIRE(pl.classify(vek::aabb<3>(vek::vec<3>(-2, -2, -2), vek::vec<3>(-1, -1.5, -1))) == vek::plane<>::POSITIVE);
	BOOST_REQUIRE(pl.classify(vek::aabb<3>(vek::vec<3>(-2, -2, -2), vek::vec<3>(-1, -1, -1))) == vek::plane<>::INTERSECTS);
	BOOST_REQUIRE(pl.classify(vek::aabb<3>(vek::vec<3>(0, 0, 0), vek::vec<3>(1, 1, 1))) == vek::plane<>::NEGATIVE);
	BOOST_REQUIRE(pl.classify(vek::aabb<3>(vek::vec<3>(-1, -1, -1), vek::vec<3>(1, 1, 1))) == vek::plane<>::INTERSECTS);
}

BOOST_AUTO_TEST_CASE(classify_aabb_3)
{
	vek::plane<> pl(vek::vec<3>(0, 0, 0), vek::vec<3>(1, -1, 1));

	BOOST_REQUIRE(pl.classify(vek::aabb<3>(vek::vec<3>(0, 4, 0), vek::vec<3>(1, 5, 1))) == vek::plane<>::NEGATIVE);
	BOOST_REQUIRE(pl.classify(vek::aabb<3>(vek::vec<3>(-1, -5, -1), vek::vec<3>(0, -4, 0))) == vek::plane<>::POSITIVE);
	BOOST_REQUIRE(pl.classify(vek::aabb<3>(vek::vec<3>(-2, -2, -2), vek::vec<3>(0, 0, 0))) == vek::plane<>::INTERSECTS);
	BOOST_REQUIRE(pl.classify(vek::aabb<3>(vek::vec<3>(-1, -1, -1), vek::vec<3>(1, 1, 1))) == vek::plane<>::INTERSECTS);
}

BOOST_AUTO_TEST_CASE(classify_aabb_4)
{
	vek::plane<> pl(vek::vec<3>(0, 2, 0), vek::vec<3>(1, 1, 1));

	BOOST_REQUIRE(pl.classify(vek::aabb<3>(vek::vec<3>(-1, -1, -1), vek::vec<3>(0, 0, 0))) == vek::plane<>::NEGATIVE);
}

BOOST_AUTO_TEST_CASE(classify_obb_1)
{
	vek::plane<> pl(vek::vec<3>(0, 0, 0), vek::vec<3>(1, 1, 1));

	BOOST_REQUIRE(pl.classify(
			vek::obb<3>(vek::vec<3>(0, 1, 0), vek::vec<3>(1, 0, 0), vek::vec<3>(0, 1, 0), vek::vec<3>(0, 0, 1)))
		== vek::plane<>::POSITIVE);

	BOOST_REQUIRE(pl.classify(
			vek::obb<3>(vek::vec<3>(-1, -2, -1), vek::vec<3>(1, 0, 0), vek::vec<3>(0, 1, 0), vek::vec<3>(0, 0, 1)))
		== vek::plane<>::NEGATIVE);
}

BOOST_AUTO_TEST_CASE(classify_obb_2)
{
	vek::plane<> pl(vek::vec<3>(0, 0, 0), vek::vec<3>(1, 1, 1));

	BOOST_REQUIRE(pl.classify(
			vek::obb<3>(
				vek::vec<3>(-1, -1, -1),
				vek::vec<3>(11, 10, 10),
				vek::vec<3>(10, 11, 10),
				vek::vec<3>(10, 10, 11)))
		== vek::plane<>::INTERSECTS);
}

BOOST_AUTO_TEST_CASE(classify_obb_3)
{
	vek::plane<> pl;
	pl.A =  0.00000000f;
	pl.B = -0.26937401f;
	pl.C = -0.26937401f;
	pl.D =  1.00618340f;

	vek::obb<3> bound;
	bound.origin.set(-5.6245117f, 0.00000000f, 23.716305f);
	bound.tangent.set(2.0000000f, 0.00000000f, 0.00000000f);
	bound.normal.set(0.00000000f, 1.0000000f, 0.00000000f);
	bound.binormal.set(0.00000000f, 0.00000000f, 2.0000000f);

	BOOST_REQUIRE (pl.classify(bound.origin) == vek::plane<>::NEGATIVE);
	BOOST_REQUIRE (pl.classify(bound.origin + bound.tangent) == vek::plane<>::NEGATIVE);
	BOOST_REQUIRE (pl.classify(bound.origin + bound.normal) == vek::plane<>::NEGATIVE);
	BOOST_REQUIRE (pl.classify(bound.origin + bound.binormal) == vek::plane<>::NEGATIVE);

	BOOST_REQUIRE (pl.classify(bound) == vek::plane<>::NEGATIVE);
}

BOOST_AUTO_TEST_CASE(raytrace_1)
{
	vek::plane<> pl(vek::vec<3>(0, 0, 0), vek::vec<3>(0, 1, 0));

	vek::ray<3> r(vek::vec<3>(0, 1, 0), vek::vec<3>(0, -10, 0));

	vek::scalar t;
	vek::vec<3> v;

	pl.trace(r, t, v);

	BOOST_REQUIRE((v - vek::vec<3>(0, 0, 0)).length() < vek::EPSILON);
}

BOOST_AUTO_TEST_CASE(raytrace_2)
{
	vek::plane<> pl(vek::vec<3>(0, 0, 0), vek::vec<3>(0, 1, 0));

	vek::ray<3> r(vek::vec<3>(7, 1, 11), vek::vec<3>(0, -10, 0));

	vek::scalar t;
	vek::vec<3> v;

	pl.trace(r, t, v);

	BOOST_REQUIRE((v - vek::vec<3>(7, 0, 11)).length() < vek::EPSILON);
}

BOOST_AUTO_TEST_CASE(raytrace_3)
{
	vek::plane<> pl(vek::vec<3>(1, 3, 5), vek::vec<3>(0, 1, 0));

	vek::ray<3> r(vek::vec<3>(7 + 1, 1 + 3, 11 + 5), vek::vec<3>(0, -10, 0));

	vek::scalar t;
	vek::vec<3> v;

	pl.trace(r, t, v);

	BOOST_REQUIRE((v - vek::vec<3>(7 + 1, 0 + 3, 11 + 5)).length() < vek::EPSILON);
}

BOOST_AUTO_TEST_CASE(raytrace_4)
{
	vek::plane<> pl(vek::vec<3>(1, 3, 5), vek::vec<3>(1, 1, 1));

	vek::ray<3> r(vek::vec<3>(1, 10, 5), vek::vec<3>(0, -10, 0));

	vek::scalar t;
	vek::vec<3> v;

	pl.trace(r, t, v);

	BOOST_REQUIRE((v - vek::vec<3>(1, 3, 5)).length() < vek::EPSILON);
}

BOOST_AUTO_TEST_SUITE_END()

