
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

// --- 3x3 basic operations ---

BOOST_AUTO_TEST_CASE(test_3x3_identity)
{
	rove::matrix<3,3> m;
	m.identity();
	BOOST_REQUIRE(rove::abs(m.ij[0][0] - 1) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(m.ij[1][1] - 1) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(m.ij[2][2] - 1) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(m.ij[0][1]) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(m.ij[0][2]) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(m.ij[1][0]) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(m.ij[1][2]) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(m.ij[2][0]) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(m.ij[2][1]) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(test_3x3_zero)
{
	rove::matrix<3,3> m;
	m.zero();
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			BOOST_REQUIRE(rove::abs(m.ij[i][j]) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(test_3x3_trace_identity)
{
	rove::matrix<3,3> m;
	m.identity();
	BOOST_REQUIRE(rove::abs(m.trace() - 3.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(test_3x3_trace_scaling)
{
	rove::matrix<3,3> m;
	m.scaling(2.0f, 3.0f);
	// ij[0][0]=2, ij[1][1]=3, ij[2][2]=1 → trace = 6
	BOOST_REQUIRE(rove::abs(m.trace() - 6.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(test_3x3_transpose_rotation)
{
	rove::matrix<3,3> m, tp, result, identity;
	identity.identity();
	m.rotation(rove::vec<3>(0, 0, 1), rove::PI / 4);
	m.transpose(tp);
	// For rotation: M * M^T = I
	rove::mul(result, m, tp);
	BOOST_REQUIRE(rove::equal(result, identity));
}

BOOST_AUTO_TEST_CASE(test_3x3_inverse_rotation)
{
	rove::matrix<3,3> m, inv, result, identity;
	identity.identity();
	m.rotation(rove::vec<3>(1, 1, 0), rove::PI / 3);
	bool ok = m.inverse(inv);
	BOOST_REQUIRE(ok);
	rove::mul(result, m, inv);
	BOOST_REQUIRE(rove::equal(result, identity));
}

BOOST_AUTO_TEST_CASE(test_3x3_inverse_scaling)
{
	rove::matrix<3,3> m, inv, result, identity;
	identity.identity();
	m.scaling(2.0f, 4.0f);
	bool ok = m.inverse(inv);
	BOOST_REQUIRE(ok);
	rove::mul(result, m, inv);
	BOOST_REQUIRE(rove::equal(result, identity));
}

BOOST_AUTO_TEST_CASE(test_3x3_translation_transform)
{
	rove::matrix<3,3> m;
	m.translation(5.0f, -3.0f);
	rove::vec<2> p(1, 2);
	rove::vec<2> result = p * m;
	BOOST_REQUIRE(rove::abs(result.x - 6.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(result.y - (-1.0f)) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(test_3x3_scaling_transform)
{
	rove::matrix<3,3> m;
	m.scaling(2.0f, 3.0f);
	rove::vec<2> p(4, 5);
	rove::vec<2> result = p * m;
	BOOST_REQUIRE(rove::abs(result.x - 8.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(result.y - 15.0f) < rove::EPSILON);
}

// --- 4x4 transpose ---

BOOST_AUTO_TEST_CASE(test_4x4_transpose_rotation)
{
	rove::matrix<4,4> m, tp, result, identity;
	identity.identity();
	m.rotation(rove::PI / 3, rove::PI / 4, rove::PI / 6);
	m.transpose(tp);
	// Pure rotation: M * M^T = I
	rove::mul(result, m, tp);
	BOOST_REQUIRE(rove::equal(result, identity));
}

BOOST_AUTO_TEST_CASE(test_4x4_transpose_inverts_itself)
{
	rove::matrix<4,4> m, tp, result;
	m.rotation(0.5f, 0.3f, 0.7f);
	m.transpose(tp);
	tp.transpose(result);
	BOOST_REQUIRE(rove::equal(result, m));
}

// --- 4x4 translation and scaling transforms ---

BOOST_AUTO_TEST_CASE(test_4x4_translation_transform)
{
	rove::matrix<4,4> m;
	m.translation(1.0f, 2.0f, 3.0f);
	rove::vec<3> p(5, 6, 7);
	rove::vec<3> result = p * m;
	BOOST_REQUIRE(rove::abs(result.x - 6.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(result.y - 8.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(result.z - 10.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(test_4x4_scaling_transform)
{
	rove::matrix<4,4> m;
	m.scaling(2.0f, 3.0f, 4.0f);
	rove::vec<3> p(1, 2, 3);
	rove::vec<3> result = p * m;
	BOOST_REQUIRE(rove::abs(result.x - 2.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(result.y - 6.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(result.z - 12.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(test_4x4_translation_then_scale)
{
	rove::matrix<4,4> m;
	m.translation(10.0f, 0.0f, 0.0f);
	m.scale(2.0f, 2.0f, 2.0f);
	rove::vec<3> p(0, 0, 0);
	rove::vec<3> result = p * m;
	// translate puts at (10,0,0), then scale doubles: (20,0,0)
	BOOST_REQUIRE(rove::abs(result.x - 20.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(result.y) < rove::EPSILON);
}

// --- lookat ---
//
// lookat(eye, at, up) builds a view matrix (LH convention, +Z forward):
//   z_axis = normalize(at - eye)
//   x_axis = normalize(up ^ z_axis)
//   y_axis = z_axis ^ x_axis
// After applying: eye transforms to origin, at is at positive-Z.

BOOST_AUTO_TEST_CASE(test_lookat_eye_maps_to_origin)
{
	rove::matrix<4,4> m;
	rove::vec<3> eye(0, 0, 5), at(0, 0, 0), up(0, 1, 0);
	m.lookat(eye, at, up);
	rove::vec<3> result = eye * m;
	BOOST_REQUIRE(result.length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(test_lookat_at_is_forward)
{
	rove::matrix<4,4> m;
	rove::vec<3> eye(0, 0, 5), at(0, 0, 0), up(0, 1, 0);
	m.lookat(eye, at, up);
	// 'at' should project to a point along the forward axis (positive Z)
	rove::vec<3> at_view = at * m;
	BOOST_REQUIRE(rove::abs(at_view.x) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(at_view.y) < rove::EPSILON);
	BOOST_REQUIRE(at_view.z > 0);
}

BOOST_AUTO_TEST_CASE(test_lookat_up_preserved)
{
	rove::matrix<4,4> m;
	// Camera at origin looking along +X, up is +Y
	rove::vec<3> eye(0, 0, 0), at(1, 0, 0), up(0, 1, 0);
	m.lookat(eye, at, up);
	// A point above the camera in world space should be above in view space
	rove::vec<3> above = rove::vec<3>(0, 1, 0) * m;
	BOOST_REQUIRE(above.y > 0);
	BOOST_REQUIRE(rove::abs(above.x) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(test_lookat_distance_preserved)
{
	rove::matrix<4,4> m;
	rove::vec<3> eye(0, 0, 0), at(5, 0, 0), up(0, 1, 0);
	m.lookat(eye, at, up);
	// at is 5 units from eye; in view space it should be 5 units along forward
	rove::vec<3> at_view = at * m;
	BOOST_REQUIRE(rove::abs(at_view.z - 5.0f) < rove::EPSILON);
}

// --- ortho ---
//
// ortho(width, height, z_near, z_far) — D3D LH convention.
// Maps x ∈ [-width/2, width/2] to NDC [-1, 1],
//      y ∈ [-height/2, height/2] to NDC [-1, 1],
//      z ∈ [z_near, z_far] to [0, 1].

BOOST_AUTO_TEST_CASE(test_ortho_matrix_elements)
{
	rove::matrix<4,4> m;
	m.ortho(2.0f, 4.0f, 1.0f, 100.0f);
	// ij[0][0] = 2/width = 1, ij[1][1] = 2/height = 0.5
	BOOST_REQUIRE(rove::abs(m.ij[0][0] - 1.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(m.ij[1][1] - 0.5f) < rove::EPSILON);
	// ij[2][2] = 1/(z_far - z_near) = 1/99
	BOOST_REQUIRE(rove::abs(m.ij[2][2] - (1.0f / 99.0f)) < rove::EPSILON);
	// Translation row: ij[3][3] = 1 (no projection divide)
	BOOST_REQUIRE(rove::abs(m.ij[3][3] - 1.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(test_ortho_z_depth_mapping)
{
	rove::matrix<4,4> m;
	m.ortho(2.0f, 2.0f, 0.0f, 10.0f);
	// Near plane (z=0): maps to z_ndc = 0
	rove::vec<3> near_pt = rove::vec<3>(0, 0, 0) * m;
	BOOST_REQUIRE(rove::abs(near_pt.z) < rove::EPSILON);
	// Far plane (z=10): maps to z_ndc = 1
	rove::vec<3> far_pt = rove::vec<3>(0, 0, 10) * m;
	BOOST_REQUIRE(rove::abs(far_pt.z - 1.0f) < rove::EPSILON);
	// Midpoint (z=5): maps to z_ndc = 0.5
	rove::vec<3> mid_pt = rove::vec<3>(0, 0, 5) * m;
	BOOST_REQUIRE(rove::abs(mid_pt.z - 0.5f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(test_ortho_xy_mapping)
{
	rove::matrix<4,4> m;
	m.ortho(4.0f, 2.0f, 0.0f, 100.0f);
	// x=2 (right edge) should map to NDC x=1
	rove::vec<3> right = rove::vec<3>(2, 0, 0) * m;
	BOOST_REQUIRE(rove::abs(right.x - 1.0f) < rove::EPSILON);
	// y=1 (top edge) should map to NDC y=1
	rove::vec<3> top = rove::vec<3>(0, 1, 0) * m;
	BOOST_REQUIRE(rove::abs(top.y - 1.0f) < rove::EPSILON);
}

// --- perspective ---
//
// perspective(fovy, aspect, z_near, z_far) — D3D LH convention.
// For fovy=PI/2, aspect=1: h=1, w=1.

BOOST_AUTO_TEST_CASE(test_perspective_matrix_structure)
{
	rove::matrix<4,4> m;
	m.perspective(rove::PI / 2, 1.0f, 1.0f, 100.0f);
	// For fovy=PI/2, aspect=1: h=1/tan(PI/4)=1, w=h/aspect=1
	BOOST_REQUIRE(rove::abs(m.ij[0][0] - 1.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(m.ij[1][1] - 1.0f) < rove::EPSILON);
	// ij[2][3]=1: perspective divide (w=z)
	BOOST_REQUIRE(rove::abs(m.ij[2][3] - 1.0f) < rove::EPSILON);
	// ij[3][3]=0: no constant w contribution
	BOOST_REQUIRE(rove::abs(m.ij[3][3]) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(test_perspective_aspect_ratio)
{
	rove::matrix<4,4> m;
	m.perspective(rove::PI / 2, 2.0f, 1.0f, 100.0f);
	// aspect=2: w = h/aspect = 1/2 = 0.5, h = 1
	BOOST_REQUIRE(rove::abs(m.ij[0][0] - 0.5f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(m.ij[1][1] - 1.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(test_perspective_near_far_depth)
{
	rove::matrix<4,4> m;
	// Use large far/near ratio for clarity
	m.perspective(rove::PI / 2, 1.0f, 1.0f, 100.0f);
	// z_near should map to clip z / clip w = 0 in NDC
	// For vec3*(mat44): result.z = v.z * ij[2][2] + ij[3][2]
	//                   result.w = v.z * ij[2][3] (= v.z for the perspective row)
	// NDC.z = result.z / result.w
	rove::scalar z_near = 1.0f, z_far = 100.0f;
	rove::scalar z22 = m.ij[2][2];
	rove::scalar z32 = m.ij[3][2];
	// NDC at z=z_near: (z_near*z22 + z32) / z_near
	rove::scalar ndc_near = (z_near * z22 + z32) / z_near;
	BOOST_REQUIRE(rove::abs(ndc_near) < rove::EPSILON);
	// NDC at z=z_far: (z_far*z22 + z32) / z_far
	rove::scalar ndc_far = (z_far * z22 + z32) / z_far;
	BOOST_REQUIRE(rove::abs(ndc_far - 1.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_SUITE_END()
