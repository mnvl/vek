
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

BOOST_AUTO_TEST_SUITE_END()

