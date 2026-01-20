
#include <boost/test/unit_test.hpp>
#include "vec.h"

BOOST_AUTO_TEST_SUITE(vec)

BOOST_AUTO_TEST_CASE(perpendiculars)
{
	for (int i = 0; i < 1000; i++)
	{
		vek::vec<3> v(vek::scalar(rand() * 2) / RAND_MAX - 1, vek::scalar(rand() * 2) / RAND_MAX - 1,
			vek::scalar(rand() * 2) / RAND_MAX - 1);

		vek::vec<3> xy = v.perpendicular_xy();
		vek::vec<3> yz = v.perpendicular_yz();
		vek::vec<3> xz = v.perpendicular_xz();

		BOOST_CHECK (vek::abs(xy & v) < vek::EPSILON);
		BOOST_CHECK (vek::abs(yz & v) < vek::EPSILON);
		BOOST_CHECK (vek::abs(xz & v) < vek::EPSILON);
	}
}

BOOST_AUTO_TEST_SUITE_END()
