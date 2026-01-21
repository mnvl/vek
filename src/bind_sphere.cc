/**
 * @file bind_sphere.cc
 * @brief Python bindings for sphere classes
 */

#include "python_bindings.h"
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/ndarray.h>
#include <sstream>

#include "vec.h"
#include "sphere.h"
#include "aabb.h"

namespace nb = nanobind;
using namespace nb::literals;

template<typename T>
void bind_sphere2(nb::module_ &m, const char *name) {
    using Sphere = rove::sphere<2, T>;
    using Vec2 = rove::vec<2, T>;
    using AABB2 = rove::aabb<2, T>;

    nb::class_<Sphere>(m, name)
        .def(nb::init<>())
        .def(nb::init<const Vec2&, T>(),
             nb::arg("centre"), nb::arg("radius"),
             "Construct sphere from center and radius")
        .def(nb::init<const AABB2&>(),
             nb::arg("aabb"),
             "Construct bounding sphere from AABB")
        .def_rw("centre", &Sphere::centre, "Center point of the sphere")
        .def_rw("radius", &Sphere::radius, "Radius of the sphere")
        .def("set", &Sphere::set,
             nb::arg("centre"), nb::arg("radius"),
             "Set sphere center and radius")
        .def("construct", &Sphere::construct,
             nb::arg("aabb"),
             "Initialize sphere from AABB")
        .def("get_aabb", &Sphere::get_aabb,
             "Get axis-aligned bounding box containing this sphere")
        .def("contains", &Sphere::contains,
             nb::arg("point"),
             "Test if point is inside sphere")
        .def("test_intersection",
             nb::overload_cast<const Sphere&>(&Sphere::test_intersection, nb::const_),
             nb::arg("other"),
             "Test intersection with another sphere")
        .def("__repr__", [](const Sphere &s) {
            std::ostringstream ss;
            ss << "sphere2(centre=" << s.centre << ", radius=" << s.radius << ")";
            return ss.str();
        });
}

template<typename T>
void bind_sphere3(nb::module_ &m, const char *name) {
    using Sphere = rove::sphere<3, T>;
    using Vec3 = rove::vec<3, T>;
    using AABB3 = rove::aabb<3, T>;

    nb::class_<Sphere>(m, name)
        .def(nb::init<>())
        .def(nb::init<const Vec3&, T>(),
             nb::arg("centre"), nb::arg("radius"),
             "Construct sphere from center and radius")
        .def(nb::init<const AABB3&>(),
             nb::arg("aabb"),
             "Construct bounding sphere from AABB")
        .def_rw("centre", &Sphere::centre, "Center point of the sphere")
        .def_rw("radius", &Sphere::radius, "Radius of the sphere")
        .def("set", &Sphere::set,
             nb::arg("centre"), nb::arg("radius"),
             "Set sphere center and radius")
        .def("construct", &Sphere::construct,
             nb::arg("aabb"),
             "Initialize sphere from AABB")
        .def("get_aabb", &Sphere::get_aabb,
             "Get axis-aligned bounding box containing this sphere")
        .def("contains", &Sphere::contains,
             nb::arg("point"),
             "Test if point is inside sphere")
        .def("test_intersection",
             nb::overload_cast<const Sphere&>(&Sphere::test_intersection, nb::const_),
             nb::arg("other"),
             "Test intersection with another sphere")
        .def("__repr__", [](const Sphere &s) {
            std::ostringstream ss;
            ss << "sphere3(centre=" << s.centre << ", radius=" << s.radius << ")";
            return ss.str();
        });
}

// Explicit template instantiations
template void bind_sphere2<float>(nb::module_ &m, const char *name);
template void bind_sphere2<double>(nb::module_ &m, const char *name);
template void bind_sphere3<float>(nb::module_ &m, const char *name);
template void bind_sphere3<double>(nb::module_ &m, const char *name);
