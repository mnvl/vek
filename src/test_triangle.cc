
#include <boost/test/unit_test.hpp>
#include "triangle.h"

BOOST_AUTO_TEST_SUITE(test_triangle)

BOOST_AUTO_TEST_CASE(test_triangle_trace)
{
	vek::triangle<3> tri(vek::vec<3>(0, 1, 0), vek::vec<3>(0, 3, 1), vek::vec<3>(1, 3, 0));
	vek::scalar t;
	vek::vec<3> v;
	bool result = tri.trace(vek::ray<3>(vek::vec<3>(-1, 2, 0), vek::vec<3>(10, 0, 0)), t, v);
	BOOST_CHECK (result == true);
	BOOST_CHECK ((vek::vec<3>(0.5f, 2, 0) - v).length() < vek::EPSILON);
}

BOOST_AUTO_TEST_SUITE_END()
