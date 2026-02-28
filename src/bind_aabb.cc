/**
 * @file bind_aabb.cc
 * @brief Python bindings for AABB classes
 */

#include "python_bindings.h"
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/operators.h>
#include <sstream>

#include "vec.h"
#include "aabb.h"

namespace nb = nanobind;
using namespace nb::literals;

template<typename T>
void bind_aabb2(nb::module_ &m, const char *name) {
    using AABB = rove::aabb<2, T>;
    using Vec2 = rove::vec<2, T>;

    nb::class_<AABB>(m, name)
        .def(nb::init<>(),
             "Default constructor (uninitialized)")
        .def(nb::init<const Vec2&>(),
             nb::arg("centre"),
             "Construct degenerate AABB from single point")
        .def(nb::init<const Vec2&, const Vec2&>(),
             nb::arg("lo"), nb::arg("hi"),
             "Construct AABB from two corner points")
        .def(nb::init<const Vec2&, rove::scalar>(),
             nb::arg("centre"), nb::arg("radius"),
             "Construct AABB from center and half-extent")
        .def_rw("lo", &AABB::lo, "Minimum corner (lower bounds)")
        .def_rw("hi", &AABB::hi, "Maximum corner (upper bounds)")
        .def("null", &AABB::null,
             "Reset AABB to null state for accumulation")
        .def("normalize", &AABB::normalize,
             "Ensure lo <= hi in all dimensions")
        .def("diagonal", &AABB::diagonal,
             "Get AABB diagonal vector (hi - lo)")
        .def("centre", &AABB::centre,
             "Get AABB center point")
        .def("extend", nb::overload_cast<const Vec2&>(&AABB::extend),
             nb::arg("point"),
             "Extend AABB to include a point")
        .def("extend", nb::overload_cast<const AABB&>(&AABB::extend),
             nb::arg("other"),
             "Extend AABB to include another AABB")
        .def("contains", nb::overload_cast<const Vec2&>(&AABB::contains, nb::const_),
             nb::arg("point"),
             "Test if point is inside AABB")
        .def("contains", nb::overload_cast<const AABB&>(&AABB::contains, nb::const_),
             nb::arg("other"),
             "Test if another AABB is fully contained")
        .def("test_intersection", &AABB::test_intersection,
             nb::arg("other"),
             "Test intersection with another AABB")
        .def("__repr__", [name](const AABB &a) {
            std::ostringstream ss;
            ss << name << "(lo=" << a.lo << ", hi=" << a.hi << ")";
            return ss.str();
        })
        .def("__getstate__", [](const AABB &a) {
            return nb::make_tuple(a.lo, a.hi);
        })
        .def("__setstate__", [](AABB &a, nb::tuple t) {
            a.lo = nb::cast<Vec2>(t[0]);
            a.hi = nb::cast<Vec2>(t[1]);
        });
}

template<typename T>
void bind_aabb3(nb::module_ &m, const char *name) {
    using AABB = rove::aabb<3, T>;
    using Vec3 = rove::vec<3, T>;

    nb::class_<AABB>(m, name)
        .def(nb::init<>(),
             "Default constructor (uninitialized)")
        .def(nb::init<const Vec3&>(),
             nb::arg("centre"),
             "Construct degenerate AABB from single point")
        .def(nb::init<const Vec3&, const Vec3&>(),
             nb::arg("lo"), nb::arg("hi"),
             "Construct AABB from two corner points")
        .def(nb::init<const Vec3&, rove::scalar>(),
             nb::arg("centre"), nb::arg("radius"),
             "Construct AABB from center and half-extent")
        .def_rw("lo", &AABB::lo, "Minimum corner (lower bounds)")
        .def_rw("hi", &AABB::hi, "Maximum corner (upper bounds)")
        .def("null", &AABB::null,
             "Reset AABB to null state for accumulation")
        .def("normalize", &AABB::normalize,
             "Ensure lo <= hi in all dimensions")
        .def("diagonal", &AABB::diagonal,
             "Get AABB diagonal vector (hi - lo)")
        .def("centre", &AABB::centre,
             "Get AABB center point")
        .def("extend", nb::overload_cast<const Vec3&>(&AABB::extend),
             nb::arg("point"),
             "Extend AABB to include a point")
        .def("extend", nb::overload_cast<const AABB&>(&AABB::extend),
             nb::arg("other"),
             "Extend AABB to include another AABB")
        .def("contains", nb::overload_cast<const Vec3&>(&AABB::contains, nb::const_),
             nb::arg("point"),
             "Test if point is inside AABB")
        .def("contains", nb::overload_cast<const AABB&>(&AABB::contains, nb::const_),
             nb::arg("other"),
             "Test if another AABB is fully contained")
        .def("test_intersection", &AABB::test_intersection,
             nb::arg("other"),
             "Test intersection with another AABB")
        .def("__repr__", [name](const AABB &a) {
            std::ostringstream ss;
            ss << name << "(lo=" << a.lo << ", hi=" << a.hi << ")";
            return ss.str();
        })
        .def("__getstate__", [](const AABB &a) {
            return nb::make_tuple(a.lo, a.hi);
        })
        .def("__setstate__", [](AABB &a, nb::tuple t) {
            a.lo = nb::cast<Vec3>(t[0]);
            a.hi = nb::cast<Vec3>(t[1]);
        });
}

// Explicit template instantiations
template void bind_aabb2<float>(nb::module_ &m, const char *name);
template void bind_aabb2<double>(nb::module_ &m, const char *name);
template void bind_aabb3<float>(nb::module_ &m, const char *name);
template void bind_aabb3<double>(nb::module_ &m, const char *name);
