
#include <boost/test/unit_test.hpp>
#include "ray.h"
#include "aabb.h"

BOOST_AUTO_TEST_SUITE(aabb)

BOOST_AUTO_TEST_CASE(trace_through_corners)
{
	vek::aabb<3> b(vek::vec<3>(0, 0, 0), 20.0f);
	vek::ray<3> r(vek::vec<3>(-20, -20, -20), vek::vec<3>(40, 40, 40));

	vek::scalar t0, t1;

	b.trace(r, &t0, &t1);

	BOOST_REQUIRE (vek::abs(t0 - 0.25f) < vek::EPSILON);
	BOOST_REQUIRE (vek::abs(t1 - 0.75f) < vek::EPSILON);
}

BOOST_AUTO_TEST_SUITE_END()

