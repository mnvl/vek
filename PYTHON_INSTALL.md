# Python Installation Guide

This guide explains how to install and use the Python bindings for rove.

## Prerequisites

- Python 3.8 or higher
- pip
- CMake 3.15 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Boost headers (for the C++ library)

On Ubuntu/Debian:
```bash
sudo apt-get install python3-dev cmake libboost-dev
```

On macOS:
```bash
brew install python cmake boost
```

## Installation

### Option 1: Install from source (local development)

```bash
# Install build dependencies
pip3 install scikit-build-core nanobind

# Install the package
pip3 install .
```

### Option 2: Editable/development install

For development, you can install in editable mode:

```bash
pip3 install -e .
```

This allows you to modify the C++ code and rebuild without reinstalling.

### Option 3: Build wheel for distribution

To create a wheel package:

```bash
pip3 install build
python3 -m build
```

This will create a wheel file in the `dist/` directory that can be installed with:

```bash
pip3 install dist/pyrove-1.0.0-*.whl
```

## Verifying Installation

After installation, verify it works:

```python
import pyrove

# Create a 3D vector
v = pyrove.vec3(1.0, 2.0, 3.0)
print(v)  # Output: vec3(1, 2, 3)

# Calculate length
print(v.length())

# Normalize the vector
v.normalize()
print(v)
```

## Usage Example

```python
import pyrove

# Vector operations
v1 = pyrove.vec3(1.0, 0.0, 0.0)
v2 = pyrove.vec3(0.0, 1.0, 0.0)
v3 = v1 + v2
print(v3)  # vec3(1, 1, 0)

# Matrix transformations
mat = pyrove.mat4.identity()
mat = pyrove.mat4.translation(1.0, 2.0, 3.0)

# Quaternions
q = pyrove.quat(0.0, 0.0, 0.0, 1.0)

# Collision detection
sphere1 = pyrove.sphere(pyrove.vec3(0, 0, 0), 1.0)
sphere2 = pyrove.sphere(pyrove.vec3(1.5, 0, 0), 1.0)
# contact = rove.collide(sphere1, sphere2)

# Pathfinding
hf = pyrove.heightfield(100, 100, 1.0)
# path = rove.a_star(start, goal, hf)
```

## Troubleshooting

### ImportError: Failed to import the pyrove C++ extension module

Make sure:
1. The package was installed correctly: `pip3 list | grep pyrove`
2. Boost is installed on your system
3. Your Python version is compatible (3.8+)

### Build errors

If you encounter build errors:
1. Ensure CMake is version 3.15 or higher: `cmake --version`
2. Ensure Boost is installed: `dpkg -l | grep libboost` (Ubuntu) or `brew list | grep boost` (macOS)
3. Check that you have a C++17 compatible compiler

### Missing nanobind

If you get errors about nanobind:
```bash
pip3 install nanobind>=2.0.0
```

## Uninstalling

```bash
pip3 uninstall pyrove
```

## Additional Notes

- The Python bindings use nanobind for efficient C++/Python interop
- NumPy integration is supported via `from_numpy()` and `to_numpy()` methods
- The C++ extension module is named `pyrove` and imported as `pyrove`
