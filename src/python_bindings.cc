#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/ndarray.h>
#include <sstream>

#include "vec.h"
#include "matrix.h"
#include "quaternion.h"

namespace nb = nanobind;
using namespace nb::literals;

template<typename T>
void bind_vec2(nb::module_ &m, const char *name) {
    using Vec = rove::vec<2, T>;

    nb::class_<Vec>(m, name)
        .def(nb::init<>())
        .def(nb::init<T, T>(), nb::arg("x"), nb::arg("y"))
        .def_rw("x", &Vec::x)
        .def_rw("y", &Vec::y)
        .def_static("from_numpy", [](nb::ndarray<T, nb::shape<2>> arr) {
            return Vec(arr(0), arr(1));
        }, nb::arg("array"), "Create vec2 from numpy array")
        .def("to_numpy", [](const Vec &v) {
            T* data = new T[2]{v.x, v.y};
            size_t shape[1] = {2};
            nb::capsule deleter(data, [](void *p) noexcept {
                delete[] (T*)p;
            });
            return nb::ndarray<nb::numpy, T, nb::shape<2>>(data, 1, shape, deleter);
        }, "Convert vec2 to numpy array")
        .def("set", &Vec::set, nb::arg("x"), nb::arg("y"))
        .def("set_all", &Vec::set_all, nb::arg("a"))
        .def("length", &Vec::length)
        .def("length_sq", &Vec::length_sq)
        .def("length_manhattan", &Vec::length_manhattan)
        .def("normalize", &Vec::normalize)
        .def("negate", &Vec::negate)
        .def("perpendicular", &Vec::perpendicular)
        .def("is_collinear", &Vec::is_collinear, nb::arg("v"), nb::arg("epsilon") = rove::EPSILON)
        .def("__neg__", [](const Vec &v) { return -v; })
        .def("__add__", [](const Vec &a, const Vec &b) { return a + b; })
        .def("__sub__", [](const Vec &a, const Vec &b) { return a - b; })
        .def("__mul__", [](const Vec &a, const Vec &b) { return a * b; })
        .def("__mul__", [](const Vec &a, T k) { return a * k; })
        .def("__rmul__", [](const Vec &a, T k) { return a * k; })
        .def("__truediv__", [](const Vec &a, const Vec &b) { return a / b; })
        .def("__truediv__", [](const Vec &a, T k) { return a / k; })
        .def("__eq__", [](const Vec &a, const Vec &b) { return a == b; })
        .def("__ne__", [](const Vec &a, const Vec &b) { return a != b; })
        .def("__repr__", [](const Vec &v) {
            std::ostringstream ss;
            ss << "vec2(" << v.x << ", " << v.y << ")";
            return ss.str();
        })
        .def("dot", [](const Vec &a, const Vec &b) { return rove::dot_product(a, b); });
}

template<typename T>
void bind_vec3(nb::module_ &m, const char *name) {
    using Vec = rove::vec<3, T>;

    nb::class_<Vec>(m, name)
        .def(nb::init<>())
        .def(nb::init<T, T, T>(), nb::arg("x"), nb::arg("y"), nb::arg("z"))
        .def_rw("x", &Vec::x)
        .def_rw("y", &Vec::y)
        .def_rw("z", &Vec::z)
        .def_static("from_numpy", [](nb::ndarray<T, nb::shape<3>> arr) {
            return Vec(arr(0), arr(1), arr(2));
        }, nb::arg("array"), "Create vec3 from numpy array")
        .def("to_numpy", [](const Vec &v) {
            T* data = new T[3]{v.x, v.y, v.z};
            size_t shape[1] = {3};
            nb::capsule deleter(data, [](void *p) noexcept {
                delete[] (T*)p;
            });
            return nb::ndarray<nb::numpy, T, nb::shape<3>>(data, 1, shape, deleter);
        }, "Convert vec3 to numpy array")
        .def("set", &Vec::set, nb::arg("x"), nb::arg("y"), nb::arg("z"))
        .def("set_all", &Vec::set_all, nb::arg("a"))
        .def("length", &Vec::length)
        .def("length_sq", &Vec::length_sq)
        .def("normalize", &Vec::normalize)
        .def("negate", &Vec::negate)
        .def("is_collinear", &Vec::is_collinear, nb::arg("v"), nb::arg("epsilon") = rove::EPSILON)
        .def("__neg__", [](const Vec &v) { return -v; })
        .def("__add__", [](const Vec &a, const Vec &b) { return a + b; })
        .def("__sub__", [](const Vec &a, const Vec &b) { return a - b; })
        .def("__mul__", [](const Vec &a, const Vec &b) { return a * b; })
        .def("__mul__", [](const Vec &a, T k) { return a * k; })
        .def("__rmul__", [](const Vec &a, T k) { return a * k; })
        .def("__truediv__", [](const Vec &a, const Vec &b) { return a / b; })
        .def("__truediv__", [](const Vec &a, T k) { return a / k; })
        .def("__eq__", [](const Vec &a, const Vec &b) { return a == b; })
        .def("__ne__", [](const Vec &a, const Vec &b) { return a != b; })
        .def("__repr__", [](const Vec &v) {
            std::ostringstream ss;
            ss << "vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
            return ss.str();
        })
        .def("dot", [](const Vec &a, const Vec &b) { return rove::dot_product(a, b); })
        .def("cross", [](const Vec &a, const Vec &b) {
            Vec result;
            rove::cross_product(result, a, b);
            return result;
        });
}

template<typename T>
void bind_vec4(nb::module_ &m, const char *name) {
    using Vec = rove::vec<4, T>;

    nb::class_<Vec>(m, name)
        .def(nb::init<>())
        .def(nb::init<T, T, T, T>(), nb::arg("x"), nb::arg("y"), nb::arg("z"), nb::arg("w"))
        .def_rw("x", &Vec::x)
        .def_rw("y", &Vec::y)
        .def_rw("z", &Vec::z)
        .def_rw("w", &Vec::w)
        .def_static("from_numpy", [](nb::ndarray<T, nb::shape<4>> arr) {
            return Vec(arr(0), arr(1), arr(2), arr(3));
        }, nb::arg("array"), "Create vec4 from numpy array")
        .def("to_numpy", [](const Vec &v) {
            T* data = new T[4]{v.x, v.y, v.z, v.w};
            size_t shape[1] = {4};
            nb::capsule deleter(data, [](void *p) noexcept {
                delete[] (T*)p;
            });
            return nb::ndarray<nb::numpy, T, nb::shape<4>>(data, 1, shape, deleter);
        }, "Convert vec4 to numpy array")
        .def("set", &Vec::set, nb::arg("x"), nb::arg("y"), nb::arg("z"), nb::arg("w"))
        .def("set_all", &Vec::set_all, nb::arg("a"))
        .def("length", &Vec::length)
        .def("length_sq", &Vec::length_sq)
        .def("normalize", &Vec::normalize)
        .def("negate", &Vec::negate)
        .def("__neg__", [](const Vec &v) { return Vec(-v.x, -v.y, -v.z, -v.w); })
        .def("__add__", [](const Vec &a, const Vec &b) { return a + b; })
        .def("__sub__", [](const Vec &a, const Vec &b) { return a - b; })
        .def("__mul__", [](const Vec &a, T k) { return a * k; })
        .def("__rmul__", [](const Vec &a, T k) { return a * k; })
        .def("__truediv__", [](const Vec &a, T k) { return a / k; })
        .def("__eq__", [](const Vec &a, const Vec &b) { return a == b; })
        .def("__ne__", [](const Vec &a, const Vec &b) { return a != b; })
        .def("__repr__", [](const Vec &v) {
            std::ostringstream ss;
            ss << "vec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
            return ss.str();
        })
        .def("dot", [](const Vec &a, const Vec &b) { return rove::dot_product(a, b); });
}

template<typename T>
void bind_matrix3(nb::module_ &m, const char *name) {
    using Mat = rove::matrix<3, 3, T>;
    using Vec3 = rove::vec<3, T>;
    using Vec2 = rove::vec<2, T>;
    using Quat = rove::quaternion<T>;

    nb::class_<Mat>(m, name)
        .def(nb::init<>())
        .def_static("from_numpy", [](nb::ndarray<T, nb::shape<3, 3>> arr) {
            Mat result;
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    result.ij[col][row] = arr(row, col);
                }
            }
            return result;
        }, nb::arg("array"), "Create mat3 from numpy array")
        .def("to_numpy", [](const Mat &m) {
            T* data = new T[9];
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    data[row * 3 + col] = m.ij[col][row];
                }
            }
            size_t shape[2] = {3, 3};
            nb::capsule deleter(data, [](void *p) noexcept {
                delete[] (T*)p;
            });
            return nb::ndarray<nb::numpy, T, nb::shape<3, 3>>(data, 2, shape, deleter);
        }, "Convert mat3 to numpy array")
        .def("identity", &Mat::identity, "Set to identity matrix")
        .def("zero", &Mat::zero, "Set to zero matrix")
        .def("translation", nb::overload_cast<T, T>(&Mat::translation),
             nb::arg("x"), nb::arg("y"), "Set to translation matrix")
        .def("translation", nb::overload_cast<const Vec2&>(&Mat::translation),
             nb::arg("p"), "Set to translation matrix")
        .def("translate", nb::overload_cast<T, T>(&Mat::translate),
             nb::arg("x"), nb::arg("y"), "Apply translation")
        .def("translate", nb::overload_cast<const Vec2&>(&Mat::translate),
             nb::arg("p"), "Apply translation")
        .def("scaling", nb::overload_cast<T, T>(&Mat::scaling),
             nb::arg("x"), nb::arg("y"), "Set to scaling matrix")
        .def("scaling", nb::overload_cast<const Vec2&>(&Mat::scaling),
             nb::arg("p"), "Set to scaling matrix")
        .def("scale", nb::overload_cast<T, T>(&Mat::scale),
             nb::arg("x"), nb::arg("y"), "Apply scaling")
        .def("scale", nb::overload_cast<const Vec2&>(&Mat::scale),
             nb::arg("p"), "Apply scaling")
        .def("rotation", nb::overload_cast<const Vec3&, T>(&Mat::rotation),
             nb::arg("axis"), nb::arg("angle"), "Set to rotation matrix")
        .def("rotation", nb::overload_cast<const Quat&>(&Mat::rotation),
             nb::arg("q"), "Set to rotation matrix from quaternion")
        .def("rotate", nb::overload_cast<const Vec3&, T>(&Mat::rotate),
             nb::arg("axis"), nb::arg("angle"), "Apply rotation")
        .def("rotate", nb::overload_cast<const Quat&>(&Mat::rotate),
             nb::arg("q"), "Apply rotation from quaternion")
        .def("determinant", &Mat::determinant, "Compute determinant")
        .def("trace", &Mat::trace, "Compute trace")
        .def("invert", &Mat::invert, "Invert matrix in place")
        .def("inverse", [](const Mat &m) {
            Mat result;
            if (!m.inverse(result)) {
                throw std::runtime_error("Matrix is singular");
            }
            return result;
        }, "Return inverse matrix")
        .def("transpose", [](const Mat &m) {
            Mat result;
            m.transpose(result);
            return result;
        }, "Return transposed matrix")
        .def("__imul__", nb::overload_cast<T>(&Mat::operator*=), nb::arg("k"))
        .def("__imul__", nb::overload_cast<const Mat&>(&Mat::operator*=), nb::arg("m"))
        .def("__itruediv__", &Mat::operator/=, nb::arg("k"))
        .def("__mul__", [](const Mat &m, T k) {
            Mat result = m;
            result *= k;
            return result;
        })
        .def("__mul__", [](const Mat &a, const Mat &b) {
            Mat result;
            rove::mul(result, a, b);
            return result;
        })
        .def("__mul__", [](const Mat &m, const Vec2 &v) {
            return v * m;
        }, "Transform vec2 by mat3, returning vec3")
        .def("__mul__", [](const Mat &m, const Vec3 &v) {
            return v * m;
        }, "Transform vec3 by mat3")
        .def("__rmul__", [](const Mat &m, T k) {
            Mat result = m;
            result *= k;
            return result;
        })
        .def("__truediv__", [](const Mat &m, T k) {
            Mat result = m;
            result /= k;
            return result;
        })
        .def("__repr__", [](const Mat &m) {
            std::ostringstream ss;
            ss << "mat3([";
            for (int i = 0; i < 3; i++) {
                ss << "[";
                for (int j = 0; j < 3; j++) {
                    ss << m.ij[j][i];
                    if (j < 2) ss << ", ";
                }
                ss << "]";
                if (i < 2) ss << ", ";
            }
            ss << "])";
            return ss.str();
        })
        .def("get", [](const Mat &m, int i, int j) {
            if (i < 0 || i >= 3 || j < 0 || j >= 3) {
                throw std::out_of_range("Matrix index out of range");
            }
            return m.ij[i][j];
        }, nb::arg("i"), nb::arg("j"), "Get matrix element at [i][j]")
        .def("set", [](Mat &m, int i, int j, T value) {
            if (i < 0 || i >= 3 || j < 0 || j >= 3) {
                throw std::out_of_range("Matrix index out of range");
            }
            m.ij[i][j] = value;
        }, nb::arg("i"), nb::arg("j"), nb::arg("value"), "Set matrix element at [i][j]");
}

template<typename T>
void bind_matrix4(nb::module_ &m, const char *name) {
    using Mat = rove::matrix<4, 4, T>;
    using Vec3 = rove::vec<3, T>;
    using Vec4 = rove::vec<4, T>;
    using Quat = rove::quaternion<T>;

    nb::class_<Mat>(m, name)
        .def(nb::init<>())
        .def_static("from_numpy", [](nb::ndarray<T, nb::shape<4, 4>> arr) {
            Mat result;
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    result.ij[col][row] = arr(row, col);
                }
            }
            return result;
        }, nb::arg("array"), "Create mat4 from numpy array")
        .def("to_numpy", [](const Mat &m) {
            T* data = new T[16];
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    data[row * 4 + col] = m.ij[col][row];
                }
            }
            size_t shape[2] = {4, 4};
            nb::capsule deleter(data, [](void *p) noexcept {
                delete[] (T*)p;
            });
            return nb::ndarray<nb::numpy, T, nb::shape<4, 4>>(data, 2, shape, deleter);
        }, "Convert mat4 to numpy array")
        .def("identity", &Mat::identity, "Set to identity matrix")
        .def("zero", &Mat::zero, "Set to zero matrix")
        .def("translation", nb::overload_cast<T, T, T>(&Mat::translation),
             nb::arg("x"), nb::arg("y"), nb::arg("z"), "Set to translation matrix")
        .def("translation", nb::overload_cast<const Vec3&>(&Mat::translation),
             nb::arg("p"), "Set to translation matrix")
        .def("translate", nb::overload_cast<T, T, T>(&Mat::translate),
             nb::arg("x"), nb::arg("y"), nb::arg("z"), "Apply translation")
        .def("translate", nb::overload_cast<const Vec3&>(&Mat::translate),
             nb::arg("p"), "Apply translation")
        .def("scaling", nb::overload_cast<T, T, T>(&Mat::scaling),
             nb::arg("x"), nb::arg("y"), nb::arg("z"), "Set to scaling matrix")
        .def("scaling", nb::overload_cast<const Vec3&>(&Mat::scaling),
             nb::arg("p"), "Set to scaling matrix")
        .def("scale", nb::overload_cast<T, T, T>(&Mat::scale),
             nb::arg("x"), nb::arg("y"), nb::arg("z"), "Apply scaling")
        .def("scale", nb::overload_cast<const Vec3&>(&Mat::scale),
             nb::arg("p"), "Apply scaling")
        .def("rotation", nb::overload_cast<const Vec3&, T>(&Mat::rotation),
             nb::arg("axis"), nb::arg("angle"), "Set to rotation matrix")
        .def("rotation", nb::overload_cast<const Quat&>(&Mat::rotation),
             nb::arg("q"), "Set to rotation matrix from quaternion")
        .def("rotate", nb::overload_cast<const Vec3&, T>(&Mat::rotate),
             nb::arg("axis"), nb::arg("angle"), "Apply rotation")
        .def("rotate", nb::overload_cast<const Quat&>(&Mat::rotate),
             nb::arg("q"), "Apply rotation from quaternion")
        .def("lookat", &Mat::lookat,
             nb::arg("eye"), nb::arg("at"), nb::arg("up"), "Set to lookat matrix")
        .def("ortho", &Mat::ortho,
             nb::arg("width"), nb::arg("height"), nb::arg("z_near"), nb::arg("z_far"),
             "Set to orthographic projection matrix")
        .def("perspective", &Mat::perspective,
             nb::arg("fovy"), nb::arg("aspect"), nb::arg("z_near"), nb::arg("z_far"),
             "Set to perspective projection matrix")
        .def("determinant", &Mat::determinant, "Compute determinant")
        .def("invert", &Mat::invert, "Invert matrix in place")
        .def("inverse", [](const Mat &m) {
            Mat result;
            if (!m.inverse(result)) {
                throw std::runtime_error("Matrix is singular");
            }
            return result;
        }, "Return inverse matrix")
        .def("transpose", [](const Mat &m) {
            Mat result;
            m.transpose(result);
            return result;
        }, "Return transposed matrix")
        .def("transpose_inplace", &Mat::transpose_inplace, "Transpose matrix in place")
        .def("column", &Mat::column, nb::arg("index"), "Get column as vec4")
        .def("projected_column", &Mat::projected_column, nb::arg("index"), "Get column as vec3")
        .def("projected_row", &Mat::projected_row, nb::arg("index"), "Get row as vec3")
        .def("__imul__", nb::overload_cast<T>(&Mat::operator*=), nb::arg("k"))
        .def("__imul__", nb::overload_cast<const Mat&>(&Mat::operator*=), nb::arg("m"))
        .def("__itruediv__", &Mat::operator/=, nb::arg("k"))
        .def("__mul__", [](const Mat &m, T k) {
            Mat result = m;
            result *= k;
            return result;
        })
        .def("__mul__", [](const Mat &a, const Mat &b) {
            Mat result;
            rove::mul(result, a, b);
            return result;
        })
        .def("__mul__", [](const Mat &m, const Vec3 &v) {
            return v * m;
        }, "Transform vec3 by mat4, returning vec3")
        .def("__mul__", [](const Mat &m, const Vec4 &v) {
            return v * m;
        }, "Transform vec4 by mat4")
        .def("__rmul__", [](const Mat &m, T k) {
            Mat result = m;
            result *= k;
            return result;
        })
        .def("__truediv__", [](const Mat &m, T k) {
            Mat result = m;
            result /= k;
            return result;
        })
        .def("__repr__", [](const Mat &m) {
            std::ostringstream ss;
            ss << "mat4([";
            for (int i = 0; i < 4; i++) {
                ss << "[";
                for (int j = 0; j < 4; j++) {
                    ss << m.ij[j][i];
                    if (j < 3) ss << ", ";
                }
                ss << "]";
                if (i < 3) ss << ", ";
            }
            ss << "])";
            return ss.str();
        })
        .def("get", [](const Mat &m, int i, int j) {
            if (i < 0 || i >= 4 || j < 0 || j >= 4) {
                throw std::out_of_range("Matrix index out of range");
            }
            return m.ij[i][j];
        }, nb::arg("i"), nb::arg("j"), "Get matrix element at [i][j]")
        .def("set", [](Mat &m, int i, int j, T value) {
            if (i < 0 || i >= 4 || j < 0 || j >= 4) {
                throw std::out_of_range("Matrix index out of range");
            }
            m.ij[i][j] = value;
        }, nb::arg("i"), nb::arg("j"), nb::arg("value"), "Set matrix element at [i][j]");
}

template<typename T>
void bind_quaternion(nb::module_ &m, const char *name) {
    using Quat = rove::quaternion<T>;
    using Vec3 = rove::vec<3, T>;

    nb::class_<Quat>(m, name)
        .def(nb::init<>())
        .def(nb::init<T, T, T, T>(), nb::arg("x"), nb::arg("y"), nb::arg("z"), nb::arg("w"))
        .def_static("from_numpy", [](nb::ndarray<T, nb::shape<4>> arr) {
            return Quat(arr(0), arr(1), arr(2), arr(3));
        }, nb::arg("array"), "Create quaternion from numpy array")
        .def("to_numpy", [](const Quat &q) {
            T* data = new T[4]{q.x, q.y, q.z, q.w};
            size_t shape[1] = {4};
            nb::capsule deleter(data, [](void *p) noexcept {
                delete[] (T*)p;
            });
            return nb::ndarray<nb::numpy, T, nb::shape<4>>(data, 1, shape, deleter);
        }, "Convert quaternion to numpy array")
        .def_rw("x", &Quat::x)
        .def_rw("y", &Quat::y)
        .def_rw("z", &Quat::z)
        .def_rw("w", &Quat::w)
        .def("identity", &Quat::identity, "Set to identity quaternion (0, 0, 0, 1)")
        .def("norm", &Quat::norm, "Compute quaternion norm")
        .def("normalize", &Quat::normalize, "Normalize quaternion in place")
        .def("normalized", &Quat::normalized, "Return normalized quaternion")
        .def("scale", &Quat::scale, nb::arg("s"), "Scale quaternion by scalar")
        .def("__neg__", [](const Quat &q) { return -q; })
        .def("__add__", [](const Quat &p, const Quat &q) { return p + q; })
        .def("__sub__", [](const Quat &p, const Quat &q) { return p - q; })
        .def("__mul__", [](const Quat &q, T s) { return q * s; })
        .def("__rmul__", [](const Quat &q, T s) { return q * s; })
        .def("__repr__", [](const Quat &q) {
            std::ostringstream ss;
            ss << "quat(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
            return ss.str();
        })
        .def("dot", [](const Quat &p, const Quat &q) {
            return rove::dot_product(p, q);
        }, nb::arg("other"), "Compute dot product with another quaternion");
}

template<typename T>
void bind_quaternion_slerper(nb::module_ &m, const char *name) {
    using Slerper = rove::quaternion_slerper<T>;
    using Quat = rove::quaternion<T>;

    nb::class_<Slerper>(m, name)
        .def(nb::init<>())
        .def("setup", &Slerper::setup, nb::arg("p"), nb::arg("q"),
             "Setup SLERP between two quaternions")
        .def("interpolate", nb::overload_cast<T>(&Slerper::interpolate, nb::const_),
             nb::arg("t"), "Interpolate between quaternions at parameter t [0,1]");
}

NB_MODULE(pyrove, m) {
    m.doc() = "Python bindings for rove vector math library";

    // Bind float versions (default)
    bind_vec2<float>(m, "vec2");
    bind_vec3<float>(m, "vec3");
    bind_vec4<float>(m, "vec4");

    // Bind double versions
    bind_vec2<double>(m, "dvec2");
    bind_vec3<double>(m, "dvec3");
    bind_vec4<double>(m, "dvec4");

    // Bind matrix float versions (default)
    bind_matrix3<float>(m, "mat3");
    bind_matrix4<float>(m, "mat4");

    // Bind matrix double versions
    bind_matrix3<double>(m, "dmat3");
    bind_matrix4<double>(m, "dmat4");

    // Bind quaternion float versions (default)
    bind_quaternion<float>(m, "quat");
    bind_quaternion_slerper<float>(m, "quat_slerper");

    // Bind quaternion double versions
    bind_quaternion<double>(m, "dquat");
    bind_quaternion_slerper<double>(m, "dquat_slerper");

    // Free functions
    m.def("dot", [](const rove::vec<2, float> &a, const rove::vec<2, float> &b) {
        return rove::dot_product(a, b);
    }, nb::arg("a"), nb::arg("b"), "Compute dot product of two vec2");

    m.def("dot", [](const rove::vec<3, float> &a, const rove::vec<3, float> &b) {
        return rove::dot_product(a, b);
    }, nb::arg("a"), nb::arg("b"), "Compute dot product of two vec3");

    m.def("dot", [](const rove::vec<4, float> &a, const rove::vec<4, float> &b) {
        return rove::dot_product(a, b);
    }, nb::arg("a"), nb::arg("b"), "Compute dot product of two vec4");

    m.def("cross", [](const rove::vec<3, float> &a, const rove::vec<3, float> &b) {
        rove::vec<3, float> result;
        rove::cross_product(result, a, b);
        return result;
    }, nb::arg("a"), nb::arg("b"), "Compute cross product of two vec3");

    m.def("normalize", [](const rove::vec<2, float> &v) {
        return rove::normalize(v);
    }, nb::arg("v"), "Return normalized vec2");

    m.def("normalize", [](const rove::vec<3, float> &v) {
        return rove::normalize(v);
    }, nb::arg("v"), "Return normalized vec3");

    m.def("normalize", [](const rove::vec<4, float> &v) {
        return rove::normalize(v);
    }, nb::arg("v"), "Return normalized vec4");

    // Quaternion free functions
    m.def("dot", [](const rove::quaternion<float> &p, const rove::quaternion<float> &q) {
        return rove::dot_product(p, q);
    }, nb::arg("p"), nb::arg("q"), "Compute dot product of two quaternions");

    m.def("slerp", [](const rove::quaternion<float> &p, const rove::quaternion<float> &q, float t) {
        rove::quaternion_slerper<float> slerper;
        slerper.setup(p, q);
        return slerper.interpolate(t);
    }, nb::arg("p"), nb::arg("q"), nb::arg("t"), "Spherical linear interpolation between quaternions");
}
