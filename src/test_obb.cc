
#include <boost/test/unit_test.hpp>
#include "obb.h"
#include "aabb.h"
#include "ray.h"
#include "matrix.h"

BOOST_AUTO_TEST_SUITE(obb)

// 3D OBB Construction tests
BOOST_AUTO_TEST_CASE(construct_3d_from_aabb)
{
	rove::aabb<3> a(rove::vec<3>(0, 0, 0), rove::vec<3>(10, 20, 30));
	rove::obb<3> b(a);

	BOOST_REQUIRE((b.origin - rove::vec<3>(0, 0, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((b.tangent - rove::vec<3>(10, 0, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((b.normal - rove::vec<3>(0, 20, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((b.binormal - rove::vec<3>(0, 0, 30)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(construct_3d_from_vectors)
{
	rove::vec<3> o(1, 2, 3);
	rove::vec<3> i(4, 0, 0);
	rove::vec<3> j(0, 5, 0);
	rove::vec<3> k(0, 0, 6);

	rove::obb<3> b(o, i, j, k);

	BOOST_REQUIRE((b.origin - o).length() < rove::EPSILON);
	BOOST_REQUIRE((b.tangent - i).length() < rove::EPSILON);
	BOOST_REQUIRE((b.normal - j).length() < rove::EPSILON);
	BOOST_REQUIRE((b.binormal - k).length() < rove::EPSILON);
}

// 2D OBB Construction tests
BOOST_AUTO_TEST_CASE(construct_2d_from_aabb)
{
	rove::aabb<2> a(rove::vec<2>(0, 0), rove::vec<2>(10, 20));
	rove::obb<2> b(a);

	BOOST_REQUIRE((b.origin - rove::vec<2>(0, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((b.tangent - rove::vec<2>(10, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((b.normal - rove::vec<2>(0, 20)).length() < rove::EPSILON);
}

// 3D Geometric property tests
BOOST_AUTO_TEST_CASE(get_centre_3d)
{
	rove::obb<3> b;
	b.origin.set(0, 0, 0);
	b.tangent.set(10, 0, 0);
	b.normal.set(0, 20, 0);
	b.binormal.set(0, 0, 30);

	rove::vec<3> c = b.get_centre();

	BOOST_REQUIRE((c - rove::vec<3>(5, 10, 15)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(get_volume_3d)
{
	rove::obb<3> b;
	b.origin.set(0, 0, 0);
	b.tangent.set(2, 0, 0);
	b.normal.set(0, 3, 0);
	b.binormal.set(0, 0, 4);

	rove::scalar volume = b.get_volume();

	BOOST_REQUIRE(rove::abs(volume - 24.0f) < rove::EPSILON);
}

// 3D Contains tests
BOOST_AUTO_TEST_CASE(contains_point_3d_inside)
{
	rove::obb<3> b;
	b.origin.set(0, 0, 0);
	b.tangent.set(10, 0, 0);
	b.normal.set(0, 10, 0);
	b.binormal.set(0, 0, 10);

	BOOST_REQUIRE(b.contains(rove::vec<3>(5, 5, 5)));
	BOOST_REQUIRE(b.contains(rove::vec<3>(0, 0, 0))); // boundary
	BOOST_REQUIRE(b.contains(rove::vec<3>(10, 10, 10))); // boundary
}

BOOST_AUTO_TEST_CASE(contains_point_3d_outside)
{
	rove::obb<3> b;
	b.origin.set(0, 0, 0);
	b.tangent.set(10, 0, 0);
	b.normal.set(0, 10, 0);
	b.binormal.set(0, 0, 10);

	BOOST_REQUIRE(!b.contains(rove::vec<3>(15, 5, 5)));
	BOOST_REQUIRE(!b.contains(rove::vec<3>(-1, 5, 5)));
	BOOST_REQUIRE(!b.contains(rove::vec<3>(5, 11, 5)));
}

// 2D Contains tests
BOOST_AUTO_TEST_CASE(contains_point_2d_inside)
{
	rove::obb<2> b;
	b.origin.set(0, 0);
	b.tangent.set(10, 0);
	b.normal.set(0, 10);

	BOOST_REQUIRE(b.contains(rove::vec<2>(5, 5)));
	BOOST_REQUIRE(b.contains(rove::vec<2>(0, 0)));
}

BOOST_AUTO_TEST_CASE(contains_point_2d_outside)
{
	rove::obb<2> b;
	b.origin.set(0, 0);
	b.tangent.set(10, 0);
	b.normal.set(0, 10);

	BOOST_REQUIRE(!b.contains(rove::vec<2>(15, 5)));
	BOOST_REQUIRE(!b.contains(rove::vec<2>(-1, 5)));
}

// Rotated OBB contains test
BOOST_AUTO_TEST_CASE(contains_rotated_obb_3d)
{
	rove::obb<3> b;
	b.origin.set(0, 0, 0);
	b.tangent.set(1, 1, 0); // rotated 45 degrees in XY plane
	b.normal.set(-1, 1, 0);
	b.binormal.set(0, 0, 1);

	// Point at center of the rotated box
	rove::vec<3> center = b.origin + b.tangent * 0.5f + b.normal * 0.5f + b.binormal * 0.5f;
	BOOST_REQUIRE(b.contains(center));
}

// Get vertex tests
BOOST_AUTO_TEST_CASE(get_vertex_3d)
{
	rove::obb<3> b;
	b.origin.set(1, 2, 3);
	b.tangent.set(4, 0, 0);
	b.normal.set(0, 5, 0);
	b.binormal.set(0, 0, 6);

	rove::vec<3> v0, v7;
	b.get_vertex(0, v0);
	b.get_vertex(7, v7);

	BOOST_REQUIRE((v0 - b.origin).length() < rove::EPSILON);
	BOOST_REQUIRE((v7 - (b.origin + b.tangent + b.normal + b.binormal)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(get_vertex_2d)
{
	rove::obb<2> b;
	b.origin.set(0, 0);
	b.tangent.set(10, 0);
	b.normal.set(0, 10);

	rove::vec<2> v0 = b.get_vertex(0);
	rove::vec<2> v1 = b.get_vertex(1);
	rove::vec<2> v2 = b.get_vertex(2);
	rove::vec<2> v3 = b.get_vertex(3);

	BOOST_REQUIRE((v0 - rove::vec<2>(0, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((v1 - rove::vec<2>(10, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((v2 - rove::vec<2>(10, 10)).length() < rove::EPSILON);
	BOOST_REQUIRE((v3 - rove::vec<2>(0, 10)).length() < rove::EPSILON);
}

// Get side tests for 2D
BOOST_AUTO_TEST_CASE(get_side_2d)
{
	rove::obb<2> b;
	b.origin.set(0, 0);
	b.tangent.set(10, 0);
	b.normal.set(0, 10);

	rove::line<2> side0 = b.get_side(0);
	BOOST_REQUIRE((side0.A - rove::vec<2>(0, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((side0.B - rove::vec<2>(10, 0)).length() < rove::EPSILON);
}

// Get normal tests for 2D
BOOST_AUTO_TEST_CASE(get_normal_2d)
{
	rove::obb<2> b;
	b.origin.set(0, 0);
	b.tangent.set(10, 0);
	b.normal.set(0, 10);

	rove::vec<2> n0 = b.get_normal(0);
	rove::vec<2> n1 = b.get_normal(1);
	rove::vec<2> n2 = b.get_normal(2);
	rove::vec<2> n3 = b.get_normal(3);

	// Normals should be unit vectors
	BOOST_REQUIRE(rove::abs(n0.length() - 1.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(n1.length() - 1.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(n2.length() - 1.0f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(n3.length() - 1.0f) < rove::EPSILON);
}

// Get segment tests (octants)
BOOST_AUTO_TEST_CASE(get_segment_3d)
{
	rove::obb<3> b;
	b.origin.set(0, 0, 0);
	b.tangent.set(10, 0, 0);
	b.normal.set(0, 10, 0);
	b.binormal.set(0, 0, 10);

	rove::obb<3> segment;
	b.get_segment(0, segment);

	// Segment 0 should have origin at (0,0,0) and half dimensions
	BOOST_REQUIRE((segment.origin - rove::vec<3>(0, 0, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((segment.tangent - rove::vec<3>(5, 0, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((segment.normal - rove::vec<3>(0, 5, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((segment.binormal - rove::vec<3>(0, 0, 5)).length() < rove::EPSILON);
}

// Get AABB tests
BOOST_AUTO_TEST_CASE(get_aabb_3d_axis_aligned)
{
	rove::obb<3> b;
	b.origin.set(0, 0, 0);
	b.tangent.set(10, 0, 0);
	b.normal.set(0, 20, 0);
	b.binormal.set(0, 0, 30);

	rove::aabb<3> aabb;
	b.get_aabb(aabb);

	BOOST_REQUIRE((aabb.lo - rove::vec<3>(0, 0, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((aabb.hi - rove::vec<3>(10, 20, 30)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(get_aabb_2d)
{
	rove::obb<2> b;
	b.origin.set(0, 0);
	b.tangent.set(10, 0);
	b.normal.set(0, 10);

	rove::aabb<2> aabb;
	b.get_aabb(aabb);

	BOOST_REQUIRE((aabb.lo - rove::vec<2>(0, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((aabb.hi - rove::vec<2>(10, 10)).length() < rove::EPSILON);
}

// World to local transformation tests
BOOST_AUTO_TEST_CASE(world_to_local_vector)
{
	rove::obb<3> bounds;
	bounds.origin.set(0, 0, 0);
	bounds.tangent.set(1, 2, 3);
	bounds.normal.set(3, 1, 2);
	bounds.binormal.set(2, 3, 1);

	rove::vec<3> point, point_local, point_world;

	point.set(100, 50, -2);

	bounds.world_to_local_vector(point_local, point);
	bounds.local_to_world_vector(point_world, point_local);

	BOOST_REQUIRE((point - point_world).length_sq() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(world_to_local_point_3d)
{
	rove::obb<3> b;
	b.origin.set(10, 10, 10);
	b.tangent.set(20, 0, 0);
	b.normal.set(0, 20, 0);
	b.binormal.set(0, 0, 20);

	rove::vec<3> world_point(20, 20, 20); // center of the box
	rove::vec<3> local_point;

	b.world_to_local_point(local_point, world_point);

	// Should be at (0.5, 0.5, 0.5) in local space
	BOOST_REQUIRE(rove::abs(local_point.x - 0.5f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(local_point.y - 0.5f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(local_point.z - 0.5f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(world_to_local_vector_2d)
{
	rove::obb<2> b;
	b.origin.set(0, 0);
	b.tangent.set(10, 0);
	b.normal.set(0, 20);

	rove::vec<2> world_vec(5, 10);
	rove::vec<2> local_vec = b.world_to_local_vector(world_vec);

	BOOST_REQUIRE(rove::abs(local_vec.x - 0.5f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(local_vec.y - 0.5f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(world_to_local_point_2d)
{
	rove::obb<2> b;
	b.origin.set(5, 5);
	b.tangent.set(10, 0);
	b.normal.set(0, 10);

	rove::vec<2> world_point(10, 10); // center
	rove::vec<2> local_point = b.world_to_local_point(world_point);

	BOOST_REQUIRE(rove::abs(local_point.x - 0.5f) < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(local_point.y - 0.5f) < rove::EPSILON);
}

// Transformation matrix tests
BOOST_AUTO_TEST_CASE(transform_3d_identity)
{
	rove::obb<3> b;
	b.origin.set(0, 0, 0);
	b.tangent.set(10, 0, 0);
	b.normal.set(0, 10, 0);
	b.binormal.set(0, 0, 10);

	rove::matrix<4, 4> identity;
	identity.identity();

	b.transform(identity);

	BOOST_REQUIRE((b.origin - rove::vec<3>(0, 0, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((b.tangent - rove::vec<3>(10, 0, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((b.normal - rove::vec<3>(0, 10, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((b.binormal - rove::vec<3>(0, 0, 10)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(transform_3d_translation)
{
	rove::obb<3> b;
	b.origin.set(0, 0, 0);
	b.tangent.set(10, 0, 0);
	b.normal.set(0, 10, 0);
	b.binormal.set(0, 0, 10);

	rove::matrix<4, 4> translation;
	translation.identity();
	translation.translation(5, 5, 5);

	b.transform(translation);

	BOOST_REQUIRE((b.origin - rove::vec<3>(5, 5, 5)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(transform_2d_identity)
{
	rove::obb<2> b;
	b.origin.set(0, 0);
	b.tangent.set(10, 0);
	b.normal.set(0, 10);

	rove::matrix<3, 3> identity;
	identity.identity();

	b.transform(identity);

	BOOST_REQUIRE((b.origin - rove::vec<2>(0, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((b.tangent - rove::vec<2>(10, 0)).length() < rove::EPSILON);
	BOOST_REQUIRE((b.normal - rove::vec<2>(0, 10)).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_SUITE_END()

