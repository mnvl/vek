
#include <stdlib.h>
#include <boost/test/unit_test.hpp>
#include "matrix.h"
#include "quaternion.h"

#include <iostream>

namespace
{

void test_matrix_quaternion_matrix(rove::matrix<3, 3> const& tf) {
	BOOST_REQUIRE (abs(tf.determinant() - 1) < rove::EPSILON);

	rove::quaternion<> q;
	q.set_unit(tf);

	rove::matrix<3, 3> m;
	m.rotation(q.normalized());

	BOOST_REQUIRE (rove::equal(m, tf, rove::scalar(0.001)));
}

void test_slerp_i(rove::vec<3> const& p, rove::vec<3> const& q) {
	rove::matrix<3, 3> tf1;
	tf1.rotation(p);
	rove::quaternion<> q1;
	q1.set_unit(tf1);

	rove::matrix<3, 3> tf2;
	tf2.rotation(q);
	rove::quaternion<> q2;
	q2.set_unit(tf2);

	rove::quaternion_slerper<> slerper;
	slerper.setup(q1, q2);

	for (int i = 0; i <= 100; ++i) {
		rove::scalar x = rove::scalar(i) / 100;

		rove::quaternion<> q3 = slerper.interpolate(x);
		rove::matrix<3, 3> tf3;
		tf3.rotation(q3);

		rove::vec<3> angles = p * (1 - x) + q * x;
		rove::matrix<3, 3> tf4;
		tf4.rotation(angles);

		BOOST_REQUIRE(equal(tf3, tf4, rove::scalar(0.001)));
	}
}

void test_slerp(rove::vec<3> const& p, rove::vec<3> const& q) {
	test_slerp_i(p, q);
	test_slerp_i(q, p);
}

}

BOOST_AUTO_TEST_SUITE (test_quaternion)

BOOST_AUTO_TEST_CASE (identity_matrix)
{
	rove::matrix<3, 3> tf;
	tf.identity();

	test_matrix_quaternion_matrix(tf);
}

BOOST_AUTO_TEST_CASE (rotate_90_degrees)
{
	rove::matrix<3, 3> tf;

	tf.rotation(rove::PI / 2, 0, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(-rove::PI / 2, 0, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, rove::PI / 2, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, -rove::PI / 2, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, 0, rove::PI / 2);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, 0, -rove::PI / 2);
	test_matrix_quaternion_matrix(tf);
}

BOOST_AUTO_TEST_CASE (rotate_60_degrees)
{
	rove::matrix<3, 3> tf;

	tf.rotation(rove::PI / 3, 0, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(-rove::PI / 3, 0, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, rove::PI / 3, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, -rove::PI / 3, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, 0, rove::PI / 3);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, 0, -rove::PI / 3);
	test_matrix_quaternion_matrix(tf);
}

BOOST_AUTO_TEST_CASE (rotate_30_degrees)
{
	rove::matrix<3, 3> tf;

	tf.rotation(rove::PI / 6, 0, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(-rove::PI / 6, 0, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, rove::PI / 6, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, -rove::PI / 6, 0);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, 0, rove::PI / 6);
	test_matrix_quaternion_matrix(tf);

	tf.rotation(0, 0, -rove::PI / 6);
	test_matrix_quaternion_matrix(tf);
}

BOOST_AUTO_TEST_CASE (rotate_random_degrees)
{
	rove::matrix<3, 3> tf;

	for (int i = 0; i < 1000; ++i) {
		rove::vec<3> angles(
			2 * rove::scalar(rand()) / RAND_MAX * rove::PI,
			2 * rove::scalar(rand()) / RAND_MAX * rove::PI,
			2 * rove::scalar(rand()) / RAND_MAX * rove::PI);
		tf.rotation(angles);
		test_matrix_quaternion_matrix(tf);
	}
}

BOOST_AUTO_TEST_CASE (slerp_identity)
{
	test_slerp(rove::vec<3>(0, 0, 0), rove::vec<3>(0, 0, 0));
}

BOOST_AUTO_TEST_CASE (slerp_90_degrees)
{
	test_slerp(rove::vec<3>(0, 0, 0), rove::vec<3>(rove::PI / 2, 0, 0));
	test_slerp(rove::vec<3>(0, 0, 0), rove::vec<3>(-rove::PI / 2, 0, 0));
	test_slerp(rove::vec<3>(0, 0, 0), rove::vec<3>(0, rove::PI / 2, 0));
	test_slerp(rove::vec<3>(0, 0, 0), rove::vec<3>(0, -rove::PI / 2, 0));
	test_slerp(rove::vec<3>(0, 0, 0), rove::vec<3>(0, 0, rove::PI / 2));
	test_slerp(rove::vec<3>(0, 0, 0), rove::vec<3>(0, 0, -rove::PI / 2));

	test_slerp(rove::vec<3>(-rove::PI/2, 0, 0), rove::vec<3>(rove::PI / 2, 0, 0));
	test_slerp(rove::vec<3>(0, -rove::PI/2, 0), rove::vec<3>(0, rove::PI / 2, 0));
	test_slerp(rove::vec<3>(0, 0, -rove::PI/2), rove::vec<3>(0, 0, rove::PI / 2));
}

BOOST_AUTO_TEST_CASE (slerp_random_degrees)
{
	for (int i = 0; i < 1000; ++i) {
		rove::vec<3> axis(
			2 * rove::scalar(rand()) / RAND_MAX - 1,
			2 * rove::scalar(rand()) / RAND_MAX - 1,
			2 * rove::scalar(rand()) / RAND_MAX - 1);

		if (axis.length_sq() < 0.1) continue;

		rove::scalar angle1 = rove::scalar(rand()) / RAND_MAX * rove::PI * 2;
		rove::scalar angle2 = angle1 + rove::scalar(rand() - 1) / RAND_MAX * rove::PI;

		rove::matrix<3,3> tf1;
		tf1.rotation(axis, angle1);

		rove::matrix<3,3> tf2;
		tf2.rotation(axis, angle2);

		rove::quaternion<> q1;
		q1.set_unit(tf1);

		rove::quaternion<> q2;
		q2.set_unit(tf2);

		rove::quaternion_slerper<> slerper;
		slerper.setup(q1, q2);

		for (int j = 0; j <= 100; ++j) {
			rove::scalar x = rove::scalar(j) / 100;

			rove::quaternion<> q = slerper.interpolate(x);

			rove::matrix<3,3> m1;
			m1.rotation(q);

			rove::matrix<3,3> m2;
			m2.rotation(axis, angle1 * (1 - x) + angle2 * x);

			BOOST_REQUIRE(equal(m1, m2, rove::scalar(0.001)));
		}
	}
}

// Test that set_unit produces a unit quaternion without needing normalization
// This catches the bug where w component was missing /2 divisor
BOOST_AUTO_TEST_CASE (set_unit_produces_unit_quaternion)
{
	// Identity matrix should produce quaternion (0, 0, 0, 1)
	rove::matrix<3, 3> identity;
	identity.identity();
	rove::quaternion<> q_identity;
	q_identity.set_unit(identity);

	BOOST_REQUIRE(abs(q_identity.norm() - 1) < rove::EPSILON);
	BOOST_REQUIRE(abs(q_identity.w - 1) < rove::EPSILON);
	BOOST_REQUIRE(abs(q_identity.x) < rove::EPSILON);
	BOOST_REQUIRE(abs(q_identity.y) < rove::EPSILON);
	BOOST_REQUIRE(abs(q_identity.z) < rove::EPSILON);

	// 90-degree rotation around X axis should produce unit quaternion
	rove::matrix<3, 3> rot_x;
	rot_x.rotation(rove::PI / 2, 0, 0);
	rove::quaternion<> q_rot_x;
	q_rot_x.set_unit(rot_x);

	BOOST_REQUIRE(abs(q_rot_x.norm() - 1) < rove::EPSILON);

	// Random rotations should produce unit quaternions
	for (int i = 0; i < 100; ++i) {
		rove::vec<3> angles(
			2 * rove::scalar(rand()) / RAND_MAX * rove::PI,
			2 * rove::scalar(rand()) / RAND_MAX * rove::PI,
			2 * rove::scalar(rand()) / RAND_MAX * rove::PI);

		rove::matrix<3, 3> tf;
		tf.rotation(angles);

		rove::quaternion<> q;
		q.set_unit(tf);

		BOOST_REQUIRE(abs(q.norm() - 1) < rove::EPSILON);
	}
}

// Test SLERP with quaternions that have negative dot product
// This catches the bug where SLERP would take the long path instead of short path
BOOST_AUTO_TEST_CASE (slerp_negative_dot_product)
{
	// Create a quaternion and its negation (same rotation, opposite quaternion)
	// Then test SLERP from a third quaternion to both - should give same result
	rove::vec<3> axis(0, 0, 1);

	// q1: small rotation around Z
	rove::matrix<3, 3> tf1;
	tf1.rotation(axis, rove::scalar(0.2));
	rove::quaternion<> q1;
	q1.set_unit(tf1);

	// q2: another rotation around Z
	rove::matrix<3, 3> tf2;
	tf2.rotation(axis, rove::scalar(0.5));
	rove::quaternion<> q2;
	q2.set_unit(tf2);

	// q2_neg: negation of q2 (same rotation, but negative dot product with q1)
	rove::quaternion<> q2_neg = -q2;

	// Verify q2_neg has negative dot product with q1
	rove::scalar dot_pos = rove::dot_product(q1.normalized(), q2.normalized());
	rove::scalar dot_neg = rove::dot_product(q1.normalized(), q2_neg.normalized());
	BOOST_REQUIRE(dot_pos > 0);
	BOOST_REQUIRE(dot_neg < 0);

	// SLERP to q2 and to q2_neg should produce the same rotation matrices
	rove::quaternion_slerper<> slerper_pos, slerper_neg;
	slerper_pos.setup(q1, q2);
	slerper_neg.setup(q1, q2_neg);

	for (int i = 0; i <= 10; ++i) {
		rove::scalar t = rove::scalar(i) / 10;

		rove::quaternion<> r_pos = slerper_pos.interpolate(t);
		rove::quaternion<> r_neg = slerper_neg.interpolate(t);

		// Both should be unit quaternions
		BOOST_REQUIRE(abs(r_pos.norm() - 1) < rove::EPSILON);
		BOOST_REQUIRE(abs(r_neg.norm() - 1) < rove::EPSILON);

		// Convert to matrices - should be equal since SLERP should handle negative dot
		rove::matrix<3, 3> m_pos, m_neg;
		m_pos.rotation(r_pos);
		m_neg.rotation(r_neg);

		BOOST_REQUIRE(rove::equal(m_pos, m_neg, rove::scalar(0.001)));
	}
}

// Test SLERP specifically with opposite quaternions (dot product near -1)
BOOST_AUTO_TEST_CASE (slerp_opposite_quaternions)
{
	rove::quaternion<> q1;
	q1.identity();  // (0, 0, 0, 1)

	// Create quaternion for 180-degree rotation around Z axis
	// This gives (0, 0, 1, 0) which has dot product 0 with identity
	// But we want to test near -1, so use a small rotation and its opposite
	rove::matrix<3, 3> tf;
	tf.rotation(0, 0, rove::scalar(0.1));
	rove::quaternion<> q2;
	q2.set_unit(tf);

	// Negate q2 - represents same rotation but opposite quaternion
	rove::quaternion<> q2_neg = -q2;

	// dot(q1, q2_neg) should be negative
	rove::scalar dot = rove::dot_product(q1, q2_neg);
	BOOST_REQUIRE(dot < 0);

	// SLERP between q1 and q2_neg should give same result as q1 to q2
	rove::quaternion_slerper<> slerper1, slerper2;
	slerper1.setup(q1, q2);
	slerper2.setup(q1, q2_neg);

	for (int i = 0; i <= 10; ++i) {
		rove::scalar t = rove::scalar(i) / 10;

		rove::quaternion<> r1 = slerper1.interpolate(t);
		rove::quaternion<> r2 = slerper2.interpolate(t);

		// Convert to matrices - should be equal since q and -q represent same rotation
		rove::matrix<3, 3> m1, m2;
		m1.rotation(r1);
		m2.rotation(r2);

		BOOST_REQUIRE(rove::equal(m1, m2, rove::scalar(0.001)));
	}
}

BOOST_AUTO_TEST_SUITE_END()

