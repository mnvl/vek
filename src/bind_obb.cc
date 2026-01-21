/**
 * @file bind_obb.cc
 * @brief Python bindings for OBB classes
 */

#include "python_bindings.h"
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/operators.h>
#include <sstream>

#include "vec.h"
#include "aabb.h"
#include "obb.h"

namespace nb = nanobind;
using namespace nb::literals;

template<typename T>
void bind_obb2(nb::module_ &m, const char *name) {
    using OBB = rove::obb<2, T>;
    using Vec2 = rove::vec<2, T>;
    using AABB2 = rove::aabb<2, T>;

    nb::class_<OBB>(m, name)
        .def(nb::init<>(),
             "Default constructor (uninitialized)")
        .def(nb::init<const AABB2&>(),
             nb::arg("aabb"),
             "Construct OBB from axis-aligned bounding box")
        .def_rw("origin", &OBB::origin, "Origin corner of the OBB")
        .def_rw("tangent", &OBB::tangent, "First edge vector (width direction)")
        .def_rw("normal", &OBB::normal, "Second edge vector (height direction)")
        .def("world_to_local_point",
             static_cast<Vec2 (OBB::*)(const Vec2&) const>(&OBB::world_to_local_point),
             nb::arg("point"),
             "Transform point from world to local OBB coordinates")
        .def("world_to_local_vector",
             static_cast<Vec2 (OBB::*)(const Vec2&) const>(&OBB::world_to_local_vector),
             nb::arg("vector"),
             "Transform vector from world to local OBB coordinates")
        .def("contains", &OBB::contains,
             nb::arg("point"),
             "Test if point is inside OBB")
        .def("get_vertex", &OBB::get_vertex,
             nb::arg("vertex_number"),
             "Get vertex position by index (0-3)")
        .def("get_normal", &OBB::get_normal,
             nb::arg("side_number"),
             "Get outward normal for a side (0-3)")
        .def("get_aabb", [](const OBB &obb) {
            AABB2 result;
            obb.get_aabb(result);
            return result;
        }, "Get axis-aligned bounding box containing this OBB")
        .def("__repr__", [name](const OBB &o) {
            std::ostringstream ss;
            ss << name << "(origin=" << o.origin
               << ", tangent=" << o.tangent
               << ", normal=" << o.normal << ")";
            return ss.str();
        });
}

template<typename T>
void bind_obb3(nb::module_ &m, const char *name) {
    using OBB = rove::obb<3, T>;
    using Vec3 = rove::vec<3, T>;
    using AABB3 = rove::aabb<3, T>;

    nb::class_<OBB>(m, name)
        .def(nb::init<>(),
             "Default constructor (uninitialized)")
        .def(nb::init<const AABB3&>(),
             nb::arg("aabb"),
             "Construct OBB from axis-aligned bounding box")
        .def(nb::init<const Vec3&, const Vec3&, const Vec3&, const Vec3&>(),
             nb::arg("origin"), nb::arg("tangent"), nb::arg("normal"), nb::arg("binormal"),
             "Construct OBB from origin and three edge vectors")
        .def_rw("origin", &OBB::origin, "Origin corner of the OBB")
        .def_rw("tangent", &OBB::tangent, "First edge vector (X-like direction)")
        .def_rw("normal", &OBB::normal, "Second edge vector (Y-like direction)")
        .def_rw("binormal", &OBB::binormal, "Third edge vector (Z-like direction)")
        .def("get_centre", &OBB::get_centre,
             "Get center point of the OBB")
        .def("get_volume", &OBB::get_volume,
             "Get volume of the OBB")
        .def("get_aabb", [](const OBB &obb) {
            AABB3 result;
            obb.get_aabb(result);
            return result;
        }, "Get axis-aligned bounding box containing this OBB")
        .def("get_vertex", [](const OBB &obb, size_t vertex_number) {
            Vec3 position;
            obb.get_vertex(vertex_number, position);
            return position;
        }, nb::arg("vertex_number"),
           "Get vertex position by index (0-7)")
        .def("contains", nb::overload_cast<const Vec3&>(&OBB::contains, nb::const_),
             nb::arg("point"),
             "Test if point is inside OBB")
        .def("test_intersection", nb::overload_cast<const OBB&>(&OBB::test_intersection, nb::const_),
             nb::arg("other"),
             "Test intersection with another OBB")
        .def("__repr__", [name](const OBB &o) {
            std::ostringstream ss;
            ss << name << "(origin=" << o.origin
               << ", tangent=" << o.tangent
               << ", normal=" << o.normal
               << ", binormal=" << o.binormal << ")";
            return ss.str();
        });
}

// Explicit template instantiations
template void bind_obb2<float>(nb::module_ &m, const char *name);
template void bind_obb2<double>(nb::module_ &m, const char *name);
template void bind_obb3<float>(nb::module_ &m, const char *name);
template void bind_obb3<double>(nb::module_ &m, const char *name);
