#pragma once

#include <algorithm>
#include "scalar.h"
#include "vec.h"
#include "matrix.h"

namespace math
{

template<class T = scalar>
class quaternion {
public:
    T x, y, z, w;

	quaternion() {
	}

	quaternion(T x0, T y0, T z0, T w0) {
        x = x0; y = y0;	z = z0;	w = w0;
	}

    void identity() {
       	x = 0; y = 0; z = 0; w = 1;
    }

	quaternion operator -() const {
		return quaternion<T>(-x, -y, -z, -w);
	}

	void scale(T s) {
		x *= s; y *= s; z *= s; w *= s;
    }

	quaternion operator *(T s) const {
		quaternion<T> result(*this);
		result.scale(s);
		return result;
	}

    T norm() const {
       	return (T) sqrt(x*x + y*y + z*z + w*w);
    }

   	void normalize() {
		T n = norm();
		if(n < EPSILON) {
			identity();
		} else {
			scale(1/n);
		}
	}

	quaternion normalized() const {
		quaternion q(*this);
		q.normalize();
		return q;
	}

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

template<class T> inline T
dot_product(const quaternion<T> &p, const quaternion<T> &q) {
	return p.x*q.x + p.y*q.y + p.z*q.z + p.w*q.w;
}

template<class T> inline quaternion<T>
operator +(const quaternion<T> &p, const quaternion<T> &q) {
	return quaternion<T>(p.x + q.x, p.y + q.y, p.z + q.z, p.w + q.w);
}

template<class T> inline quaternion<T>
operator -(const quaternion<T> &p, const quaternion<T> &q) {
	return quaternion<T>(p.x - q.x, p.y - q.y, p.z - q.z, p.w - q.w);
}

template<class T = scalar>
class quaternion_slerper {
public:
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

    void interpolate(T t, quaternion<T> &result) const {
		result = q1_ * T(sin((1 - t) * omega_)) + q2_ * T(sin(t*omega_));
    }

	quaternion<T> interpolate(T t) const {
		quaternion<T> result;
		interpolate(t, result);
		return result;
	}

private:
    quaternion<T> q1_, q2_;
    T dot, omega_;
};

}
