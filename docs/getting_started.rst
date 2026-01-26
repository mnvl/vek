Getting Started
===============

Installation
------------

Prerequisites
~~~~~~~~~~~~~

* CMake 3.15 or later
* C++17 compatible compiler
* Python 3.8 or later
* NumPy (optional, for array conversions)
* Boost (headers + libraries)

Building from Source
~~~~~~~~~~~~~~~~~~~~

1. Clone the repository:

   .. code-block:: bash

      git clone https://github.com/mnvl/rove.git
      cd rove

2. Build with Python bindings enabled:

   .. code-block:: bash

      mkdir build
      cd build
      cmake -DROVE_BUILD_PYTHON=ON ..
      make

3. The ``pyrove`` module will be available in the ``build/`` directory.

4. To use it, either:

   * Add the build directory to your ``PYTHONPATH``
   * Copy the ``.so`` file to your Python site-packages
   * Install using pip (if setup.py is available)

First Steps
-----------

Import the Module
~~~~~~~~~~~~~~~~~

.. code-block:: python

   import pyrove

   # Check available types
   print(dir(pyrove))

Create Vectors
~~~~~~~~~~~~~~

.. code-block:: python

   # 2D vector
   v2 = pyrove.vec2(1.0, 2.0)
   print(v2)  # vec2(1.0, 2.0)

   # 3D vector
   v3 = pyrove.vec3(1.0, 2.0, 3.0)
   print(v3.length())  # 3.7416...

   # 4D vector
   v4 = pyrove.vec4(1.0, 2.0, 3.0, 1.0)

   # Component access
   print(v3.x, v3.y, v3.z)  # 1.0 2.0 3.0

Vector Operations
~~~~~~~~~~~~~~~~~

.. code-block:: python

   v1 = pyrove.vec3(1.0, 0.0, 0.0)
   v2 = pyrove.vec3(0.0, 1.0, 0.0)

   # Arithmetic
   v3 = v1 + v2          # Addition
   v4 = v1 - v2          # Subtraction
   v5 = v1 * 2.0         # Scalar multiplication
   v6 = v1 / 2.0         # Scalar division

   # Dot product
   dot = pyrove.dot(v1, v2)     # 0.0
   dot = v1.dot(v2)             # Also works

   # Cross product (3D only)
   cross = pyrove.cross(v1, v2) # vec3(0, 0, 1)
   cross = v1.cross(v2)         # Also works

   # Normalization
   v_norm = pyrove.normalize(v1)
   v1.normalize()  # In-place

Create Matrices
~~~~~~~~~~~~~~~

.. code-block:: python

   # 4x4 matrix
   m = pyrove.mat4()
   m.identity()

   # Transformations
   m.translate(1.0, 2.0, 3.0)
   m.rotate(pyrove.vec3(0.0, 1.0, 0.0), 1.57)  # Rotate 90° around Y
   m.scale(2.0, 2.0, 2.0)

   # Matrix multiplication
   m1 = pyrove.mat4()
   m2 = pyrove.mat4()
   m3 = m1 * m2

Matrix Operations
~~~~~~~~~~~~~~~~~

.. code-block:: python

   m = pyrove.mat4()
   m.identity()

   # Inverse and transpose
   m_inv = m.inverse()
   m_trans = m.transpose()

   # Determinant
   det = m.determinant()

   # Transform vectors
   v = pyrove.vec3(1.0, 2.0, 3.0)
   v_transformed = m * v

Create Quaternions
~~~~~~~~~~~~~~~~~~

.. code-block:: python

   # Identity quaternion
   q = pyrove.quat(0.0, 0.0, 0.0, 1.0)
   q.identity()

   # Normalize
   q.normalize()

   # Spherical linear interpolation (SLERP)
   q1 = pyrove.quat(0.0, 0.0, 0.0, 1.0)
   q2 = pyrove.quat(0.0, 0.707, 0.0, 0.707)
   q_mid = pyrove.slerp(q1, q2, 0.5)

Geometric Primitives
~~~~~~~~~~~~~~~~~~~~

The library includes comprehensive geometric primitive support:

.. code-block:: python

   # Rays for ray casting
   ray = pyrove.ray3(pyrove.vec3(0, 0, 0), pyrove.vec3(1, 0, 0))
   point = ray.apply(5.0)  # Get point at t=5

   # Line segments
   line = pyrove.line3(pyrove.vec3(0, 0, 0), pyrove.vec3(10, 0, 0))
   dist = line.distance(pyrove.vec3(5, 3, 0))

   # Planes with intersection tests
   plane = pyrove.plane(pyrove.vec3(0, 0, 0), pyrove.vec3(0, 1, 0))
   if plane.test_intersection(ray):
       t = plane.trace(ray)

   # Triangles with geometric properties
   tri = pyrove.triangle3(
       pyrove.vec3(0, 0, 0),
       pyrove.vec3(1, 0, 0),
       pyrove.vec3(0, 1, 0)
   )
   area = tri.area()
   centroid = tri.cog()

   # Capsules for collision detection
   cap = pyrove.capsule3(pyrove.vec3(0, 0, 0), pyrove.vec3(0, 2, 0), 0.5)
   if cap.contains(pyrove.vec3(0, 1, 0.3)):
       print("Inside capsule")

   # Frustum culling
   frustum = pyrove.frustum(view_proj_matrix)
   if frustum.test_intersection(bbox):
       print("Object visible")

Double Precision
~~~~~~~~~~~~~~~~

All types have double-precision variants:

.. code-block:: python

   # Double precision vectors
   dv2 = pyrove.dvec2(1.0, 2.0)
   dv3 = pyrove.dvec3(1.0, 2.0, 3.0)
   dv4 = pyrove.dvec4(1.0, 2.0, 3.0, 1.0)

   # Double precision matrices
   dm3 = pyrove.dmat3()
   dm4 = pyrove.dmat4()

   # Double precision quaternions
   dq = pyrove.dquat(0.0, 0.0, 0.0, 1.0)

   # Geometric primitives also have double variants
   ray_d = pyrove.ray3d(pyrove.vec3d(0, 0, 0), pyrove.vec3d(1, 0, 0))
   tri_d = pyrove.triangle3d(v1d, v2d, v3d)

NumPy Integration
~~~~~~~~~~~~~~~~~

Convert between pyrove and NumPy:

.. code-block:: python

   import numpy as np
   import pyrove

   # NumPy to pyrove
   arr = np.array([1.0, 2.0, 3.0], dtype=np.float32)
   v = pyrove.vec3.from_numpy(arr)

   # pyrove to NumPy
   v = pyrove.vec3(1.0, 2.0, 3.0)
   arr = v.to_numpy()

   # Matrices
   mat_array = np.eye(4, dtype=np.float32)
   m = pyrove.mat4.from_numpy(mat_array)
   mat_back = m.to_numpy()

Next Steps
----------

* Learn more about :doc:`vectors`
* Explore :doc:`matrices` for 3D transformations
* Understand :doc:`quaternions` for smooth rotations
* Discover :doc:`geometric_primitives` for ray casting, collision detection, and more
* See :doc:`examples` for practical use cases
