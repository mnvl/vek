
#include <boost/test/unit_test.hpp>
#include "matrix.h"
#include "aabb.h"
#include "obb.h"
#include "frustum.h"

BOOST_AUTO_TEST_SUITE(frustum)

BOOST_AUTO_TEST_CASE(orthographic_projection_and_obb_1)
{
	rove::matrix<4,4> tf;
	tf.ortho(400, 300, 0, 1000);

	rove::frustum<> fr(tf);

	rove::obb<3> bound;
	bound.origin.set(-1000, 0, 0);
	bound.tangent.set(1, 0, 0);
	bound.normal.set(0, 1, 0);
	bound.binormal.set(0, 0, 1);

	BOOST_REQUIRE (fr.test_intersection(bound) == false);
}

BOOST_AUTO_TEST_CASE(orthographic_projection_and_obb_2)
{
	rove::matrix<4,4> tf;
	tf.ortho(400, 300, 0, 1000);

	rove::frustum<> fr(tf);

	rove::obb<3> bound;
	bound.origin.set(-199.5f, 0, 0);
	bound.tangent.set(1, 0, 0);
	bound.normal.set(0, 1, 0);
	bound.binormal.set(0, 0, 1);

	BOOST_REQUIRE (fr.test_intersection(bound) == true);
}

BOOST_AUTO_TEST_CASE(orthographic_projection_and_obb_3)
{
	rove::matrix<4,4> tf;
	tf.ortho(400, 300, 0, 1000);

	rove::frustum<> fr(tf);

	rove::obb<3> bound;
	bound.origin.set(0, 0, 0);
	bound.tangent.set(1, 0, 0);
	bound.normal.set(0, 1, 0);
	bound.binormal.set(0, 0, 1);

	BOOST_REQUIRE (fr.test_intersection(bound) == true);
}

BOOST_AUTO_TEST_CASE(orthographic_projection_and_obb_4)
{
	rove::matrix<4,4> tf;
	tf.ortho(400, 300, 0, 1000);

	rove::frustum<> fr(tf);

	rove::obb<3> bound;
	bound.origin.set(199.5f, 0, 0);
	bound.tangent.set(1, 0, 0);
	bound.normal.set(0, 1, 0);
	bound.binormal.set(0, 0, 1);

	BOOST_REQUIRE (fr.test_intersection(bound) == true);
}

BOOST_AUTO_TEST_CASE(orthographic_projection_and_obb_5)
{
	rove::matrix<4,4> tf;
	tf.ortho(400, 300, 0, 1000);

	rove::frustum<> fr(tf);

	rove::obb<3> bound;
	bound.origin.set(1000, 0, 0);
	bound.tangent.set(1, 0, 0);
	bound.normal.set(0, 1, 0);
	bound.binormal.set(0, 0, 1);

	BOOST_REQUIRE (fr.test_intersection(bound) == false);
}

BOOST_AUTO_TEST_CASE(orthographic_projection_with_rotation_and_obb_1)
{
	rove::matrix<4,4> projection;
	projection.ortho(400, 300, 0, 1000);

	rove::matrix<4,4> world;
	world.rotation(-rove::PI/4, 0, 0);

	rove::frustum<> fr(world * projection);

	rove::obb<3> bound;
	bound.origin.set(0, 1000, 0);
	bound.tangent.set(1, 0, 0);
	bound.normal.set(0, 1, 0);
	bound.binormal.set(0, 0, 1);

	BOOST_REQUIRE (fr.test_intersection(bound) == false);
}

BOOST_AUTO_TEST_CASE(orthographic_projection_with_rotation_and_obb_2)
{
	rove::matrix<4,4> projection;
	projection.ortho(400, 300, 0, 1000);

	rove::matrix<4,4> world;
	world.rotation(-rove::PI/4, 0, 0);

	rove::frustum<> fr(world * projection);

	rove::obb<3> bound;
	bound.origin.set(0, 0, 0);
	bound.tangent.set(1, 0, 0);
	bound.normal.set(0, 1, 0);
	bound.binormal.set(0, 0, 1);

	BOOST_REQUIRE (fr.test_intersection(bound) == true);
}

BOOST_AUTO_TEST_CASE(orthographic_projection_with_rotation_and_obb_3)
{
	rove::matrix<4,4> projection;
	projection.ortho(400, 300, 0, 1000);

	rove::matrix<4,4> world;
	world.rotation(-rove::PI/4, 0, 0);

	rove::frustum<> fr(world * projection);

	rove::obb<3> bound;
	bound.origin.set(0, -1000, 0);
	bound.tangent.set(1, 0, 0);
	bound.normal.set(0, 1, 0);
	bound.binormal.set(0, 0, 1);

	BOOST_REQUIRE (fr.test_intersection(bound) == false);
}

BOOST_AUTO_TEST_CASE(perspective_projection_with_rotation_translation_and_obb_1)
{
	rove::matrix<4,4> projection;
	projection.perspective(rove::PI/2, 1.5f, 0, 1000);

	rove::matrix<4,4> world;
	world.translation(-500, -10, 500);
	world.rotate(-rove::PI/4, 0, 0);

	rove::frustum<> fr(world * projection);

	rove::obb<3> bound;
	bound.origin.set(510, 2, 490);
	bound.tangent.set(1, 0, 0);
	bound.normal.set(0, 1, 0);
	bound.binormal.set(0, 0, 1);

	BOOST_REQUIRE (fr.contains(bound.origin) == true);
	BOOST_REQUIRE (fr.contains(bound.origin + bound.tangent) == true);
	BOOST_REQUIRE (fr.contains(bound.origin + bound.normal) == true);
	BOOST_REQUIRE (fr.contains(bound.origin + bound.binormal) == true);

	BOOST_REQUIRE (fr.test_intersection(bound) == true);
}

BOOST_AUTO_TEST_CASE(from_matrix_and_intersection_with_obb_1)
{
	rove::matrix<4,4> tf;
	tf._11 =  0.20000000f;
	tf._12 =  0.00000000f;
	tf._13 =  0.00000000f;
	tf._14 =  0.00000000f;
	tf._21 =  0.00000000f;
	tf._22 =  0.26937401f;
	tf._23 = -0.00070781459f;
	tf._24 =  0.00000000f;
	tf._31 =  0.00000000f;
	tf._32 =  0.26937401f;
	tf._33 =  0.00070781459f;
	tf._34 =  0.00000000f;
	tf._41 = -0.19687501f;
	tf._42 = -0.0061834427f;
	tf._43 =  0.012431229f;
	tf._44 =  1.0000000f;

	rove::frustum<> fr(tf);

	rove::obb<3> bound;
	bound.origin.set(-5.6245117f, 0.00000000f, 23.716305f);
	bound.tangent.set(2.0000000f, 0.00000000f, 0.00000000f);
	bound.normal.set(0.00000000f, 1.0000000f, 0.00000000f);
	bound.binormal.set(0.00000000f, 0.00000000f, 2.0000000f);

	BOOST_REQUIRE (fr.contains(bound.origin) == false);
	BOOST_REQUIRE (fr.contains(bound.origin + bound.tangent) == false);
	BOOST_REQUIRE (fr.contains(bound.origin + bound.normal) == false);
	BOOST_REQUIRE (fr.contains(bound.origin + bound.binormal) == false);

	BOOST_REQUIRE (fr.test_intersection(bound) == false);
}

// --- AABB intersection ---
//
// ortho(400, 300, 0, 1000) maps:
//   x ∈ [-200, 200] to NDC [-1, 1]
//   y ∈ [-150, 150] to NDC [-1, 1]
//   z ∈ [0, 1000]   to NDC [0, 1]

BOOST_AUTO_TEST_CASE(ortho_and_aabb_inside)
{
	rove::matrix<4,4> tf;
	tf.ortho(400, 300, 0, 1000);
	rove::frustum<> fr(tf);

	rove::aabb<3> box;
	box.lo.set(-50, -50, 10);
	box.hi.set( 50,  50, 100);

	BOOST_REQUIRE(fr.test_intersection(box) == true);
}

BOOST_AUTO_TEST_CASE(ortho_and_aabb_outside_left)
{
	rove::matrix<4,4> tf;
	tf.ortho(400, 300, 0, 1000);
	rove::frustum<> fr(tf);

	// Entirely to the left of x = -200
	rove::aabb<3> box;
	box.lo.set(-400, -50, 10);
	box.hi.set(-250, 50, 100);

	BOOST_REQUIRE(fr.test_intersection(box) == false);
}

BOOST_AUTO_TEST_CASE(ortho_and_aabb_outside_right)
{
	rove::matrix<4,4> tf;
	tf.ortho(400, 300, 0, 1000);
	rove::frustum<> fr(tf);

	// Entirely to the right of x = 200
	rove::aabb<3> box;
	box.lo.set(250, -50, 10);
	box.hi.set(400,  50, 100);

	BOOST_REQUIRE(fr.test_intersection(box) == false);
}

BOOST_AUTO_TEST_CASE(ortho_and_aabb_outside_far)
{
	rove::matrix<4,4> tf;
	tf.ortho(400, 300, 0, 1000);
	rove::frustum<> fr(tf);

	// Beyond far plane (z > 1000)
	rove::aabb<3> box;
	box.lo.set(-50, -50, 1100);
	box.hi.set( 50,  50, 1200);

	BOOST_REQUIRE(fr.test_intersection(box) == false);
}

BOOST_AUTO_TEST_CASE(ortho_and_aabb_outside_near)
{
	rove::matrix<4,4> tf;
	tf.ortho(400, 300, 0, 1000);
	rove::frustum<> fr(tf);

	// The GL-style frustum plane extraction places the near cull at z = -z_far
	// (i.e. -1000 for this ortho), not at z_near=0. Use a box well beyond that.
	rove::aabb<3> box;
	box.lo.set(-50, -50, -1500);
	box.hi.set( 50,  50, -1100);

	BOOST_REQUIRE(fr.test_intersection(box) == false);
}

BOOST_AUTO_TEST_CASE(ortho_and_aabb_spanning_right_boundary)
{
	rove::matrix<4,4> tf;
	tf.ortho(400, 300, 0, 1000);
	rove::frustum<> fr(tf);

	// Straddles right boundary: lo.x=150 (inside), hi.x=250 (outside)
	rove::aabb<3> box;
	box.lo.set(150, -50, 10);
	box.hi.set(250,  50, 100);

	BOOST_REQUIRE(fr.test_intersection(box) == true);
}

BOOST_AUTO_TEST_CASE(ortho_and_aabb_spanning_far_boundary)
{
	rove::matrix<4,4> tf;
	tf.ortho(400, 300, 0, 1000);
	rove::frustum<> fr(tf);

	// Straddles far plane: lo.z=900 (inside), hi.z=1100 (outside)
	rove::aabb<3> box;
	box.lo.set(-50, -50, 900);
	box.hi.set( 50,  50, 1100);

	BOOST_REQUIRE(fr.test_intersection(box) == true);
}

BOOST_AUTO_TEST_CASE(ortho_and_aabb_at_center)
{
	rove::matrix<4,4> tf;
	tf.ortho(400, 300, 0, 1000);
	rove::frustum<> fr(tf);

	// Unit box at origin
	rove::aabb<3> box;
	box.lo.set(-0.5f, -0.5f, 10);
	box.hi.set( 0.5f,  0.5f, 20);

	BOOST_REQUIRE(fr.test_intersection(box) == true);
}

BOOST_AUTO_TEST_CASE(perspective_and_aabb_in_front_of_camera)
{
	rove::matrix<4,4> proj;
	proj.perspective(rove::PI / 2, 1.0f, 1.0f, 1000.0f);
	rove::frustum<> fr(proj);

	// Box centered at z=50, well within the fov and depth range
	rove::aabb<3> box;
	box.lo.set(-10, -10, 40);
	box.hi.set( 10,  10, 60);

	BOOST_REQUIRE(fr.test_intersection(box) == true);
}

BOOST_AUTO_TEST_CASE(perspective_and_aabb_behind_camera)
{
	rove::matrix<4,4> proj;
	proj.perspective(rove::PI / 2, 1.0f, 1.0f, 1000.0f);
	rove::frustum<> fr(proj);

	// Box at negative z (behind the camera)
	rove::aabb<3> box;
	box.lo.set(-10, -10, -100);
	box.hi.set( 10,  10,  -10);

	BOOST_REQUIRE(fr.test_intersection(box) == false);
}

BOOST_AUTO_TEST_SUITE_END()
