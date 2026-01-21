/**
 * @file matrix.h
 * @brief Template matrix classes for 3x3 and 4x4 transformations
 *
 * This file provides optimized matrix classes specialized for 3x3 and 4x4 matrices.
 * Includes transformation operations (translation, rotation, scaling), matrix algebra
 * (inverse, transpose, determinant), and vector-matrix multiplication.
 *
 * Matrix storage is row-major, compatible with Direct3D conventions.
 */

#pragma once

#include <assert.h>
#include <iosfwd>
#include "scalar.h"
#include "vec.h"

#if defined(minor)
#undef minor
#endif

namespace rove
{

/// @brief Template matrix class, specialized for 3x3 and 4x4
template<int N,int M,class T = scalar> class matrix;
template<class T> class matrix<3,3,T>;
template<class T> class matrix<4,4,T>;
template<class T> class quaternion;

template<class T> inline void mul(vec<2,T> &r,const vec<2,T> &v,const matrix<3,3,T> &m);
template<class T> inline void mul(vec<3,T> &r,const vec<2,T> &v,const matrix<3,3,T> &m);
template<class T> inline void mul(vec<3,T> &r,const vec<3,T> &v,const matrix<3,3,T> &m);
template<class T> inline void mul(vec<3,T> &r,const vec<3,T> &v,const matrix<4,4,T> &m);
template<class T> inline void mul(vec<4,T> &r,const vec<3,T> &v,const matrix<4,4,T> &m);
template<class T> inline void mul(vec<4,T> &r,const vec<4,T> &v,const matrix<4,4,T> &m);

template<int M,int N,int K,class T> inline void
mul(matrix<M,K,T> &result,const matrix<M,N,T> &left,const matrix<N,K,T> &right);

/**
 * @brief 3x3 matrix class for 2D transformations and 3D rotations
 *
 * Provides transformation operations (translation, rotation, scaling) for 2D graphics
 * and 3D rotation matrices. Includes matrix algebra operations like inverse, transpose,
 * and determinant.
 *
 * Matrix elements can be accessed via:
 * - Named members: _11, _12, ..., _33
 * - 2D array: ij[col][row]
 *
 * Storage is column-major: ij[column][row]
 *
 * @tparam T Scalar type (typically float or double)
 */
template<class T>
class matrix<3,3,T> {
public:
	// types and constants
	static size_t const WIDTH=3,HEIGHT=3;  ///< Matrix dimensions
	typedef T scalar_t;                    ///< Scalar component type
	typedef vec<WIDTH-1,scalar_t> point_t; ///< 2D point type
	typedef vec<WIDTH,scalar_t> vec_t;     ///< 3D vector type
	typedef quaternion<scalar_t> quaternion_t;  ///< Associated quaternion type
	typedef matrix<WIDTH,HEIGHT,scalar_t> matrix_t;  ///< Self type

	/// Matrix element storage
    union {
		struct {
			float _11, _12, _13;  ///< Matrix elements (row-major naming)
            float _21, _22, _23;
            float _31, _32, _33;
        };

		scalar_t ij[WIDTH][HEIGHT];  ///< 2D array access [column][row]
	};

	// Initialization methods
	/// @brief Assign matrix from three row/column vectors
	void assign(vec_t const &l1, vec_t const &l2, vec_t const &l3);

	/// @brief Set all elements to zero
	void zero();

	/// @brief Set to identity matrix
	void identity();

	// Translation operations
	/// @brief Set to translation matrix
	/// @param x X translation
	/// @param y Y translation
	void translation(scalar_t x,scalar_t y);

	/// @brief Set to translation matrix from point
	void translation(const point_t &p);

	/// @brief Apply translation (multiply current matrix by translation)
	void translate(scalar_t x,scalar_t y);

	/// @brief Apply translation from point
	void translate(const point_t &p);

	// Scaling operations
	/// @brief Set to scaling matrix
	void scaling(scalar_t x, scalar_t y);

	/// @brief Set to scaling matrix from point
	void scaling(const point_t &p);

	/// @brief Apply scaling (multiply current matrix by scale)
	void scale(scalar_t x, scalar_t y);

	/// @brief Apply scaling from point
	void scale(const point_t &p);

	// Rotation operations
	/// @brief Set to rotation matrix from Euler angles
	void rotation(scalar_t alpha,scalar_t beta,scalar_t gamma);

	/// @brief Set to rotation matrix from rotation vector
	void rotation(const vec_t &v);

	/// @brief Set to rotation matrix from quaternion
	void rotation(const quaternion_t &q);

	/// @brief Set to rotation matrix from axis and precomputed sin/cos
	void rotation(vec_t const &axis, scalar_t sine, scalar_t cosine);

	/// @brief Set to rotation matrix from axis and angle
	void rotation(vec_t const &axis, scalar_t angle);

	/// @brief Apply rotation from Euler angles
	void rotate(scalar_t alpha,scalar_t beta,scalar_t gamma);

	/// @brief Apply rotation from rotation vector
	void rotate(const vec_t &v);

	/// @brief Apply rotation from quaternion
	void rotate(const quaternion_t &q);

	/// @brief Apply rotation from axis and precomputed sin/cos
	void rotate(vec_t const &axis, scalar_t sine, scalar_t cosine);

	/// @brief Apply rotation from axis and angle
	void rotate(vec_t const &axis, scalar_t angle);

	/// @brief Set to rotation matrix that aligns with direction
	void direction(const vec_t &dir);

	/// @brief Apply direction alignment
	void direct(const vec_t &dir);

	// Coordinate system transformations
	/// @brief Transform from world coordinates to local tangent space
	bool world_to_axes(vec_t const &tangent, vec_t const &normal, vec_t const &binormal);

	/// @brief Transform from local tangent space to world coordinates
	bool axes_to_world(vec_t const &tangent, vec_t const &normal, vec_t const &binormal);

	/**
	 * @brief Compute 2x2 minor determinant
	 * @param r0 First column index
	 * @param r1 Second column index
	 * @param c0 First row index
	 * @param c1 Second row index
	 * @return Determinant of 2x2 submatrix
	 */
	scalar_t minor(int r0,int r1,int c0,int c1) const {
		return ij[r0][c0] * ij[r1][c1] - ij[r1][c0] * ij[r0][c1];
	}

	/**
	 * @brief Compute adjoint (adjugate) matrix
	 * @param[out] M Adjoint matrix result
	 */
	void adjoint(matrix_t &M) const {
		M.ij[0][0] =  minor( 1, 2, 1, 2);
		M.ij[0][1] = -minor( 0, 2, 1, 2);
		M.ij[0][2] =  minor( 0, 1, 1, 2);
		M.ij[1][0] = -minor( 1, 2, 0, 2);
		M.ij[1][1] =  minor( 0, 2, 0, 2);
		M.ij[1][2] = -minor( 0, 1, 0, 2);
		M.ij[2][0] =  minor( 1, 2, 0, 1);
		M.ij[2][1] = -minor( 0, 2, 0, 1);
		M.ij[2][2] =  minor( 0, 1, 0, 1);
	}

	/**
	 * @brief Compute matrix determinant
	 * @return Determinant value (zero for singular matrix)
	 */
	scalar_t determinant() const {
		return 	ij[0][0] * minor(1, 2, 1, 2) -
			ij[0][1] * minor(1, 2, 0, 2) +
			ij[0][2] * minor(1, 2, 0, 1);
	}

	/**
	 * @brief Compute matrix trace (sum of diagonal elements)
	 * @return Sum of diagonal elements
	 */
	scalar_t trace() const {
		return ij[0][0] + ij[1][1] + ij[2][2];
	}

	/**
	 * @brief Compute inverse matrix
	 * @param[out] M Inverse matrix result
	 * @return false if matrix is singular (determinant is zero)
	 */
	bool inverse(matrix_t &M) const {
		scalar_t det = determinant();
		if(det == 0) return false;
		adjoint(M);
		M /= det;
		return true;
	}

	/**
	 * @brief Invert matrix in place
	 * @return false if matrix is singular
	 */
	bool invert() {
		return matrix_t(*this).inverse(*this);
	}

	/**
	 * @brief Compute transpose matrix
	 * @param[out] tp Transposed matrix result
	 */
	void transpose(matrix_t &tp) const {
        tp.ij[0][0] = ij[0][0];		tp.ij[0][1] = ij[1][0];		tp.ij[0][2] = ij[2][0];
		tp.ij[1][0] = ij[0][1];		tp.ij[1][1] = ij[1][1];		tp.ij[1][2] = ij[2][1];
		tp.ij[2][0] = ij[0][2];		tp.ij[2][1] = ij[1][2];		tp.ij[2][2] = ij[2][2];
	}

	matrix_t &operator *=(scalar_t k) {
		ij[0][0]*=k;	ij[0][1]*=k;	ij[0][2]*=k;
		ij[1][0]*=k;	ij[1][1]*=k;	ij[1][2]*=k;
		ij[2][0]*=k;	ij[2][1]*=k;	ij[2][2]*=k;
		return *this;
	}

	matrix_t &operator *=(const matrix_t &m) {
		matrix_t n=*this;
		mul(*this,n,m);
		return *this;
	}

	matrix_t &operator /=(scalar_t k) {
		return *this *= 1/k;
	}

	template<class Archive>
	void serialize(Archive &archive, unsigned const /*file_version*/)
	{
		archive & ij;
	}
};

/**
 * @brief 4x4 matrix class for 3D transformations
 *
 * Provides full 3D transformation capabilities including translation, rotation,
 * scaling, perspective projection, and view transformations. Compatible with
 * Direct3D matrix layout (D3DMATRIX).
 *
 * Matrix elements can be accessed via:
 * - Named members: _11, _12, ..., _44
 * - 2D array: ij[col][row]
 *
 * Storage is column-major: ij[column][row]
 *
 * @tparam T Scalar type (typically float or double)
 */
template<class T>
class matrix<4,4,T> {
public:
	// types and constants
	static size_t const WIDTH=4,HEIGHT=4;  ///< Matrix dimensions
	typedef T scalar_t;                    ///< Scalar component type
	typedef vec<WIDTH,scalar_t> vec_t;     ///< 4D vector type
	typedef vec<WIDTH-1,scalar_t> point_t; ///< 3D point type
	typedef quaternion<scalar_t> quaternion_t;  ///< Associated quaternion type
	typedef matrix<WIDTH,HEIGHT,scalar_t> matrix_t;  ///< Self type

	/// Matrix element storage
	union {
		struct {
			scalar_t _11, _12, _13, _14;  ///< Matrix elements (row-major naming)
            scalar_t _21, _22, _23, _24;
            scalar_t _31, _32, _33, _34;
            scalar_t _41, _42, _43, _44;
        };

		scalar_t ij[WIDTH][HEIGHT];  ///< 2D array access [column][row]
	};

	/// Default constructor (uninitialized)
	matrix() {
	}

	/// @brief Set to identity matrix
	void identity();

	/// @brief Set all elements to zero
	void zero();

	// Translation operations
	/// @brief Set to translation matrix
	void translation(scalar_t x,scalar_t y,scalar_t z);

	/// @brief Set to translation matrix from 3D point
	void translation(const point_t &p);

	/// @brief Apply translation (multiply current matrix by translation)
	void translate(scalar_t x,scalar_t y,scalar_t z);

	/// @brief Apply translation from 3D point
	void translate(const point_t &p);

	// Scaling operations
	/// @brief Set to scaling matrix
	void scaling(scalar_t sx,scalar_t sy,scalar_t sz);

	/// @brief Set to scaling matrix from 3D point
	void scaling(const point_t &s);

	/// @brief Apply scaling (multiply current matrix by scale)
	void scale(scalar_t sx,scalar_t sy,scalar_t sz);

	/// @brief Apply scaling from 3D point
	void scale(const point_t &s);

	// Rotation operations
	/// @brief Set to rotation matrix from Euler angles
	void rotation(scalar_t alpha,scalar_t beta,scalar_t gamma);

	/// @brief Set to rotation matrix from rotation vector
	void rotation(const point_t &v);

	/// @brief Set to rotation matrix from quaternion
	void rotation(const quaternion_t &q);

	/// @brief Set to rotation matrix from axis and precomputed sin/cos
	void rotation(point_t const &axis, scalar_t sine, scalar_t cosine);

	/// @brief Set to rotation matrix from axis and angle
	void rotation(point_t const &axis, scalar_t angle);

	/// @brief Apply rotation from Euler angles
	void rotate(scalar_t alpha,scalar_t beta,scalar_t gamma);

	/// @brief Apply rotation from rotation vector
	void rotate(const point_t &v);

	/// @brief Apply rotation from quaternion
	void rotate(const quaternion_t &q);

	/// @brief Apply rotation from axis and precomputed sin/cos
	void rotate(point_t const &axis, scalar_t sine, scalar_t cosine);

	/// @brief Apply rotation from axis and angle
  	void rotate(point_t const &axis, scalar_t angle);

	/// @brief Set to rotation matrix that aligns with direction
	void direction(const point_t &dir);

	/// @brief Apply direction alignment
	void direct(const point_t &dir);

	/**
	 * @brief Create view (camera) matrix using look-at parameters
	 * @param eye Camera position
	 * @param at Target position to look at
	 * @param up Up direction vector
	 */
	void lookat(const point_t &eye,const point_t &at,const point_t &up);

	/**
	 * @brief Create orthographic projection matrix
	 * @param width View width
	 * @param height View height
	 * @param z_near Near clipping plane
	 * @param z_far Far clipping plane
	 */
	void ortho(scalar_t width,scalar_t height,scalar_t z_near,scalar_t z_far);

	/**
	 * @brief Create perspective projection matrix
	 * @param fovy Field of view angle (Y axis) in radians
	 * @param aspect Aspect ratio (width/height)
	 * @param z_near Near clipping plane
	 * @param z_far Far clipping plane
	 */
	void perspective(scalar_t fovy,scalar_t aspect,scalar_t z_near,scalar_t z_far);

	/**
	 * @brief Compute 3x3 minor determinant
	 * @param r0 First column index
	 * @param r1 Second column index
	 * @param r2 Third column index
	 * @param c0 First row index
	 * @param c1 Second row index
	 * @param c2 Third row index
	 * @return Determinant of 3x3 submatrix
	 */
	scalar_t minor(int r0,int r1,int r2,int c0,int c1,int c2) const {
		return
			ij[r0][c0] * (ij[r1][c1] * ij[r2][c2] - ij[r2][c1] * ij[r1][c2]) -
			ij[r0][c1] * (ij[r1][c0] * ij[r2][c2] - ij[r2][c0] * ij[r1][c2]) +
			ij[r0][c2] * (ij[r1][c0] * ij[r2][c1] - ij[r2][c0] * ij[r1][c1]);
	}

	/**
	 * @brief Compute adjoint (adjugate) matrix
	 * @param[out] M Adjoint matrix result
	 */
	void adjoint(matrix_t &M) const {
		M.ij[0][0] =  minor( 1, 2, 3, 1, 2, 3);
		M.ij[0][1] = -minor( 0, 2, 3, 1, 2, 3);
		M.ij[0][2] =  minor( 0, 1, 3, 1, 2, 3);
		M.ij[0][3] = -minor( 0, 1, 2, 1, 2, 3);
		M.ij[1][0] = -minor( 1, 2, 3, 0, 2, 3);
		M.ij[1][1] =  minor( 0, 2, 3, 0, 2, 3);
		M.ij[1][2] = -minor( 0, 1, 3, 0, 2, 3);
		M.ij[1][3] =  minor( 0, 1, 2, 0, 2, 3);
		M.ij[2][0] =  minor( 1, 2, 3, 0, 1, 3);
		M.ij[2][1] = -minor( 0, 2, 3, 0, 1, 3);
		M.ij[2][2] =  minor( 0, 1, 3, 0, 1, 3);
		M.ij[2][3] = -minor( 0, 1, 2, 0, 1, 3);
		M.ij[3][0] = -minor( 1, 2, 3, 0, 1, 2);
		M.ij[3][1] =  minor( 0, 2, 3, 0, 1, 2);
		M.ij[3][2] = -minor( 0, 1, 3, 0, 1, 2);
		M.ij[3][3] =  minor( 0, 1, 2, 0, 1, 2);
	}

	/**
	 * @brief Compute matrix determinant
	 * @return Determinant value (zero for singular matrix)
	 */
	scalar_t determinant() const {
		return
			ij[0][0] * minor(1, 2, 3, 1, 2, 3) -
			ij[0][1] * minor(1, 2, 3, 0, 2, 3) +
			ij[0][2] * minor(1, 2, 3, 0, 1, 3) -
			ij[0][3] * minor(1, 2, 3, 0, 1, 2);
	}

	/**
	 * @brief Compute inverse matrix
	 * @param[out] M Inverse matrix result
	 * @return false if matrix is singular (determinant is zero)
	 */
	bool inverse(matrix_t &M) const {
		scalar_t det = determinant();
		if(det == 0) return false;
		adjoint(M);
		M /= det;
		return true;
	}

	/**
	 * @brief Invert matrix in place
	 * @return false if matrix is singular
	 */
	bool invert() {
		return matrix_t(*this).inverse(*this);
	}

	/**
	 * @brief Compute transpose matrix
	 * @param[out] tp Transposed matrix result
	 */
	void transpose(matrix_t &tp) const {
       	tp.ij[0][0] = ij[0][0];	tp.ij[0][1] = ij[1][0];	tp.ij[0][2] = ij[2][0]; tp.ij[0][3] = ij[3][0];
		tp.ij[1][0] = ij[0][1];	tp.ij[1][1] = ij[1][1];	tp.ij[1][2] = ij[2][1]; tp.ij[1][3] = ij[3][1];
		tp.ij[2][0] = ij[0][2];	tp.ij[2][1] = ij[1][2];	tp.ij[2][2] = ij[2][2]; tp.ij[2][3] = ij[3][2];
		tp.ij[3][0] = ij[0][3];	tp.ij[3][1] = ij[1][3];	tp.ij[3][2] = ij[2][3]; tp.ij[3][3] = ij[3][3];
	}

	/// @brief Transpose matrix in place
	void transpose_inplace() {
		matrix_t tp;
		tp = *this;
		tp.transpose(*this);
	}

	matrix_t &operator *=(scalar_t k) {
		ij[0][0]*=k;	ij[0][1]*=k;	ij[0][2]*=k;	ij[0][3]*=k;
		ij[1][0]*=k;	ij[1][1]*=k;	ij[1][2]*=k;	ij[1][3]*=k;
		ij[2][0]*=k;	ij[2][1]*=k;	ij[2][2]*=k;	ij[2][3]*=k;
		ij[3][0]*=k;	ij[3][1]*=k;	ij[3][2]*=k;	ij[3][3]*=k;
		return *this;
	}

	matrix_t &operator /=(scalar_t k) {
		return *this *= 1/k;
	}

	matrix_t &operator *=(const matrix_t &m) {
		matrix_t n=*this;
		mul(*this,n,m);
		return *this;
	}

	/**
	 * @brief Extract 3D column vector (drops w component)
	 * @param column_index Column index (0-3)
	 * @return 3D vector from column
	 */
	point_t projected_column(size_t column_index) const {
		return point_t(ij[column_index][0], ij[column_index][1], ij[column_index][2]);
	}

	/**
	 * @brief Extract 4D column vector
	 * @param column_index Column index (0-3)
	 * @return 4D vector from column
	 */
	vec_t column(size_t column_index) const {
		return vec_t(ij[column_index][0], ij[column_index][1], ij[column_index][2],
			ij[column_index][3]);
	}

	/**
	 * @brief Extract 3D row vector (drops w component)
	 * @param row_index Row index (0-3)
	 * @return 3D vector from row
	 */
	point_t projected_row(size_t row_index) const {
		return point_t(ij[0][row_index], ij[1][row_index], ij[2][row_index]);
	}

	/**
	 * @brief Extract 4D row vector
	 * @param row_index Row index (0-3)
	 * @return 4D vector from row
	 */
	vec_t row(size_t row_index) const {
		return vec_t(ij[0][row_index], ij[1][row_index], ij[2][row_index],
			ij[3][row_index]);
	}

	template<class Archive>
	void serialize(Archive &archive, unsigned const /*file_version*/)
	{
		archive & ij;
	}
};

template<class T> void
mul(vec<2,T> &result,const vec<2,T> &v,const matrix<3,3,T> &m) {
	vec<3,T> r;
	mul(r,v,m);
	result.x = r.x/r.z;
	result.y = r.y/r.z;
}

template<class T> void
mul(vec<3,T> &result,const vec<2,T> &v,const matrix<3,3,T> &m) {
	assert((void*)&result != (void*)&v);

	result.x=m.ij[0][0]*v.x+m.ij[1][0]*v.y+m.ij[2][0];
	result.y=m.ij[0][1]*v.x+m.ij[1][1]*v.y+m.ij[2][1];
	result.z=m.ij[0][2]*v.x+m.ij[1][2]*v.y+m.ij[2][2];
}

template<class T> void
mul(vec<3,T> &result,const vec<3,T> &v,const matrix<3,3,T> &m) {
	assert((void*)&result != (void*)&v);

	result.x=m.ij[0][0]*v.x+m.ij[1][0]*v.y+m.ij[2][0]*v.z;
	result.y=m.ij[0][1]*v.x+m.ij[1][1]*v.y+m.ij[2][1]*v.z;
	result.z=m.ij[0][2]*v.x+m.ij[1][2]*v.y+m.ij[2][2]*v.z;
}

template<class T> void
mul(vec<3,T> &result,const vec<3,T> &v,const matrix<4,4,T> &m) {
	assert((void*)&result != (void*)&v);

	result.x=m.ij[0][0]*v.x+m.ij[1][0]*v.y+m.ij[2][0]*v.z+m.ij[3][0];
	result.y=m.ij[0][1]*v.x+m.ij[1][1]*v.y+m.ij[2][1]*v.z+m.ij[3][1];
	result.z=m.ij[0][2]*v.x+m.ij[1][2]*v.y+m.ij[2][2]*v.z+m.ij[3][2];
}

template<class T> void
mul(vec<4,T> &result,const vec<3,T> &v,const matrix<4,4,T> &m) {
	assert((void*)&result != (void*)&v);

	result.x=m.ij[0][0]*v.x+m.ij[1][0]*v.y+m.ij[2][0]*v.z+m.ij[3][0];
	result.y=m.ij[0][1]*v.x+m.ij[1][1]*v.y+m.ij[2][1]*v.z+m.ij[3][1];
	result.z=m.ij[0][2]*v.x+m.ij[1][2]*v.y+m.ij[2][2]*v.z+m.ij[3][2];
	result.w=m.ij[0][3]*v.x+m.ij[1][3]*v.y+m.ij[2][3]*v.z+m.ij[3][3];
}

template<class T> void
mul(vec<4,T> &result,const vec<4,T> &v,const matrix<4,4,T> &m) {
	assert((void*)&result != (void*)&v);

	result.x=m.ij[0][0]*v.x+m.ij[1][0]*v.y+m.ij[2][0]*v.z+m.ij[3][0]*v.w;
	result.y=m.ij[0][1]*v.x+m.ij[1][1]*v.y+m.ij[2][1]*v.z+m.ij[3][1]*v.w;
	result.z=m.ij[0][2]*v.x+m.ij[1][2]*v.y+m.ij[2][2]*v.z+m.ij[3][2]*v.w;
	result.w=m.ij[0][3]*v.x+m.ij[1][3]*v.y+m.ij[2][3]*v.z+m.ij[3][3]*v.w;
}

/**
 * @brief Multiply two matrices: result = left × right
 * @param[out] result Result matrix (must not alias left or right)
 * @param left Left matrix
 * @param right Right matrix
 * @tparam M Number of rows in left matrix
 * @tparam N Number of columns in left / rows in right
 * @tparam K Number of columns in right matrix
 */
template<int M,int N,int K,class T> void
mul(matrix<M,K,T> &result,const matrix<M,N,T> &left,const matrix<N,K,T> &right) {
	assert(&result != &left);
	assert(&result != &right);

	int i,j,k;
	for(i=0; i<M; i++) {
		for(j=0; j<K; j++) {
			result.ij[i][j]=left.ij[i][0]*right.ij[0][j];
			for(k=1; k<N; k++) result.ij[i][j]+=left.ij[i][k]*right.ij[k][j];
		}
	}
}

/**
 * @brief Test matrix equality within epsilon tolerance
 * @param lhs Left matrix
 * @param rhs Right matrix
 * @param epsilon Tolerance for floating point comparison
 * @return true if all elements differ by less than epsilon
 */
template <int M, int N, class T> inline bool
equal(matrix<M, N, T> const &lhs, matrix<M, N, T> const &rhs, T epsilon = EPSILON) {
	for(size_t i = 0; i < M; i++) {
		for(size_t j = 0; j < N; j++) {
			if (abs(lhs.ij[i][j] - rhs.ij[i][j]) > epsilon) return false;
		}
	}

	return true;
}

}

template<int M,int N,class T> inline rove::vec<N-1,T>
operator *(const rove::vec<N-1,T> &left, const rove::matrix<M,N,T> &right) {
	rove::vec<N-1,T> result;
	mul(result,left,right);
	return result;
}

template<int M,int N,class T> inline rove::vec<N,T>
operator *(const rove::vec<N,T> &left, const rove::matrix<M,N,T> &right) {
	rove::vec<N,T> result;
	mul(result,left,right);
	return result;
}

template<int M,int N,int K,class T> inline rove::matrix<N,K,T>
operator *(const rove::matrix<M,N,T> &left,const rove::matrix<N,K,T> &right) {
	rove::matrix<N,K,T> result;
	mul(result,left,right);
	return result;
}

template<int M,int N,class T> inline rove::vec<N,T> &
operator *=(rove::vec<N,T> &left, const rove::matrix<M,N,T> &right) {
	mul(left,rove::vec<N,T>(left),right);
	return left;
}

