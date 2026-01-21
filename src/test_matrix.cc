
#include <boost/test/unit_test.hpp>
#include "matrix.h"

namespace
{

template<int N> void
test_axis_angle_i(rove::vec<3> const &src, rove::vec<3> const &axis, rove::scalar angle, rove::vec<3> const &res)
{
	rove::matrix<N,N> m;
	m.rotation(axis, angle);

	rove::vec<3> result = src * m;

	BOOST_REQUIRE ((result.length() - src.length()) < rove::EPSILON);
	BOOST_REQUIRE ((res - result).length() < rove::EPSILON);
}

void test_axis_angle(rove::vec<3> const &src, rove::vec<3> const &axis, rove::scalar angle, rove::vec<3> const &res)
{
	test_axis_angle_i<3>(src, axis, angle, res);
	test_axis_angle_i<4>(src, axis, angle, res);

	test_axis_angle_i<3>(res, axis, -angle, src);
	test_axis_angle_i<4>(res, axis, -angle, src);
}

template<int N> void
test_rotation_i(rove::vec<3> const &src, rove::vec<3> const &angles, rove::vec<3> const &res)
{
	rove::matrix<N,N> m;
	m.rotation(angles);

	rove::vec<3> result = src * m;

	BOOST_REQUIRE ((result.length() - src.length()) < rove::EPSILON);
	BOOST_REQUIRE ((res - result).length() < rove::EPSILON);
}

void test_rotation(rove::vec<3> const &src, rove::vec<3> const &angles, rove::vec<3> const &res)
{
	test_rotation_i<3>(src, angles, res);
	test_rotation_i<4>(src, angles, res);

	test_rotation_i<3>(res, -angles, src);
	test_rotation_i<4>(res, -angles, src);
}

}

BOOST_AUTO_TEST_SUITE (test_matrix)

BOOST_AUTO_TEST_CASE (test_ijk)
{
	rove::vec<3> const i(1, 0, 0), j(0, 1, 0), k(0, 0, 1);

	test_axis_angle(i, j, -rove::PI / 2, k);
	test_rotation(i, -rove::PI / 2 * j, k);

	test_axis_angle(j, k, -rove::PI / 2, i);
	test_rotation(j, -rove::PI / 2 * k, i);

	test_axis_angle(k, i, -rove::PI / 2, j);
	test_rotation(k, -rove::PI / 2 * i, j);
}

BOOST_AUTO_TEST_CASE (test_pi4)
{
	{
		rove::vec<3> src(1, 0, 0);
		rove::vec<3> res = rove::normalize(rove::vec<3>(1, -1, 0));

		test_rotation(src, rove::vec<3>(0, 0, -rove::PI / 4), res);
		test_axis_angle(src, rove::vec<3>(0, 0, 1), -rove::PI / 4, res);
	}

	{
		rove::vec<3> src(0, 1, 0);
		rove::vec<3> res = rove::normalize(rove::vec<3>(1, 1, 0));

		test_rotation(src, rove::vec<3>(0, 0, -rove::PI / 4), res);
		test_axis_angle(src, rove::vec<3>(0, 0, 1), -rove::PI / 4, res);
	}
}

BOOST_AUTO_TEST_CASE (test_pi4_number_2)
{
	{
		rove::vec<3> src(1, 0, 0);
		rove::vec<3> res = rove::normalize(rove::vec<3>(1, 1, 0));

		test_rotation(src, rove::vec<3>(0, 0, rove::PI / 4), res);
		test_axis_angle(src, rove::vec<3>(0, 0, 1), rove::PI / 4, res);
	}

	{
		rove::vec<3> src(0, 1, 0);
		rove::vec<3> res = rove::normalize(rove::vec<3>(-1, 1, 0));

		test_rotation(src, rove::vec<3>(0, 0, rove::PI / 4), res);
		test_axis_angle(src, rove::vec<3>(0, 0, 1), rove::PI / 4, res);
	}
}

BOOST_AUTO_TEST_CASE (test_matrix_inverse_4x4_1)
{
	rove::matrix<4,4> M, I, R, U;

	M.scaling(rove::scalar(rand() + 1), rove::scalar(rand() + 1), rove::scalar(rand() + 1));

	M.inverse(I);

	R = M * I;
	U.identity();

	BOOST_REQUIRE(rove::equal(R, U));
}

BOOST_AUTO_TEST_CASE (test_matrix_inverse_4x4_2)
{
	rove::matrix<4,4> M, I, R, U;

	M.rotation(rand() * rove::PI / RAND_MAX , rand() * rove::PI / RAND_MAX, rand() * rove::PI / RAND_MAX);

	M.inverse(I);

	R = M * I;
	U.identity();

	BOOST_REQUIRE(rove::equal(R, U));
}


BOOST_AUTO_TEST_CASE (test_matrix_inverse_4x4_3)
{
	rove::matrix<4,4> M, I, R, U;

	M.translation(rand() * 10.0f / RAND_MAX, rand() * 10.0f / RAND_MAX, rand() * 10.0f / RAND_MAX);

	M.inverse(I);

	R = M * I;
	U.identity();

	BOOST_REQUIRE(rove::equal(R, U));
}

BOOST_AUTO_TEST_CASE (test_matrix_inverse_4x4_4)
{
	rove::matrix<4,4> M, I, R, U;

	M.translation(rand() * 10.0f / RAND_MAX, rand() * 10.0f / RAND_MAX, rand() * 10.0f / RAND_MAX);
	M.rotate(rand() * rove::PI / RAND_MAX , rand() * rove::PI / RAND_MAX, rand() * rove::PI / RAND_MAX);

	M.inverse(I);

	R = M * I;
	U.identity();

	BOOST_REQUIRE(rove::equal(R, U));
}

BOOST_AUTO_TEST_SUITE_END()
