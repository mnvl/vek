Performance Tips
================

Optimize your pyrove code for best performance.

General Guidelines
------------------

1. **Use appropriate precision**: Float is usually faster than double
2. **Avoid repeated conversions**: Minimize NumPy ↔ pyrove conversions
3. **Batch operations**: Use NumPy for bulk processing
4. **Prefer squared lengths**: ``length_sq()`` avoids expensive sqrt
5. **Reuse objects**: Pre-allocate matrices and vectors when possible

Vector Operations
-----------------

Fast Length Comparisons
~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: python

   # SLOW - calculates sqrt twice
   if v1.length() < v2.length():
       pass

   # FAST - compares squared lengths
   if v1.length_sq() < v2.length_sq():
       pass

Normalize Once
~~~~~~~~~~~~~~

.. code-block:: python

   # SLOW - normalizes every frame
   for frame in range(1000):
       v = pyrove.vec3(1, 2, 3)
       v.normalize()
       use(v)

   # FAST - normalize outside loop
   v = pyrove.vec3(1, 2, 3)
   v.normalize()
   for frame in range(1000):
       use(v)

Matrix Operations
-----------------

Reuse Matrices
~~~~~~~~~~~~~~

.. code-block:: python

   # SLOW - creates new matrix each time
   def transform_point(p):
       m = pyrove.mat4()
       m.identity()
       m.translate(1, 2, 3)
       return m * p

   # FAST - create matrix once
   transform_matrix = pyrove.mat4()
   transform_matrix.identity()
   transform_matrix.translate(1, 2, 3)

   def transform_point(p):
       return transform_matrix * p

Combine Transformations
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: python

   # SLOW - transforms 1000 points individually
   points = [pyrove.vec3(i, i, i) for i in range(1000)]
   for p in points:
       m1 = pyrove.mat4()
       m1.translate(1, 0, 0)
       temp = m1 * p

       m2 = pyrove.mat4()
       m2.rotate(pyrove.vec3(0, 1, 0), 1.57)
       result = m2 * temp

   # FAST - pre-combine matrices
   combined = pyrove.mat4()
   combined.identity()
   combined.translate(1, 0, 0)
   combined.rotate(pyrove.vec3(0, 1, 0), 1.57)

   for p in points:
       result = combined * p

NumPy Integration
-----------------

Batch Processing
~~~~~~~~~~~~~~~~

.. code-block:: python

   import numpy as np
   import pyrove

   # SLOW - individual operations
   vectors = [pyrove.vec3(i, i, i) for i in range(10000)]
   for v in vectors:
       v.normalize()
       v *= 2.0

   # FAST - use NumPy for bulk operations
   arr = np.array([[i, i, i] for i in range(10000)], dtype=np.float32)
   norms = np.linalg.norm(arr, axis=1, keepdims=True)
   arr /= norms
   arr *= 2.0
   vectors = [pyrove.vec3.from_numpy(arr[i]) for i in range(len(arr))]

Minimize Conversions
~~~~~~~~~~~~~~~~~~~~

.. code-block:: python

   # SLOW - converts every iteration
   for frame in range(1000):
       v = pyrove.vec3(1, 2, 3)
       arr = v.to_numpy()
       # ... numpy operations ...
       v = pyrove.vec3.from_numpy(arr)

   # FAST - stay in NumPy
   arr = np.array([1, 2, 3], dtype=np.float32)
   for frame in range(1000):
       # ... numpy operations on arr ...
   v = pyrove.vec3.from_numpy(arr)

Quaternions
-----------

Reuse SLERP Objects
~~~~~~~~~~~~~~~~~~~

.. code-block:: python

   # SLOW - creates slerper every time
   def interpolate_rotation(q1, q2, t):
       return pyrove.slerp(q1, q2, t)

   for t in np.linspace(0, 1, 100):
       q = interpolate_rotation(q_start, q_end, t)

   # FAST - create slerper once
   slerper = pyrove.quat_slerper()
   slerper.setup(q_start, q_end)

   for t in np.linspace(0, 1, 100):
       q = slerper.interpolate(t)

Memory Management
-----------------

Object Pooling
~~~~~~~~~~~~~~

.. code-block:: python

   class VectorPool:
       def __init__(self, size):
           self.pool = [pyrove.vec3() for _ in range(size)]
           self.available = list(range(size))

       def get(self):
           if self.available:
               return self.pool[self.available.pop()]
           return pyrove.vec3()  # Fallback

       def release(self, index):
           self.available.append(index)

   # Usage
   pool = VectorPool(1000)
   v = pool.get()
   # ... use v ...
   pool.release(0)

Profiling
---------

Use Python's profiler to identify bottlenecks:

.. code-block:: python

   import cProfile
   import pyrove

   def my_function():
       for i in range(10000):
           v = pyrove.vec3(i, i, i)
           v.normalize()
           length = v.length()

   cProfile.run('my_function()')

Benchmarking Example
--------------------

.. code-block:: python

   import pyrove
   import numpy as np
   import time

   def benchmark_vector_normalize():
       # pyrove method
       vectors = [pyrove.vec3(i, i, i) for i in range(10000)]

       start = time.time()
       for v in vectors:
           v.normalize()
       pyrove_time = time.time() - start

       # NumPy method
       arr = np.array([[i, i, i] for i in range(10000)], dtype=np.float32)

       start = time.time()
       norms = np.linalg.norm(arr, axis=1, keepdims=True)
       arr /= norms
       numpy_time = time.time() - start

       print(f"pyrove: {pyrove_time:.4f}s")
       print(f"NumPy:  {numpy_time:.4f}s")
       print(f"Speedup: {pyrove_time/numpy_time:.2f}x")

   benchmark_vector_normalize()

Key Takeaways
-------------

1. **Profile first**: Don't optimize without measuring
2. **Use NumPy for large datasets**: 1000+ vectors/matrices
3. **Use pyrove for small operations**: Individual transforms, gameplay logic
4. **Cache expensive operations**: Matrix inverses, normalization
5. **Avoid unnecessary precision**: Use float unless you need double
