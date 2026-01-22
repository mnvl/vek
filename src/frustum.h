#pragma once

#include <boost/array.hpp>
#include "vec.h"
#include "plane.h"
#include "matrix.h"

namespace rove
{

template<int N, class T> class aabb;
template<int N, class T> class obb;

// this class is specialized for frustum culling
template<class T = scalar>
class frustum {
public:
	typedef T scalar_t;
	static size_t const ARITY = 3;
	static size_t const PLANES_COUNT = 6;

	typedef vec<ARITY, scalar_t> vec_t;
	typedef aabb<ARITY, scalar_t> aabb_t;
	typedef obb<ARITY, scalar_t> obb_t;
	typedef plane<scalar_t> plane_t;
	typedef matrix<4,4,scalar_t> matrix_t;

	enum {
		PLANE_LEFT,
		PLANE_RIGHT,
		PLANE_TOP,
		PLANE_BOTTOM,
		PLANE_NEAR,
		PLANE_FAR,
	};

	plane_t planes[PLANES_COUNT];

	frustum();
	frustum(matrix_t const &tf);
	~frustum();

	void load(matrix_t const &tf);

	bool contains(vec_t const &point) const;
	bool test_intersection(aabb_t const &bounds) const;
	bool test_intersection(obb_t const &bounds) const;
};

}
