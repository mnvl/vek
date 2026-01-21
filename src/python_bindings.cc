/**
 * @file python_bindings.cc
 * @brief Main Python module definition for pyrove
 *
 * This file defines the pyrove Python module and registers all bindings
 * from separate binding files (bind_vec.cc, bind_matrix.cc,
 * bind_quaternion.cc).
 */

#include "python_bindings.h"
#include <nanobind/nanobind.h>

namespace nb = nanobind;

/**
 * @brief Python module entry point
 *
 * Defines the pyrove module and registers all class bindings and
 * free functions. The actual binding implementations are in separate
 * files for better organization.
 */
NB_MODULE(pyrove, m) {
    m.doc() = "Python bindings for rove vector math library";

    // Bind vector classes (float versions)
    bind_vec2<float>(m, "vec2");
    bind_vec3<float>(m, "vec3");
    bind_vec4<float>(m, "vec4");

    // Bind vector classes (double versions)
    bind_vec2<double>(m, "vec2d");
    bind_vec3<double>(m, "vec3d");
    bind_vec4<double>(m, "vec4d");

    // Bind vector free functions
    bind_vec_functions(m);

    // Bind matrix classes (float versions)
    bind_matrix3<float>(m, "mat3");
    bind_matrix4<float>(m, "mat4");

    // Bind matrix classes (double versions)
    bind_matrix3<double>(m, "mat3d");
    bind_matrix4<double>(m, "mat4d");

    // Bind quaternion classes (float versions)
    bind_quaternion<float>(m, "quat");
    bind_quaternion_slerper<float>(m, "quat_slerper");

    // Bind quaternion classes (double versions)
    bind_quaternion<double>(m, "quatd");
    bind_quaternion_slerper<double>(m, "quat_slerperd");

    // Bind quaternion free functions
    bind_quaternion_functions(m);

    // Bind sphere classes (float versions)
    bind_sphere2<float>(m, "sphere2");
    bind_sphere3<float>(m, "sphere3");

    // Bind sphere classes (double versions)
    bind_sphere2<double>(m, "sphere2d");
    bind_sphere3<double>(m, "sphere3d");

    // Bind AABB classes (float versions)
    bind_aabb2<float>(m, "aabb2");
    bind_aabb3<float>(m, "aabb3");

    // Bind AABB classes (double versions)
    bind_aabb2<double>(m, "aabb2d");
    bind_aabb3<double>(m, "aabb3d");

    // Bind OBB classes (float versions)
    bind_obb2<float>(m, "obb2");
    bind_obb3<float>(m, "obb3");

    // Bind OBB classes (double versions)
    bind_obb2<double>(m, "obb2d");
    bind_obb3<double>(m, "obb3d");
}
