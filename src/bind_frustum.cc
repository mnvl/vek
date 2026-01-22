/**
 * @file bind_frustum.cc
 * @brief Python bindings for frustum class
 */

#include "python_bindings.h"
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <sstream>

#include "frustum.h"
#include "aabb.h"
#include "obb.h"

namespace nb = nanobind;
using namespace nb::literals;

template<typename T>
void bind_frustum(nb::module_ &m, const char *name) {
    using Frustum = rove::frustum<T>;
    using AABB3 = rove::aabb<3, T>;
    using OBB3 = rove::obb<3, T>;

    nb::class_<Frustum>(m, name)
        .def(nb::init<>())
        .def("test_intersection",
             nb::overload_cast<const AABB3&>(&Frustum::test_intersection, nb::const_),
             nb::arg("aabb"),
             "Test intersection with axis-aligned bounding box")
        .def("test_intersection",
             nb::overload_cast<const OBB3&>(&Frustum::test_intersection, nb::const_),
             nb::arg("obb"),
             "Test intersection with oriented bounding box")
        .def("__repr__", [name](const Frustum &) {
            return std::string(name) + "()";
        });
}

// Explicit template instantiations
template void bind_frustum<float>(nb::module_ &m, const char *name);
template void bind_frustum<double>(nb::module_ &m, const char *name);
