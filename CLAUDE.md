# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

**rove** is a C++ math and geometry library for game development and 3D applications. It provides vector math, matrix transformations, collision detection, and pathfinding utilities.

## Build System

CMake-based build. Requires Boost.

```bash
# Build
mkdir build && cd build
cmake ..
make

# Run tests
ctest
# or
./rove_tests
```

To disable tests: `cmake -DROVE_BUILD_TESTS=OFF ..`

Dependencies:
- Boost (headers + unit_test_framework + serialization for tests)

## Architecture

All code lives in `src/` under the `math` namespace.

### Core Types

- **scalar.h** - Base floating-point type (`math::scalar = float`) with constants (PI, EPSILON) and utility functions
- **vec.h** - Template vector classes `vec<N,T>` with specializations for 2D, 3D, and 4D. Supports unions for xyz/rgb/ABC component access. Operators `&` (dot product) and `^` (cross product)
- **matrix.h** - Template matrix classes with specializations for 3x3 and 4x4. Includes transformations (translation, rotation, scaling, perspective, lookat)
- **quaternion.h** - Quaternion class with SLERP interpolation

### Geometric Primitives

- **ray.h / line.h** - Ray and line segment classes
- **plane.h** - 3D plane with intersection tests
- **triangle.h** - Triangle primitive with barycentric coordinates
- **sphere.h / capsule.h** - Sphere and capsule shapes
- **aabb.h** - Axis-aligned bounding box (N-dimensional)
- **obb.h** - Oriented bounding box (2D and 3D specializations)
- **frustum.h** - View frustum for culling

### Collision & Physics

- **collision.h** - Template collision detection functions between shapes (sphere-sphere, line-sphere, obb-sphere, aabb-sphere). Returns `contact_info` with penetration depth, position, normal, and time
- **contact_info.h** - Collision result structure

### Algorithms

- **a_star.h** - A* pathfinding on 2D grids with custom scoring function
- **bresenham_supercover.h** - Line rasterization algorithm
- **heightfield.h** - Terrain heightfield with ray tracing and pathfinding integration

## Conventions

- Templates use `scalar_t` typedef for the numeric type
- N-dimensional templates use `ARITY` constant
- Vectors use packed structs with unions for flexible component access
- Russian comments appear in some files (matrix operations, OBB methods)
- Collision functions return bool and populate output parameters by reference
