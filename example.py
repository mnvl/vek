#!/usr/bin/env python3
"""
Example script demonstrating pyrove Python bindings.

This script shows basic usage of vectors, matrices, and other geometric primitives.
"""

import pyrove

def main():
    print("=== pyrove Python Bindings Demo ===\n")

    # Vector operations
    print("1. Vector Operations")
    print("-" * 40)
    v1 = pyrove.vec3(1.0, 2.0, 3.0)
    v2 = pyrove.vec3(4.0, 5.0, 6.0)

    print(f"v1 = {v1}")
    print(f"v2 = {v2}")
    print(f"v1 + v2 = {v1 + v2}")
    print(f"v1 - v2 = {v1 - v2}")
    print(f"v1 * 2.0 = {v1 * 2.0}")
    print(f"v1.length() = {v1.length():.4f}")
    print(f"v1.dot(v2) = {v1.dot(v2):.4f}")

    # Normalization
    v3 = pyrove.vec3(3.0, 4.0, 0.0)
    print(f"\nv3 = {v3}")
    print(f"v3.length() = {v3.length():.4f}")
    v3.normalize()
    print(f"v3 after normalize() = {v3}")
    print(f"v3.length() after normalize = {v3.length():.4f}")

    # 2D vectors
    print("\n2. 2D Vectors")
    print("-" * 40)
    v2d1 = pyrove.vec2(3.0, 4.0)
    v2d2 = pyrove.vec2(1.0, 0.0)
    print(f"v2d1 = {v2d1}")
    print(f"v2d1.length() = {v2d1.length():.4f}")
    perp = v2d1.perpendicular()
    print(f"v2d1.perpendicular() = {perp}")
    print(f"v2d1.dot(perpendicular) = {v2d1.dot(perp):.4f} (should be ~0)")

    # Matrix operations
    print("\n3. Matrix Operations")
    print("-" * 40)
    mat_identity = pyrove.mat4.identity()
    print(f"Identity matrix created")

    mat_trans = pyrove.mat4.translation(1.0, 2.0, 3.0)
    print(f"Translation matrix(1, 2, 3) created")

    mat_scale = pyrove.mat4.scale(2.0, 2.0, 2.0)
    print(f"Scale matrix(2, 2, 2) created")

    # Quaternions
    print("\n4. Quaternions")
    print("-" * 40)
    q = pyrove.quat(0.0, 0.0, 0.0, 1.0)
    print(f"Quaternion: {q}")

    # Geometric primitives
    print("\n5. Geometric Primitives")
    print("-" * 40)

    # Sphere
    sphere_center = pyrove.vec3(0.0, 0.0, 0.0)
    sphere = pyrove.sphere(sphere_center, 5.0)
    print(f"Sphere at origin with radius 5.0 created")

    # Ray
    ray_origin = pyrove.vec3(0.0, 0.0, -10.0)
    ray_dir = pyrove.vec3(0.0, 0.0, 1.0)
    ray = pyrove.ray3(ray_origin, ray_dir)
    print(f"Ray from {ray_origin} along {ray_dir} created")

    # AABB
    aabb_min = pyrove.vec3(-1.0, -1.0, -1.0)
    aabb_max = pyrove.vec3(1.0, 1.0, 1.0)
    aabb = pyrove.aabb3(aabb_min, aabb_max)
    print(f"AABB from {aabb_min} to {aabb_max} created")

    print("\n=== Demo Complete ===")
    print("\nFor more information, see PYTHON_INSTALL.md")

if __name__ == "__main__":
    main()
