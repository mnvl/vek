
#include <boost/test/unit_test.hpp>
#include "matrix.h"

namespace
{

template<int N> void
test_axis_angle_i(vek::vec<3> const &src, vek::vec<3> const &axis, vek::scalar angle, vek::vec<3> const &res)
{
	vek::matrix<N,N> m;
	m.rotation(axis, angle);

	vek::vec<3> result = src * m;

	BOOST_REQUIRE ((result.length() - src.length()) < vek::EPSILON);
	BOOST_REQUIRE ((res - result).length() < vek::EPSILON);
}

void test_axis_angle(vek::vec<3> const &src, vek::vec<3> const &axis, vek::scalar angle, vek::vec<3> const &res)
{
	test_axis_angle_i<3>(src, axis, angle, res);
	test_axis_angle_i<4>(src, axis, angle, res);

	test_axis_angle_i<3>(res, axis, -angle, src);
	test_axis_angle_i<4>(res, axis, -angle, src);
}

template<int N> void
test_rotation_i(vek::vec<3> const &src, vek::vec<3> const &angles, vek::vec<3> const &res)
{
	vek::matrix<N,N> m;
	m.rotation(angles);

	vek::vec<3> result = src * m;

	BOOST_REQUIRE ((result.length() - src.length()) < vek::EPSILON);
	BOOST_REQUIRE ((res - result).length() < vek::EPSILON);
}

void test_rotation(vek::vec<3> const &src, vek::vec<3> const &angles, vek::vec<3> const &res)
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
	vek::vec<3> const i(1, 0, 0), j(0, 1, 0), k(0, 0, 1);

	test_axis_angle(i, j, -vek::PI / 2, k);
	test_rotation(i, -vek::PI / 2 * j, k);

	test_axis_angle(j, k, -vek::PI / 2, i);
	test_rotation(j, -vek::PI / 2 * k, i);

	test_axis_angle(k, i, -vek::PI / 2, j);
	test_rotation(k, -vek::PI / 2 * i, j);
}

BOOST_AUTO_TEST_CASE (test_pi4)
{
	{
		vek::vec<3> src(1, 0, 0);
		vek::vec<3> res = vek::normalize(vek::vec<3>(1, -1, 0));

		test_rotation(src, vek::vec<3>(0, 0, -vek::PI / 4), res);
		test_axis_angle(src, vek::vec<3>(0, 0, 1), -vek::PI / 4, res);
	}

	{
		vek::vec<3> src(0, 1, 0);
		vek::vec<3> res = vek::normalize(vek::vec<3>(1, 1, 0));

		test_rotation(src, vek::vec<3>(0, 0, -vek::PI / 4), res);
		test_axis_angle(src, vek::vec<3>(0, 0, 1), -vek::PI / 4, res);
	}
}

BOOST_AUTO_TEST_CASE (test_pi4_number_2)
{
	{
		vek::vec<3> src(1, 0, 0);
		vek::vec<3> res = vek::normalize(vek::vec<3>(1, 1, 0));

		test_rotation(src, vek::vec<3>(0, 0, vek::PI / 4), res);
		test_axis_angle(src, vek::vec<3>(0, 0, 1), vek::PI / 4, res);
	}

	{
		vek::vec<3> src(0, 1, 0);
		vek::vec<3> res = vek::normalize(vek::vec<3>(-1, 1, 0));

		test_rotation(src, vek::vec<3>(0, 0, vek::PI / 4), res);
		test_axis_angle(src, vek::vec<3>(0, 0, 1), vek::PI / 4, res);
	}
}

BOOST_AUTO_TEST_CASE (test_matrix_inverse_4x4_1)
{
	vek::matrix<4,4> M, I, R, U;

	M.scaling(vek::scalar(rand() + 1), vek::scalar(rand() + 1), vek::scalar(rand() + 1));

	M.inverse(I);

	R = M * I;
	U.identity();

	BOOST_REQUIRE(vek::equal(R, U));
}

BOOST_AUTO_TEST_CASE (test_matrix_inverse_4x4_2)
{
	vek::matrix<4,4> M, I, R, U;

	M.rotation(rand() * vek::PI / RAND_MAX , rand() * vek::PI / RAND_MAX, rand() * vek::PI / RAND_MAX);

	M.inverse(I);

	R = M * I;
	U.identity();

	BOOST_REQUIRE(vek::equal(R, U));
}


BOOST_AUTO_TEST_CASE (test_matrix_inverse_4x4_3)
{
	vek::matrix<4,4> M, I, R, U;

	M.translation(rand() * 10.0f / RAND_MAX, rand() * 10.0f / RAND_MAX, rand() * 10.0f / RAND_MAX);

	M.inverse(I);

	R = M * I;
	U.identity();

	BOOST_REQUIRE(vek::equal(R, U));
}

BOOST_AUTO_TEST_CASE (test_matrix_inverse_4x4_4)
{
	vek::matrix<4,4> M, I, R, U;

	M.translation(rand() * 10.0f / RAND_MAX, rand() * 10.0f / RAND_MAX, rand() * 10.0f / RAND_MAX);
	M.rotate(rand() * vek::PI / RAND_MAX , rand() * vek::PI / RAND_MAX, rand() * vek::PI / RAND_MAX);

	M.inverse(I);

	R = M * I;
	U.identity();

	BOOST_REQUIRE(vek::equal(R, U));
}

BOOST_AUTO_TEST_SUITE_END()
