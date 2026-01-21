
#include <boost/test/unit_test.hpp>
#include "triangle.h"

BOOST_AUTO_TEST_SUITE(test_triangle)

BOOST_AUTO_TEST_CASE(test_triangle_trace)
{
	rove::triangle<3> tri(rove::vec<3>(0, 1, 0), rove::vec<3>(0, 3, 1), rove::vec<3>(1, 3, 0));
	rove::scalar t;
	rove::vec<3> v;
	bool result = tri.trace(rove::ray<3>(rove::vec<3>(-1, 2, 0), rove::vec<3>(10, 0, 0)), t, v);
	BOOST_CHECK (result == true);
	BOOST_CHECK ((rove::vec<3>(0.5f, 2, 0) - v).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_SUITE_END()
