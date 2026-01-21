
#include <boost/test/unit_test.hpp>
#include "ray.h"
#include "aabb.h"

BOOST_AUTO_TEST_SUITE(aabb)

BOOST_AUTO_TEST_CASE(trace_through_corners)
{
	rove::aabb<3> b(rove::vec<3>(0, 0, 0), 20.0f);
	rove::ray<3> r(rove::vec<3>(-20, -20, -20), rove::vec<3>(40, 40, 40));

	rove::scalar t0, t1;

	b.trace(r, &t0, &t1);

	BOOST_REQUIRE (rove::abs(t0 - 0.25f) < rove::EPSILON);
	BOOST_REQUIRE (rove::abs(t1 - 0.75f) < rove::EPSILON);
}

BOOST_AUTO_TEST_SUITE_END()

