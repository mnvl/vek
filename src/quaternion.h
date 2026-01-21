/**
 * @file quaternion.h
 * @brief Quaternion class for 3D rotations
 *
 * This file provides a quaternion class for representing and interpolating
 * 3D rotations. Quaternions avoid gimbal lock and provide smooth interpolation
 * via spherical linear interpolation (SLERP).
 *
 * A quaternion represents a rotation as q = xi + yj + zk + w, where w is the
 * scalar (real) part and (x,y,z) is the vector (imaginary) part.
 */

#pragma once

#include <algorithm>
#include "scalar.h"
#include "vec.h"
#include "matrix.h"

namespace rove
{

/**
 * @brief Quaternion class for 3D rotations
 *
 * Represents a rotation in 3D space. A unit quaternion (norm = 1) represents
 * a pure rotation. Non-unit quaternions can represent scaled rotations.
 *
 * Components: (x, y, z) = vector part, w = scalar part
 *
 * @tparam T Scalar type (typically float or double)
 */
template<class T = scalar>
class quaternion {
public:
    T x, y, z, w;  ///< Quaternion components (vector part x,y,z and scalar part w)

	/// Default constructor (uninitialized)
	quaternion() {
	}

	/// Component constructor
	/// @param x0 X component (i coefficient)
	/// @param y0 Y component (j coefficient)
	/// @param z0 Z component (k coefficient)
	/// @param w0 W component (scalar/real part)
	quaternion(T x0, T y0, T z0, T w0) {
        x = x0; y = y0;	z = z0;	w = w0;
	}

	/// @brief Set to identity quaternion (no rotation)
	/// Identity quaternion is (0, 0, 0, 1)
    void identity() {
       	x = 0; y = 0; z = 0; w = 1;
    }

	/// @brief Negate all components
	/// @return Negated quaternion
	quaternion operator -() const {
		return quaternion<T>(-x, -y, -z, -w);
	}

	/// @brief Scale all components by scalar
	/// @param s Scale factor
	void scale(T s) {
		x *= s; y *= s; z *= s; w *= s;
    }

	/// @brief Multiply quaternion by scalar
	/// @param s Scale factor
	/// @return Scaled quaternion
	quaternion operator *(T s) const {
		quaternion<T> result(*this);
		result.scale(s);
		return result;
	}

	/// @brief Compute quaternion magnitude (length)
	/// @return √(x² + y² + z² + w²)
    T norm() const {
       	return (T) sqrt(x*x + y*y + z*z + w*w);
    }

	/// @brief Normalize quaternion to unit length in place
	/// Sets to identity if norm is below EPSILON
   	void normalize() {
		T n = norm();
		if(n < EPSILON) {
			identity();
		} else {
			scale(1/n);
		}
	}

	/// @brief Return normalized copy of quaternion
	/// @return Unit quaternion in same orientation
	quaternion normalized() const {
		quaternion q(*this);
		q.normalize();
		return q;
	}

	/**
	 * @brief Extract unit quaternion from rotation matrix
	 *
	 * Converts a rotation matrix to its quaternion representation.
	 * Uses numerically stable algorithm with clamping.
	 *
	 * @param m Rotation matrix (3x3 or 4x4)
	 */
	template<int N, int M, class X>
	void set_unit(matrix<N, M, X> const &m) {
		// Clamp to avoid negative values due to floating point precision issues
		T x_sq = std::max(T(0), T(1 + m.ij[0][0] - m.ij[1][1] - m.ij[2][2]));
		T y_sq = std::max(T(0), T(1 - m.ij[0][0] + m.ij[1][1] - m.ij[2][2]));
		T z_sq = std::max(T(0), T(1 - m.ij[0][0] - m.ij[1][1] + m.ij[2][2]));
		T w_sq = std::max(T(0), T(m.ij[0][0] + m.ij[1][1] + m.ij[2][2] + 1));

		x = sign(m.ij[1][2] - m.ij[2][1]) * T(sqrt(x_sq)) / 2;
		y = sign(m.ij[2][0] - m.ij[0][2]) * T(sqrt(y_sq)) / 2;
		z = sign(m.ij[0][1] - m.ij[1][0]) * T(sqrt(z_sq)) / 2;
		w = T(sqrt(w_sq)) / 2;
	}
};

/**
 * @brief Compute quaternion dot product
 * @param p First quaternion
 * @param q Second quaternion
 * @return Scalar dot product (p·q)
 */
template<class T> inline T
dot_product(const quaternion<T> &p, const quaternion<T> &q) {
	return p.x*q.x + p.y*q.y + p.z*q.z + p.w*q.w;
}

/// @brief Add two quaternions component-wise
template<class T> inline quaternion<T>
operator +(const quaternion<T> &p, const quaternion<T> &q) {
	return quaternion<T>(p.x + q.x, p.y + q.y, p.z + q.z, p.w + q.w);
}

/// @brief Subtract two quaternions component-wise
template<class T> inline quaternion<T>
operator -(const quaternion<T> &p, const quaternion<T> &q) {
	return quaternion<T>(p.x - q.x, p.y - q.y, p.z - q.z, p.w - q.w);
}

/**
 * @brief Spherical linear interpolation (SLERP) helper class
 *
 * Precomputes values for efficient repeated SLERP interpolation between
 * two quaternions. SLERP provides smooth, constant-velocity rotation
 * interpolation along the shortest arc on the 4D unit hypersphere.
 *
 * Usage:
 * @code
 * quaternion_slerper<float> slerper;
 * slerper.setup(start_rotation, end_rotation);
 * quaternion<float> halfway = slerper.interpolate(0.5f);
 * @endcode
 *
 * @tparam T Scalar type (typically float or double)
 */
template<class T = scalar>
class quaternion_slerper {
public:
	/**
	 * @brief Setup interpolation between two quaternions
	 *
	 * Normalizes both quaternions and precomputes interpolation parameters.
	 * Automatically chooses shorter rotation path.
	 *
	 * @param p Start quaternion
	 * @param q End quaternion
	 */
    void setup(const quaternion<T> &p, const quaternion<T> &q) {
		(q1_ = p).normalize();
		(q2_ = q).normalize();

        T dot = dot_product(q1_, q2_);

		// If dot product is negative, negate one quaternion to take the shorter path
		if (dot < 0) {
			q2_ = -q2_;
			dot = -dot;
		}

		omega_ = (T) acos(dot);

		T inv_sin_omega = T(1/sin(omega_));
		q1_.scale(inv_sin_omega);
		q2_.scale(inv_sin_omega);
    }

	/**
	 * @brief Interpolate between quaternions
	 * @param t Interpolation parameter [0,1], where 0=start, 1=end
	 * @param[out] result Interpolated quaternion
	 */
    void interpolate(T t, quaternion<T> &result) const {
		result = q1_ * T(sin((1 - t) * omega_)) + q2_ * T(sin(t*omega_));
    }

	/**
	 * @brief Interpolate between quaternions (returns value)
	 * @param t Interpolation parameter [0,1], where 0=start, 1=end
	 * @return Interpolated quaternion
	 */
	quaternion<T> interpolate(T t) const {
		quaternion<T> result;
		interpolate(t, result);
		return result;
	}

private:
    quaternion<T> q1_, q2_;  ///< Normalized and scaled quaternions
    T dot, omega_;           ///< Dot product and angle between quaternions
};

}
