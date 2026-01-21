
#include <boost/test/unit_test.hpp>
#include "vec.h"

BOOST_AUTO_TEST_SUITE(vec)

BOOST_AUTO_TEST_CASE(perpendiculars)
{
	for (int i = 0; i < 1000; i++)
	{
		rove::vec<3> v(rove::scalar(rand() * 2) / RAND_MAX - 1, rove::scalar(rand() * 2) / RAND_MAX - 1,
			rove::scalar(rand() * 2) / RAND_MAX - 1);

		rove::vec<3> xy = v.perpendicular_xy();
		rove::vec<3> yz = v.perpendicular_yz();
		rove::vec<3> xz = v.perpendicular_xz();

		BOOST_CHECK (rove::abs(xy & v) < rove::EPSILON);
		BOOST_CHECK (rove::abs(yz & v) < rove::EPSILON);
		BOOST_CHECK (rove::abs(xz & v) < rove::EPSILON);
	}
}

BOOST_AUTO_TEST_SUITE_END()
