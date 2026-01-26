Performance Benchmarks
======================

Detailed performance comparison between pyrove and NumPy.

Benchmark Environment
---------------------

* **Build**: Release with ``-O3 -march=native -flto``
* **Python**: 3.14.2
* **NumPy**: 2.4.1
* **Platform**: macOS ARM64

Summary Results
---------------

pyrove wins **7 out of 19** benchmarks (36.8%), with particular strengths in
geometric operations and specialized matrix computations.

pyrove Advantages
~~~~~~~~~~~~~~~~~

Operations where pyrove is faster:

.. list-table::
   :header-rows: 1
   :widths: 40 15 15 15

   * - Operation
     - pyrove
     - NumPy
     - Speedup
   * - Cross Product
     - 2.70 µs
     - 11.98 µs
     - **4.4x faster**
   * - Triangle Area
     - 3.66 µs
     - 14.08 µs
     - **3.9x faster**
   * - Matrix Inverse
     - 1.94 µs
     - 4.29 µs
     - **2.2x faster**
   * - Vector Normalization
     - 0.85 µs
     - 1.54 µs
     - **1.8x faster**
   * - Vector Length
     - 0.86 µs
     - 1.16 µs
     - **1.4x faster**
   * - Matrix Creation
     - 0.74 µs
     - 0.84 µs
     - **1.1x faster**

NumPy Advantages
~~~~~~~~~~~~~~~~

Operations where NumPy is faster:

.. list-table::
   :header-rows: 1
   :widths: 40 15 15 15

   * - Operation
     - pyrove
     - NumPy
     - Speedup
   * - Array Conversions
     - 2.08 µs
     - 0.34 µs
     - **6.1x faster**
   * - Vector Addition
     - 2.77 µs
     - 0.91 µs
     - **3.0x faster**
   * - Ray-Plane Intersection
     - 4.58 µs
     - 1.51 µs
     - **3.0x faster**
   * - Matrix-Vector Mult
     - 3.01 µs
     - 1.63 µs
     - **1.8x faster**

When to Use pyrove
------------------

Choose pyrove for:

* **Geometric operations**: Cross products, triangle calculations
* **3D graphics**: Transformations, quaternions, view frustums
* **Robotics**: Kinematics, dynamics computations
* **Game development**: Physics, collision detection
* **Ray tracing**: Ray-triangle intersection, BVH traversal
* **Tight loops**: Small fixed-size vectors and matrices

Example use case:

.. code-block:: python

   import pyrove

   # Geometric calculations - pyrove is 3.9x faster
   tri = pyrove.triangle3(v1, v2, v3)
   area = tri.area()
   normal = tri.get_normal()

   # Cross products - pyrove is 4.4x faster
   v1 = pyrove.vec3(1, 2, 3)
   v2 = pyrove.vec3(4, 5, 6)
   perpendicular = v1.cross(v2)

When to Use NumPy
-----------------

Choose NumPy for:

* **Large arrays**: Batch operations on many vectors
* **Scientific computing**: Statistical analysis, signal processing
* **Machine learning**: Matrix operations on large datasets
* **Vectorized operations**: Operating on thousands of elements
* **Data analysis**: Working with tabular data

Example use case:

.. code-block:: python

   import numpy as np

   # Batch operations - NumPy excels here
   points = np.random.rand(10000, 3)
   translated = points + offset  # 3x faster than pyrove loop

Mixed Usage Pattern
-------------------

For best performance, use both libraries where each excels:

.. code-block:: python

   import numpy as np
   import pyrove

   # Batch preprocessing in NumPy
   vertices = np.random.rand(1000, 3).astype(np.float32)
   centered = vertices - vertices.mean(axis=0)

   # Geometric calculations in pyrove
   total_area = 0.0
   for i in range(0, len(centered), 3):
       v1 = pyrove.vec3(*centered[i])
       v2 = pyrove.vec3(*centered[i+1])
       v3 = pyrove.vec3(*centered[i+2])

       tri = pyrove.triangle3(v1, v2, v3)
       total_area += tri.area()  # 3.9x faster than NumPy

Optimization Impact
-------------------

The optimized Release build provides:

* **Binary size**: 527 KB (vs 4.4 MB debug) - 8.4x smaller
* **Performance**: 7-10x faster than debug builds
* **Compilation**: ``-O3 -march=native -flto``

Running Benchmarks
------------------

To reproduce these results:

.. code-block:: bash

   # Build optimized version
   ./build_release.sh

   # Run benchmark
   ./run_benchmark.sh

   # Or manually
   PYTHONPATH=build-release python3 benchmark.py

See :doc:`performance` for optimization tips and build instructions.

Complete Results
----------------

For detailed benchmark results, methodology, and analysis, see the
`BENCHMARKS.md <https://github.com/mnvl/rove/blob/main/BENCHMARKS.md>`_
file in the repository.
