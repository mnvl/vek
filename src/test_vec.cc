
#include <boost/test/unit_test.hpp>
#include "vec.h"

BOOST_AUTO_TEST_SUITE(vec)

// 2D Vector Construction Tests
BOOST_AUTO_TEST_CASE(vec2_default_constructor)
{
	rove::vec<2> v;
	BOOST_REQUIRE(rove::abs(v.x) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.y) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec2_component_constructor)
{
	rove::vec<2> v(3.0f, 4.0f);
	BOOST_REQUIRE(rove::abs(v.x - 3.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.y - 4.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec2_copy_constructor)
{
	rove::vec<2> v1(5.0f, 6.0f);
	rove::vec<2> v2(v1);
	BOOST_REQUIRE(rove::abs(v2.x - 5.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v2.y - 6.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec2_component_access_aliases)
{
	rove::vec<2> v;
	v.x = 1.0f;
	v.y = 2.0f;

	// Test all naming conventions access the same memory
	BOOST_REQUIRE(rove::abs(v.u - 1.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.v - 2.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.s - 1.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.t - 2.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.A - 1.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.B - 2.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.i[0] - 1.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.i[1] - 2.0f) < rove::EPSILON);
}

// 3D Vector Construction Tests
BOOST_AUTO_TEST_CASE(vec3_default_constructor)
{
	rove::vec<3> v;
	BOOST_REQUIRE(rove::abs(v.x) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.y) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.z) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec3_component_constructor)
{
	rove::vec<3> v(1.0f, 2.0f, 3.0f);
	BOOST_REQUIRE(rove::abs(v.x - 1.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.y - 2.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.z - 3.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec3_component_access_aliases)
{
	rove::vec<3> v(1.0f, 2.0f, 3.0f);

	// Test all naming conventions
	BOOST_REQUIRE(rove::abs(v.r - 1.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.g - 2.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.b - 3.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.A - 1.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.B - 2.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.C - 3.0f) < rove::EPSILON);
}

// 4D Vector Construction Tests
BOOST_AUTO_TEST_CASE(vec4_component_constructor)
{
	rove::vec<4> v(1.0f, 2.0f, 3.0f, 4.0f);
	BOOST_REQUIRE(rove::abs(v.x - 1.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.y - 2.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.z - 3.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.w - 4.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec4_component_access_aliases)
{
	rove::vec<4> v(1.0f, 2.0f, 3.0f, 4.0f);

	BOOST_REQUIRE(rove::abs(v.r - 1.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.g - 2.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.b - 3.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.a - 4.0f) < rove::EPSILON);
}

// Set methods tests
BOOST_AUTO_TEST_CASE(vec2_set)
{
	rove::vec<2> v;
	v.set(10.0f, 20.0f);
	BOOST_REQUIRE(rove::abs(v.x - 10.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.y - 20.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec3_set)
{
	rove::vec<3> v;
	v.set(10.0f, 20.0f, 30.0f);
	BOOST_REQUIRE(rove::abs(v.x - 10.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.y - 20.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.z - 30.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec_set_all)
{
	rove::vec<3> v;
	v.set_all(5.0f);
	BOOST_REQUIRE(rove::abs(v.x - 5.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.y - 5.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.z - 5.0f) < rove::EPSILON);
}

// Arithmetic operations
BOOST_AUTO_TEST_CASE(vec_addition)
{
	rove::vec<3> v1(1.0f, 2.0f, 3.0f);
	rove::vec<3> v2(4.0f, 5.0f, 6.0f);
	rove::vec<3> v3 = v1 + v2;

	BOOST_REQUIRE(rove::abs(v3.x - 5.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v3.y - 7.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v3.z - 9.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec_subtraction)
{
	rove::vec<3> v1(10.0f, 20.0f, 30.0f);
	rove::vec<3> v2(1.0f, 2.0f, 3.0f);
	rove::vec<3> v3 = v1 - v2;

	BOOST_REQUIRE(rove::abs(v3.x - 9.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v3.y - 18.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v3.z - 27.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec_scalar_multiplication)
{
	rove::vec<3> v1(2.0f, 3.0f, 4.0f);
	rove::vec<3> v2 = v1 * 3.0f;

	BOOST_REQUIRE(rove::abs(v2.x - 6.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v2.y - 9.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v2.z - 12.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec_scalar_division)
{
	rove::vec<3> v1(10.0f, 20.0f, 30.0f);
	rove::vec<3> v2 = v1 / 2.0f;

	BOOST_REQUIRE(rove::abs(v2.x - 5.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v2.y - 10.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v2.z - 15.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec_negation)
{
	rove::vec<3> v1(1.0f, -2.0f, 3.0f);
	rove::vec<3> v2 = -v1;

	BOOST_REQUIRE(rove::abs(v2.x + 1.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v2.y - 2.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v2.z + 3.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec_component_wise_multiplication)
{
	rove::vec<3> v1(2.0f, 3.0f, 4.0f);
	rove::vec<3> v2(5.0f, 6.0f, 7.0f);
	rove::vec<3> v3 = v1 * v2;

	BOOST_REQUIRE(rove::abs(v3.x - 10.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v3.y - 18.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v3.z - 28.0f) < rove::EPSILON);
}

// Compound assignment operators
BOOST_AUTO_TEST_CASE(vec_addition_assignment)
{
	rove::vec<3> v1(1.0f, 2.0f, 3.0f);
	v1 += rove::vec<3>(4.0f, 5.0f, 6.0f);

	BOOST_REQUIRE(rove::abs(v1.x - 5.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v1.y - 7.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v1.z - 9.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec_scalar_multiplication_assignment)
{
	rove::vec<3> v(2.0f, 3.0f, 4.0f);
	v *= 3.0f;

	BOOST_REQUIRE(rove::abs(v.x - 6.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.y - 9.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.z - 12.0f) < rove::EPSILON);
}

// Comparison operators
BOOST_AUTO_TEST_CASE(vec_equality)
{
	rove::vec<3> v1(1.0f, 2.0f, 3.0f);
	rove::vec<3> v2(1.0f, 2.0f, 3.0f);
	rove::vec<3> v3(1.0f, 2.0f, 4.0f);

	BOOST_REQUIRE(v1 == v2);
	BOOST_REQUIRE(!(v1 == v3));
}

BOOST_AUTO_TEST_CASE(vec_inequality)
{
	rove::vec<3> v1(1.0f, 2.0f, 3.0f);
	rove::vec<3> v2(1.0f, 2.0f, 4.0f);

	BOOST_REQUIRE(v1 != v2);
}

BOOST_AUTO_TEST_CASE(vec_less_than)
{
	rove::vec<3> v1(1.0f, 2.0f, 3.0f);
	rove::vec<3> v2(2.0f, 3.0f, 4.0f);

	BOOST_REQUIRE(v1 < v2);
	BOOST_REQUIRE(!(v2 < v1));
}

BOOST_AUTO_TEST_CASE(vec_greater_than)
{
	rove::vec<3> v1(2.0f, 3.0f, 4.0f);
	rove::vec<3> v2(1.0f, 2.0f, 3.0f);

	BOOST_REQUIRE(v1 > v2);
	BOOST_REQUIRE(!(v2 > v1));
}

// Length and normalization tests
BOOST_AUTO_TEST_CASE(vec_length_sq)
{
	rove::vec<3> v(3.0f, 4.0f, 0.0f);
	rove::scalar len_sq = v.length_sq();

	BOOST_REQUIRE(rove::abs(len_sq - 25.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec_length)
{
	rove::vec<3> v(3.0f, 4.0f, 0.0f);
	rove::scalar len = v.length();

	BOOST_REQUIRE(rove::abs(len - 5.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec_normalize)
{
	rove::vec<3> v(3.0f, 4.0f, 0.0f);
	v.normalize();

	rove::scalar len = v.length();
	BOOST_REQUIRE(rove::abs(len - 1.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.x - 0.6f) < 0.01f);
	BOOST_REQUIRE(rove::abs(v.y - 0.8f) < 0.01f);
}

BOOST_AUTO_TEST_CASE(vec_normalize_function)
{
	rove::vec<3> v1(3.0f, 4.0f, 0.0f);
	rove::vec<3> v2 = rove::normalize(v1);

	BOOST_REQUIRE(rove::abs(v2.length() - 1.0f) < rove::EPSILON);
	// Original should be unchanged
	BOOST_REQUIRE(rove::abs(v1.length() - 5.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec_negate_method)
{
	rove::vec<3> v(1.0f, -2.0f, 3.0f);
	v.negate();

	BOOST_REQUIRE(rove::abs(v.x + 1.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.y - 2.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v.z + 3.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec2_manhattan_length)
{
	rove::vec<2> v(3.0f, 4.0f);
	rove::scalar len = v.length_manhattan();

	BOOST_REQUIRE(rove::abs(len - 7.0f) < rove::EPSILON);
}

// Dot product tests
BOOST_AUTO_TEST_CASE(vec_dot_product_operator)
{
	rove::vec<3> v1(1.0f, 2.0f, 3.0f);
	rove::vec<3> v2(4.0f, 5.0f, 6.0f);
	rove::scalar dot = v1 & v2;

	// 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
	BOOST_REQUIRE(rove::abs(dot - 32.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec_dot_product_function)
{
	rove::vec<3> v1(1.0f, 0.0f, 0.0f);
	rove::vec<3> v2(0.0f, 1.0f, 0.0f);
	rove::scalar dot = rove::dot_product(v1, v2);

	// Orthogonal vectors
	BOOST_REQUIRE(rove::abs(dot) < rove::EPSILON);
}

// Cross product tests
BOOST_AUTO_TEST_CASE(vec_cross_product_operator)
{
	rove::vec<3> v1(1.0f, 0.0f, 0.0f);
	rove::vec<3> v2(0.0f, 1.0f, 0.0f);
	rove::vec<3> v3 = v1 ^ v2;

	BOOST_REQUIRE(rove::abs(v3.x) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v3.y) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v3.z - 1.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec_cross_product_function)
{
	rove::vec<3> v1(1.0f, 0.0f, 0.0f);
	rove::vec<3> v2(0.0f, 1.0f, 0.0f);
	rove::vec<3> v3;
	rove::cross_product(v3, v1, v2);

	BOOST_REQUIRE(rove::abs(v3.x) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v3.y) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v3.z - 1.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(vec_cross_product_anti_commutative)
{
	rove::vec<3> v1(1.0f, 2.0f, 3.0f);
	rove::vec<3> v2(4.0f, 5.0f, 6.0f);
	rove::vec<3> v3 = v1 ^ v2;
	rove::vec<3> v4 = v2 ^ v1;

	BOOST_REQUIRE(rove::abs(v3.x + v4.x) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v3.y + v4.y) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v3.z + v4.z) < rove::EPSILON);
}

// Collinearity tests
BOOST_AUTO_TEST_CASE(vec2_collinear)
{
	rove::vec<2> v1(1.0f, 2.0f);
	rove::vec<2> v2(2.0f, 4.0f);  // Parallel to v1
	rove::vec<2> v3(1.0f, 1.0f);  // Not parallel

	BOOST_REQUIRE(v1.is_collinear(v2));
	BOOST_REQUIRE(!v1.is_collinear(v3));
}

BOOST_AUTO_TEST_CASE(vec3_collinear)
{
	rove::vec<3> v1(1.0f, 2.0f, 3.0f);
	rove::vec<3> v2(2.0f, 4.0f, 6.0f);  // Parallel to v1
	rove::vec<3> v3(1.0f, 0.0f, 0.0f);  // Not parallel

	BOOST_REQUIRE(v1.is_collinear(v2));
	BOOST_REQUIRE(!v1.is_collinear(v3));
}

// Perpendicular tests
BOOST_AUTO_TEST_CASE(vec2_perpendicular)
{
	rove::vec<2> v(3.0f, 4.0f);
	rove::vec<2> perp = v.perpendicular();

	// Perpendicular vectors have zero dot product
	rove::scalar dot = v & perp;
	BOOST_REQUIRE(rove::abs(dot) < rove::EPSILON);
}

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

// MinMax tests
BOOST_AUTO_TEST_CASE(vec_minmax)
{
	rove::vec<3> v1(5.0f, 2.0f, 8.0f);
	rove::vec<3> v2(3.0f, 7.0f, 1.0f);

	rove::minmax(v1, v2);

	BOOST_REQUIRE(rove::abs(v1.x - 3.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v1.y - 2.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v1.z - 1.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v2.x - 5.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v2.y - 7.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v2.z - 8.0f) < rove::EPSILON);
}

// Lexicographical comparison
BOOST_AUTO_TEST_CASE(vec_lexicographical_less)
{
	rove::vec<3> v1(1.0f, 2.0f, 3.0f);
	rove::vec<3> v2(1.0f, 2.0f, 4.0f);
	rove::vec<3> v3(1.0f, 3.0f, 2.0f);

	BOOST_REQUIRE(rove::lexicographical_less(v1, v2));
	BOOST_REQUIRE(rove::lexicographical_less(v1, v3));
	BOOST_REQUIRE(!rove::lexicographical_less(v2, v1));
}

// Scalar on left-hand side
BOOST_AUTO_TEST_CASE(vec_scalar_lhs_multiplication)
{
	rove::vec<3> v1(2.0f, 3.0f, 4.0f);
	rove::vec<3> v2 = 3.0f * v1;

	BOOST_REQUIRE(rove::abs(v2.x - 6.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v2.y - 9.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(v2.z - 12.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_SUITE_END()
