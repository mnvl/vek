rove Documentation
==================

**rove** is a C++ math and geometry library for robotics and game development.
It provides high-performance vector math, matrix transformations, quaternion
operations, collision detection, and pathfinding utilities.

This documentation covers both the C++ API and the Python bindings (**pyrove**).

Features
--------

* **Fast vector operations**: 2D, 3D, and 4D vectors with NumPy integration
* **Matrix transformations**: 3x3 and 4x4 matrices for 2D/3D graphics
* **Quaternion rotations**: Smooth rotation interpolation with SLERP
* **Double precision support**: Float and double precision types
* **NumPy interoperability**: Seamless conversion to/from NumPy arrays

Quick Start
-----------

Installation
~~~~~~~~~~~~

Build and install the Python bindings:

.. code-block:: bash

   cd rove
   mkdir build && cd build
   cmake -DROVE_BUILD_PYTHON=ON ..
   make
   # The pyrove module will be in the build directory

Basic Usage
~~~~~~~~~~~

.. code-block:: python

   import pyrove

   # Create vectors
   v1 = pyrove.vec3(1.0, 2.0, 3.0)
   v2 = pyrove.vec3(4.0, 5.0, 6.0)

   # Vector operations
   dot = pyrove.dot(v1, v2)
   cross = pyrove.cross(v1, v2)
   normalized = pyrove.normalize(v1)

   # Matrices
   m = pyrove.mat4()
   m.identity()
   m.translate(1.0, 2.0, 3.0)
   m.rotate(pyrove.vec3(0.0, 1.0, 0.0), 3.14159 / 4.0)

   # Quaternions
   q = pyrove.quat(0.0, 0.0, 0.0, 1.0)
   q.normalize()

Contents
--------

.. toctree::
   :maxdepth: 2
   :caption: User Guide

   getting_started
   vectors
   matrices
   quaternions
   numpy_integration

.. toctree::
   :maxdepth: 2
   :caption: Python API Reference

   api/vectors
   api/matrices
   api/quaternions
   api/aabb
   api/obb
   api/functions

.. toctree::
   :maxdepth: 2
   :caption: C++ API Reference

   cpp_api/index

.. toctree::
   :maxdepth: 1
   :caption: Additional Information

   examples
   performance

Indices and tables
==================

* :ref:`genindex`
* :ref:`search`
