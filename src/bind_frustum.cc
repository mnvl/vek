/**
 * @file bind_frustum.cc
 * @brief Python bindings for frustum class
 */

#include "python_bindings.h"
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "frustum.h"
#include "aabb.h"
#include "obb.h"
#include "matrix.h"
#include "vec.h"

namespace nb = nanobind;
using namespace nb::literals;

template<typename T>
void bind_frustum(nb::module_ &m, const char *name) {
    using Frustum = rove::frustum<T>;
    using AABB3 = rove::aabb<3, T>;
    using OBB3 = rove::obb<3, T>;
    using Plane = rove::plane<T>;

    using Mat4 = rove::matrix<4, 4, T>;
    using Vec3 = rove::vec<3, T>;

    nb::class_<Frustum>(m, name)
        .def(nb::init<>(), "Default constructor")
        .def(nb::init<const Mat4&>(),
             nb::arg("matrix"),
             "Construct frustum from view-projection matrix")
        .def("load", &Frustum::load,
             nb::arg("matrix"),
             "Load frustum planes from view-projection matrix")
        .def("contains", &Frustum::contains,
             nb::arg("point"),
             "Test if point is inside the frustum")
        .def("test_intersection",
             nb::overload_cast<const AABB3&>(&Frustum::test_intersection, nb::const_),
             nb::arg("aabb"),
             "Test intersection with axis-aligned bounding box")
        .def("test_intersection",
             nb::overload_cast<const OBB3&>(&Frustum::test_intersection, nb::const_),
             nb::arg("obb"),
             "Test intersection with oriented bounding box")
        .def("get_plane", [](const Frustum &f, int index) -> const Plane& {
            if (index < 0 || index >= 6) {
                throw std::out_of_range("Plane index must be 0-5");
            }
            return f.planes[index];
        }, nb::arg("index"), nb::rv_policy::reference_internal,
           "Get plane by index (0=LEFT, 1=RIGHT, 2=TOP, 3=BOTTOM, 4=NEAR, 5=FAR)")
        .def_prop_rw("planes",
            // Getter: return list of 6 planes
            [](const Frustum &f) {
                std::vector<Plane> result;
                result.reserve(6);
                for (int i = 0; i < 6; i++) {
                    result.push_back(f.planes[i]);
                }
                return result;
            },
            // Setter: accept list of 6 planes
            [](Frustum &f, const std::vector<Plane> &planes) {
                if (planes.size() != 6) {
                    throw std::invalid_argument("planes must have exactly 6 elements");
                }
                for (int i = 0; i < 6; i++) {
                    f.planes[i] = planes[i];
                }
            },
            "List of 6 frustum planes (LEFT, RIGHT, TOP, BOTTOM, NEAR, FAR)")
        .def_prop_ro_static("PLANE_LEFT", [](nb::handle) { return Frustum::PLANE_LEFT; },
                            "Left clipping plane index")
        .def_prop_ro_static("PLANE_RIGHT", [](nb::handle) { return Frustum::PLANE_RIGHT; },
                            "Right clipping plane index")
        .def_prop_ro_static("PLANE_TOP", [](nb::handle) { return Frustum::PLANE_TOP; },
                            "Top clipping plane index")
        .def_prop_ro_static("PLANE_BOTTOM", [](nb::handle) { return Frustum::PLANE_BOTTOM; },
                            "Bottom clipping plane index")
        .def_prop_ro_static("PLANE_NEAR", [](nb::handle) { return Frustum::PLANE_NEAR; },
                            "Near clipping plane index")
        .def_prop_ro_static("PLANE_FAR", [](nb::handle) { return Frustum::PLANE_FAR; },
                            "Far clipping plane index")
        .def("__repr__", [name](const Frustum &f) {
            std::ostringstream ss;
            ss << name << "(planes=[";
            for (int i = 0; i < 6; i++) {
                ss << "plane(A=" << f.planes[i].A
                   << ", B=" << f.planes[i].B
                   << ", C=" << f.planes[i].C
                   << ", D=" << f.planes[i].D << ")";
                if (i < 5) ss << ", ";
            }
            ss << "])";
            return ss.str();
        });
    // Add module-level constants for plane indices (for backward compatibility)
    // Only add these once (for the first frustum type - float)
    if (!nb::hasattr(m, "PLANE_LEFT")) {
        m.attr("PLANE_LEFT") = (int) Frustum::PLANE_LEFT;
        m.attr("PLANE_RIGHT") = (int) Frustum::PLANE_RIGHT;
        m.attr("PLANE_TOP") = (int) Frustum::PLANE_TOP;
        m.attr("PLANE_BOTTOM") = (int) Frustum::PLANE_BOTTOM;
        m.attr("PLANE_NEAR") = (int) Frustum::PLANE_NEAR;
        m.attr("PLANE_FAR") = (int) Frustum::PLANE_FAR;
    }
}

// Explicit template instantiations
template void bind_frustum<float>(nb::module_ &m, const char *name);
template void bind_frustum<double>(nb::module_ &m, const char *name);
