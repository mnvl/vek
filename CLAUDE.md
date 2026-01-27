# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**rove** is a C++17 math and geometry library for robotics and game development. It provides optimized fixed-size vector, matrix, quaternion, and geometric primitive types with template-based dual-precision (float/double) support. Python bindings (`pyrove`) are available via nanobind.

All C++ code lives in the `rove` namespace. The default scalar type is `float` (typedef'd as `scalar` in `scalar.h`). Templates use `T` for scalar type and `N` for dimensionality (e.g., `vec<3,float>`, `aabb<2,double>`).

## Build Commands

### C++ library and tests (default build)
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DROVE_BUILD_TESTS=ON ..
make -j$(sysctl -n hw.ncpu)
```

### Run all C++ tests
```bash
cd build && ctest
# or directly:
cd build && ./rove_tests
```

### Run a single C++ test suite
```bash
cd build && ./rove_tests --run_test=vec_test
# Available suites: vec_test, matrix_test, quaternion_test, ray_test, plane_test,
# triangle_test, sphere_test, aabb_test, obb_test, capsule_test, frustum_test,
# collision_test, a_star_test, bresenham_test, heightfield_test
```

### Build with Python bindings
```bash
cmake -DCMAKE_BUILD_TYPE=Release -DROVE_BUILD_PYTHON=ON ..
make -j$(sysctl -n hw.ncpu)
```

### Run Python tests (from build directory)
```bash
python3 ../src/test_pyrove.py
```

### Convenience scripts
- `./build_release.sh` — optimized release build with `-march=native` and Python bindings (no tests)
- `./build_debug.sh` — debug build with tests and Python bindings

### CMake options
| Option | Default | Description |
|--------|---------|-------------|
| `ROVE_BUILD_TESTS` | ON | Build C++ test suite (requires Boost.Test + Boost.Serialization) |
| `ROVE_BUILD_PYTHON` | OFF | Build Python bindings (fetches nanobind v2.4.0) |
| `ROVE_BUILD_DOCS` | OFF | Build Doxygen + Sphinx documentation |

## Architecture

### Source layout
All source code is in `src/`. Each geometric type has a header (`.h`), implementation (`.cc`), C++ test (`test_*.cc`), Python binding (`bind_*.cc`), and Python test (`test_*.py`).

### Template design
Classes are templates parameterized by dimension `N` and scalar type `T`:
- `vec<N,T>` with specializations for N=2,3,4
- `matrix<M,N,T>` with specializations for 3x3 and 4x4
- `quaternion<T>` with SLERP support
- Geometric primitives: `ray<N,T>`, `line<N,T>`, `sphere<N,T>`, `aabb<N,T>`, `obb<N,T>`, `capsule<N,T>`, `triangle<N,T>`, `plane<T>`, `frustum<T>`

Vectors provide union-based multi-name component access (`.x/.y/.z/.w`, `.r/.g/.b/.a`, `.u/.v/.s/.t`, `.A/.B/.C/.D`, `.i[]` array).

Matrices use column-major storage (OpenGL convention), accessed via `ij[column][row]` or named members `_11`, `_12`, etc.

### Python bindings
Entry point: `src/python_bindings.cc`. Each type's bindings live in `src/bind_*.cc`. Both float and double versions are exposed (e.g., `vec3` / `vec3d`, `mat4` / `mat4d`). All types support NumPy conversion via `from_numpy()` / `to_numpy()`.

### Collision system
Collision queries are in `src/collision.h`. They return `bool` for simple intersection tests and populate `contact_info<N,T>` structs for detailed contact data. Supports static and moving object collision.

### Algorithms
- `a_star.h` — grid-based A* pathfinding
- `bresenham_supercover.h` — line rasterization
- `heightfield.h` — terrain heightfield with ray tracing

## Dependencies

- **C++ (required):** Boost headers (used for serialization support and test framework)
- **Tests:** Boost.Test (`unit_test_framework`) + Boost.Serialization
- **Python bindings:** nanobind v2.4.0 (auto-fetched by CMake), Python 3.8+
- **Docs:** Doxygen + Sphinx + Breathe
