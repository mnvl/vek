#include <set>
#include <vector>
#include <boost/test/unit_test.hpp>
#include "aabb.h"
#include "octree.h"
#include "sphere.h"
#include "triangle.h"

namespace
{

std::set<int> to_set(std::vector<int> const &values)
{
	return std::set<int>(values.begin(), values.end());
}

rove::sphere<3> random_sphere(rove::scalar max_radius = 0.25f)
{
	rove::scalar radius = rove::random(0.01f, max_radius);
	rove::vec<3> centre(
		rove::random(-1 + radius, 1 - radius),
		rove::random(-1 + radius, 1 - radius),
		rove::random(-1 + radius, 1 - radius));
	return rove::sphere<3>(centre, radius);
}

rove::aabb<3> random_aabb()
{
	rove::vec<3> half_size(
		rove::random(0.01f, 0.2f),
		rove::random(0.01f, 0.2f),
		rove::random(0.01f, 0.2f));

	rove::vec<3> centre(
		rove::random(-1 + half_size.x, 1 - half_size.x),
		rove::random(-1 + half_size.y, 1 - half_size.y),
		rove::random(-1 + half_size.z, 1 - half_size.z));

	return rove::aabb<3>(centre - half_size, centre + half_size);
}

rove::triangle<3> random_triangle()
{
	for (size_t i = 0; i < 64; ++i) {
		rove::vec<3> a(
			rove::random(-1.0f, 1.0f),
			rove::random(-1.0f, 1.0f),
			rove::random(-1.0f, 1.0f));
		rove::vec<3> b(
			rove::random(-1.0f, 1.0f),
			rove::random(-1.0f, 1.0f),
			rove::random(-1.0f, 1.0f));
		rove::vec<3> c(
			rove::random(-1.0f, 1.0f),
			rove::random(-1.0f, 1.0f),
			rove::random(-1.0f, 1.0f));

		if (((b - a) ^ (c - a)).length_sq() > rove::EPSILON) {
			return rove::triangle<3>(a, b, c);
		}
	}

	return rove::triangle<3>(
		rove::vec<3>(-0.5f, -0.5f, 0.0f),
		rove::vec<3>(0.5f, -0.5f, 0.0f),
		rove::vec<3>(0.0f, 0.5f, 0.0f));
}

template<class Primitive, class QueryPrimitive>
std::set<int> brute_force_intersection_set(std::vector<Primitive> const &primitives, QueryPrimitive const &query)
{
	typedef rove::octree_query_traits<Primitive, QueryPrimitive> query_traits_t;
	std::set<int> result;

	for (size_t i = 0; i < primitives.size(); ++i) {
		if (query_traits_t::intersects(primitives[i], query)) {
			result.insert(static_cast<int>(i));
		}
	}

	return result;
}

}

BOOST_AUTO_TEST_SUITE(test_octree)

BOOST_AUTO_TEST_CASE(subcluster_bounds)
{
	rove::octree<rove::aabb<3>, int> tree(
		rove::aabb<3>(rove::vec<3>(-1, -1, -1), rove::vec<3>(1, 1, 1)));

	BOOST_REQUIRE((tree.get_subcluster_bounds(0).lo - rove::vec<3>(-1, -1, -1)).length_sq() < rove::EPSILON);
	BOOST_REQUIRE((tree.get_subcluster_bounds(0).hi - rove::vec<3>(0, 0, 0)).length_sq() < rove::EPSILON);

	BOOST_REQUIRE((tree.get_subcluster_bounds(1).lo - rove::vec<3>(0, -1, -1)).length_sq() < rove::EPSILON);
	BOOST_REQUIRE((tree.get_subcluster_bounds(1).hi - rove::vec<3>(1, 0, 0)).length_sq() < rove::EPSILON);

	BOOST_REQUIRE((tree.get_subcluster_bounds(2).lo - rove::vec<3>(-1, 0, -1)).length_sq() < rove::EPSILON);
	BOOST_REQUIRE((tree.get_subcluster_bounds(2).hi - rove::vec<3>(0, 1, 0)).length_sq() < rove::EPSILON);

	BOOST_REQUIRE((tree.get_subcluster_bounds(3).lo - rove::vec<3>(0, 0, -1)).length_sq() < rove::EPSILON);
	BOOST_REQUIRE((tree.get_subcluster_bounds(3).hi - rove::vec<3>(1, 1, 0)).length_sq() < rove::EPSILON);

	BOOST_REQUIRE((tree.get_subcluster_bounds(4).lo - rove::vec<3>(-1, -1, 0)).length_sq() < rove::EPSILON);
	BOOST_REQUIRE((tree.get_subcluster_bounds(4).hi - rove::vec<3>(0, 0, 1)).length_sq() < rove::EPSILON);

	BOOST_REQUIRE((tree.get_subcluster_bounds(5).lo - rove::vec<3>(0, -1, 0)).length_sq() < rove::EPSILON);
	BOOST_REQUIRE((tree.get_subcluster_bounds(5).hi - rove::vec<3>(1, 0, 1)).length_sq() < rove::EPSILON);

	BOOST_REQUIRE((tree.get_subcluster_bounds(6).lo - rove::vec<3>(-1, 0, 0)).length_sq() < rove::EPSILON);
	BOOST_REQUIRE((tree.get_subcluster_bounds(6).hi - rove::vec<3>(0, 1, 1)).length_sq() < rove::EPSILON);

	BOOST_REQUIRE((tree.get_subcluster_bounds(7).lo - rove::vec<3>(0, 0, 0)).length_sq() < rove::EPSILON);
	BOOST_REQUIRE((tree.get_subcluster_bounds(7).hi - rove::vec<3>(1, 1, 1)).length_sq() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(subcluster_by_point)
{
	rove::octree<rove::aabb<3>, int> tree(
		rove::aabb<3>(rove::vec<3>(-10, -100, -1000), rove::vec<3>(10, 100, 1000)));

	for (size_t i = 0; i < 1000; ++i) {
		rove::vec<3> point(
			rove::random(-10, 10),
			rove::random(-100, 100),
			rove::random(-1000, 1000));

		unsigned index = tree.get_subcluster_by_point(point);
		rove::aabb<3> bounds = tree.get_subcluster_bounds(index);
		BOOST_REQUIRE(bounds.contains(point));
	}
}

BOOST_AUTO_TEST_CASE(subclusters_mask_by_aabb)
{
	rove::octree<rove::aabb<3>, int> tree(
		rove::aabb<3>(rove::vec<3>(-5, -5, -5), rove::vec<3>(5, 5, 5)));

	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(-2, -2, -2), rove::vec<3>(-1, -1, -1))) == 1);
	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(1, -2, -2), rove::vec<3>(2, -1, -1))) == 2);
	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(-2, 1, -2), rove::vec<3>(-1, 2, -1))) == 4);
	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(1, 1, -2), rove::vec<3>(2, 2, -1))) == 8);

	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(-2, -2, 1), rove::vec<3>(-1, -1, 2))) == 16);
	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(1, -2, 1), rove::vec<3>(2, -1, 2))) == 32);
	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(-2, 1, 1), rove::vec<3>(-1, 2, 2))) == 64);
	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(1, 1, 1), rove::vec<3>(2, 2, 2))) == 128);

	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(-2, -2, -2), rove::vec<3>(2, -1, -1))) == (1 | 2));
	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(-2, -2, -2), rove::vec<3>(-1, 2, -1))) == (1 | 4));
	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(1, -2, -2), rove::vec<3>(2, 2, -1))) == (2 | 8));
	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(-2, 1, -2), rove::vec<3>(2, 2, -1))) == (4 | 8));

	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(-2, -2, 1), rove::vec<3>(2, -1, 2))) == (16 | 32));
	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(-2, -2, 1), rove::vec<3>(-1, 2, 2))) == (16 | 64));
	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(1, -2, 1), rove::vec<3>(2, 2, 2))) == (32 | 128));
	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(-2, 1, 1), rove::vec<3>(2, 2, 2))) == (64 | 128));

	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(-2, -2, -2), rove::vec<3>(2, 2, -1))) == (1 | 2 | 4 | 8));
	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(-2, -2, 1), rove::vec<3>(2, 2, 2))) == (16 | 32 | 64 | 128));
	BOOST_REQUIRE(tree.get_subclusters_mask_by_aabb(rove::aabb<3>(rove::vec<3>(-2, -2, -2), rove::vec<3>(2, 2, 2))) == (1 | 2 | 4 | 8 | 16 | 32 | 64 | 128));
}

BOOST_AUTO_TEST_CASE(static_sphere_query_correctness_small)
{
	typedef rove::sphere<3> primitive_t;

	rove::octree<primitive_t, int> tree(
		rove::aabb<3>(rove::vec<3>(-1, -1, -1), rove::vec<3>(1, 1, 1)),
		2);

	std::vector<primitive_t> primitives;
	primitives.reserve(8);

	primitives.push_back(primitive_t(rove::vec<3>(-0.5f, -0.5f, -0.5f), 0.1f));
	primitives.push_back(primitive_t(rove::vec<3>(0.5f, -0.5f, -0.5f), 0.1f));
	primitives.push_back(primitive_t(rove::vec<3>(-0.5f, 0.5f, -0.5f), 0.1f));
	primitives.push_back(primitive_t(rove::vec<3>(0.5f, 0.5f, -0.5f), 0.1f));
	primitives.push_back(primitive_t(rove::vec<3>(-0.5f, -0.5f, 0.5f), 0.1f));
	primitives.push_back(primitive_t(rove::vec<3>(0.5f, -0.5f, 0.5f), 0.1f));
	primitives.push_back(primitive_t(rove::vec<3>(-0.5f, 0.5f, 0.5f), 0.1f));
	primitives.push_back(primitive_t(rove::vec<3>(0.5f, 0.5f, 0.5f), 0.1f));

	for (size_t i = 0; i < primitives.size(); ++i) {
		tree.insert(primitives[i], static_cast<int>(i));
	}

	tree.build();

	for (size_t i = 0; i < 100; ++i) {
		primitive_t query = random_sphere(0.9f);
		std::set<int> expected = brute_force_intersection_set(primitives, query);
		std::set<int> actual = to_set(tree.query_intersection(query));
		BOOST_REQUIRE(actual == expected);
	}
}

BOOST_AUTO_TEST_CASE(static_sphere_query_correctness_random)
{
	typedef rove::sphere<3> primitive_t;
	static size_t const N = 500;
	static size_t const M = 100;

	rove::octree<primitive_t, int> tree(
		rove::aabb<3>(rove::vec<3>(-1, -1, -1), rove::vec<3>(1, 1, 1)),
		8);

	std::vector<primitive_t> primitives;
	primitives.reserve(N);
	tree.reserve(N);

	for (size_t i = 0; i < N; ++i) {
		primitive_t primitive = random_sphere(0.2f);
		primitives.push_back(primitive);
		tree.insert(primitive, static_cast<int>(i));
	}

	tree.build();

	for (size_t i = 0; i < M; ++i) {
		primitive_t query = random_sphere(0.4f);
		std::set<int> expected = brute_force_intersection_set(primitives, query);
		std::set<int> actual = to_set(tree.query_intersection(query));
		BOOST_REQUIRE(actual == expected);
	}
}

BOOST_AUTO_TEST_CASE(static_aabb_query_correctness_random)
{
	typedef rove::aabb<3> primitive_t;
	static size_t const N = 500;
	static size_t const M = 100;

	rove::octree<primitive_t, int> tree(
		rove::aabb<3>(rove::vec<3>(-1, -1, -1), rove::vec<3>(1, 1, 1)),
		8);

	std::vector<primitive_t> primitives;
	primitives.reserve(N);
	tree.reserve(N);

	for (size_t i = 0; i < N; ++i) {
		primitive_t primitive = random_aabb();
		primitives.push_back(primitive);
		tree.insert(primitive, static_cast<int>(i));
	}

	tree.build();

	for (size_t i = 0; i < M; ++i) {
		primitive_t query = random_aabb();
		std::set<int> expected = brute_force_intersection_set(primitives, query);
		std::set<int> actual = to_set(tree.query_intersection(query));
		BOOST_REQUIRE(actual == expected);
	}
}

BOOST_AUTO_TEST_CASE(static_triangle_query_correctness_random)
{
	typedef rove::triangle<3> primitive_t;
	static size_t const N = 200;
	static size_t const M = 80;

	rove::octree<primitive_t, int> tree(
		rove::aabb<3>(rove::vec<3>(-1, -1, -1), rove::vec<3>(1, 1, 1)),
		8);

	std::vector<primitive_t> primitives;
	primitives.reserve(N);
	tree.reserve(N);

	for (size_t i = 0; i < N; ++i) {
		primitive_t primitive = random_triangle();
		primitives.push_back(primitive);
		tree.insert(primitive, static_cast<int>(i));
	}

	tree.build();

	for (size_t i = 0; i < M; ++i) {
		primitive_t query = random_triangle();
		std::set<int> expected = brute_force_intersection_set(primitives, query);
		std::set<int> actual = to_set(tree.query_intersection(query));
		BOOST_REQUIRE(actual == expected);
	}
}

BOOST_AUTO_TEST_CASE(static_triangle_query_by_aabb_correctness_random)
{
	typedef rove::triangle<3> primitive_t;
	typedef rove::aabb<3> query_t;
	static size_t const N = 200;
	static size_t const M = 80;

	rove::octree<primitive_t, int> tree(
		rove::aabb<3>(rove::vec<3>(-1, -1, -1), rove::vec<3>(1, 1, 1)),
		8);

	std::vector<primitive_t> primitives;
	primitives.reserve(N);
	tree.reserve(N);

	for (size_t i = 0; i < N; ++i) {
		primitive_t primitive = random_triangle();
		primitives.push_back(primitive);
		tree.insert(primitive, static_cast<int>(i));
	}

	tree.build();

	for (size_t i = 0; i < M; ++i) {
		query_t query = random_aabb();
		std::set<int> expected = brute_force_intersection_set(primitives, query);
		std::set<int> actual = to_set(tree.query_intersection(query));
		BOOST_REQUIRE(actual == expected);
	}
}

BOOST_AUTO_TEST_CASE(static_triangle_query_by_sphere_correctness_random)
{
	typedef rove::triangle<3> primitive_t;
	typedef rove::sphere<3> query_t;
	static size_t const N = 200;
	static size_t const M = 80;

	rove::octree<primitive_t, int> tree(
		rove::aabb<3>(rove::vec<3>(-1, -1, -1), rove::vec<3>(1, 1, 1)),
		8);

	std::vector<primitive_t> primitives;
	primitives.reserve(N);
	tree.reserve(N);

	for (size_t i = 0; i < N; ++i) {
		primitive_t primitive = random_triangle();
		primitives.push_back(primitive);
		tree.insert(primitive, static_cast<int>(i));
	}

	tree.build();

	for (size_t i = 0; i < M; ++i) {
		query_t query = random_sphere(0.4f);
		std::set<int> expected = brute_force_intersection_set(primitives, query);
		std::set<int> actual = to_set(tree.query_intersection(query));
		BOOST_REQUIRE(actual == expected);
	}
}

BOOST_AUTO_TEST_SUITE_END()
