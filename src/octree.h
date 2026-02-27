/**
 * @file octree.h
 * @brief Static 3D octree for broad-phase spatial queries
 */

#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <memory>
#include <vector>
#include "aabb.h"
#include "sphere.h"

namespace rove
{

template<class Primitive>
struct octree_traits;

template<class T>
struct octree_traits<aabb<3, T> > {
	typedef T scalar_t;
	typedef vec<3, T> vec_t;
	typedef aabb<3, T> aabb_t;
	typedef aabb<3, T> primitive_t;

	static aabb_t bounds(primitive_t const &primitive)
	{
		return primitive;
	}

	static bool intersects(primitive_t const &left, primitive_t const &right)
	{
		return left.test_intersection(right);
	}
};

template<class T>
struct octree_traits<sphere<3, T> > {
	typedef T scalar_t;
	typedef vec<3, T> vec_t;
	typedef aabb<3, T> aabb_t;
	typedef sphere<3, T> primitive_t;

	static aabb_t bounds(primitive_t const &primitive)
	{
		return primitive.get_aabb();
	}

	static bool intersects(primitive_t const &left, primitive_t const &right)
	{
		return left.test_intersection(right);
	}
};

/**
 * @brief Static octree for primitives with AABB bounds and intersection tests
 *
 * The tree is "static": call `insert(...)` to provide items, then `build()` once.
 * Queries do not mutate the tree.
 */
template<class Primitive, class Payload, class Traits = octree_traits<Primitive> >
class octree
{
public:
	typedef Primitive primitive_t;
	typedef Payload payload_t;
	typedef Traits traits_t;
	typedef typename traits_t::scalar_t scalar_t;
	typedef typename traits_t::vec_t vec_t;
	typedef typename traits_t::aabb_t aabb_t;

	struct value_type
	{
		primitive_t primitive;
		payload_t payload;
	};

	static constexpr size_t SUBCLUSTERS_COUNT = 8;
	static constexpr unsigned X_MASK = 1;
	static constexpr unsigned Y_MASK = 2;
	static constexpr unsigned Z_MASK = 4;

	explicit octree(
		aabb_t const &bounds,
		size_t leaf_items_threshold = 8,
		scalar_t min_subcluster_diagonal_sq = scalar_t(1)):
		bounds_(bounds),
		centre_(bounds.centre()),
		leaf_items_threshold_(leaf_items_threshold),
		min_subcluster_diagonal_sq_(min_subcluster_diagonal_sq),
		built_(false)
	{
		assert(bounds.lo <= bounds.hi);
	}

	void clear()
	{
		items_.clear();
		root_.reset();
		built_ = false;
	}

	void reserve(size_t n)
	{
		items_.reserve(n);
	}

	void insert(primitive_t const &primitive, payload_t const &payload)
	{
		items_.push_back(value_type{primitive, payload});
		built_ = false;
	}

	size_t size() const
	{
		return items_.size();
	}

	void build()
	{
		root_.reset(new node(bounds_));

		std::vector<size_t> indices(items_.size());
		for (size_t i = 0; i < items_.size(); ++i) {
			indices[i] = i;
		}

		build_node(*root_, indices);
		built_ = true;
	}

	bool is_leaf() const
	{
		return root_ == nullptr || root_->is_leaf();
	}

	aabb_t get_subcluster_bounds(unsigned index) const
	{
		return get_subcluster_bounds(bounds_, index);
	}

	unsigned get_subcluster_by_point(vec_t const &point) const
	{
		return get_subcluster_by_point(centre_, point);
	}

	size_t get_subclusters_by_aabb(aabb_t const &bounds, unsigned subclusters[SUBCLUSTERS_COUNT]) const
	{
		return get_subclusters_by_aabb(centre_, bounds, subclusters);
	}

	unsigned get_subclusters_mask_by_aabb(aabb_t const &bounds) const
	{
		unsigned result = 0;
		unsigned subclusters[SUBCLUSTERS_COUNT];
		size_t nsubclusters = get_subclusters_by_aabb(bounds, subclusters);

		for (size_t i = 0; i < nsubclusters; ++i) {
			result |= 1u << subclusters[i];
		}

		return result;
	}

	std::vector<payload_t> query_intersection(primitive_t const &primitive) const
	{
		std::vector<payload_t> result;
		query_intersection(primitive, result);
		return result;
	}

	void query_intersection(primitive_t const &primitive, std::vector<payload_t> &result) const
	{
		assert(built_ && "octree::build() must be called before query_intersection()");
		if (!built_ || root_ == nullptr) {
			return;
		}

		query_node(*root_, primitive, traits_t::bounds(primitive), result);
	}

private:
	struct node
	{
		explicit node(aabb_t const &node_bounds):
			bounds(node_bounds),
			centre(node_bounds.centre())
		{
		}

		bool is_leaf() const
		{
			for (size_t i = 0; i < SUBCLUSTERS_COUNT; ++i) {
				if (children[i] != nullptr) {
					return false;
				}
			}

			return true;
		}

		aabb_t bounds;
		vec_t centre;
		std::vector<size_t> item_indices;
		std::array<std::unique_ptr<node>, SUBCLUSTERS_COUNT> children;
	};

	static aabb_t get_subcluster_bounds(aabb_t const &bounds, unsigned index)
	{
		vec_t origin = bounds.lo;
		vec_t extents = bounds.diagonal() / scalar_t(2);

		if (index & X_MASK) origin.x += extents.x;
		if (index & Y_MASK) origin.y += extents.y;
		if (index & Z_MASK) origin.z += extents.z;

		return aabb_t(origin, origin + extents);
	}

	static unsigned get_subcluster_by_point(vec_t const &centre, vec_t const &point)
	{
		unsigned index = 0;

		if (point.x >= centre.x) index |= X_MASK;
		if (point.y >= centre.y) index |= Y_MASK;
		if (point.z >= centre.z) index |= Z_MASK;

		return index;
	}

	static size_t get_subclusters_by_aabb(vec_t const &centre, aabb_t const &bounds, unsigned subclusters[SUBCLUSTERS_COUNT])
	{
		assert(bounds.lo <= bounds.hi);

		unsigned c1 = get_subcluster_by_point(centre, bounds.lo);
		unsigned c2 = get_subcluster_by_point(centre, bounds.hi);

		if (c1 == c2) {
			subclusters[0] = c1;
			return 1;
		}

		if (c1 == 0 && c2 == SUBCLUSTERS_COUNT - 1) {
			for (size_t i = 0; i < SUBCLUSTERS_COUNT; ++i) {
				subclusters[i] = static_cast<unsigned>(i);
			}
			return SUBCLUSTERS_COUNT;
		}

		unsigned diff = c1 ^ c2;
		unsigned hi = diff & (diff - 1);
		unsigned lo = diff & ~hi;

		assert((c1 | lo | hi) == c2);

		if (hi) {
			subclusters[0] = c1;
			subclusters[1] = c1 | lo;
			subclusters[2] = c1 | hi;
			subclusters[3] = c2;
			return 4;
		}

		subclusters[0] = c1;
		subclusters[1] = c2;
		return 2;
	}

	void build_node(node &current, std::vector<size_t> const &indices)
	{
		current.item_indices.clear();

		if (indices.empty()) {
			return;
		}

		if (indices.size() < leaf_items_threshold_ ||
			current.bounds.diagonal().length_sq() < min_subcluster_diagonal_sq_)
		{
			current.item_indices = indices;
			return;
		}

		std::array<std::vector<size_t>, SUBCLUSTERS_COUNT> child_indices;

		for (size_t item_index : indices) {
			aabb_t primitive_bounds = traits_t::bounds(items_[item_index].primitive);
			unsigned subclusters[SUBCLUSTERS_COUNT];
			size_t nsubclusters = get_subclusters_by_aabb(current.centre, primitive_bounds, subclusters);

			// Keep spanning primitives in the current node to avoid duplicate payloads.
			if (nsubclusters != 1) {
				current.item_indices.push_back(item_index);
			} else {
				child_indices[subclusters[0]].push_back(item_index);
			}
		}

		bool have_children = false;
		for (size_t i = 0; i < SUBCLUSTERS_COUNT; ++i) {
			if (!child_indices[i].empty()) {
				have_children = true;
				break;
			}
		}

		if (!have_children) {
			return;
		}

		for (size_t i = 0; i < SUBCLUSTERS_COUNT; ++i) {
			if (child_indices[i].empty()) {
				continue;
			}

			current.children[i].reset(new node(get_subcluster_bounds(current.bounds, static_cast<unsigned>(i))));
			build_node(*current.children[i], child_indices[i]);
		}
	}

	void query_node(node const &current, primitive_t const &query_primitive, aabb_t const &query_bounds, std::vector<payload_t> &result) const
	{
		for (size_t item_index : current.item_indices) {
			if (traits_t::intersects(items_[item_index].primitive, query_primitive)) {
				result.push_back(items_[item_index].payload);
			}
		}

		if (current.is_leaf()) {
			return;
		}

		unsigned subclusters[SUBCLUSTERS_COUNT];
		size_t nsubclusters = get_subclusters_by_aabb(current.centre, query_bounds, subclusters);

		for (size_t i = 0; i < nsubclusters; ++i) {
			node const *child = current.children[subclusters[i]].get();
			if (child != nullptr) {
				query_node(*child, query_primitive, query_bounds, result);
			}
		}
	}

	aabb_t bounds_;
	vec_t centre_;
	size_t leaf_items_threshold_;
	scalar_t min_subcluster_diagonal_sq_;
	std::vector<value_type> items_;
	std::unique_ptr<node> root_;
	bool built_;
};

}
