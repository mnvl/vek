NumPy Integration
=================

pyrove provides seamless integration with NumPy for array operations.

Converting to NumPy
-------------------

All vector, matrix, and quaternion types can be converted to NumPy arrays.

Vectors
~~~~~~~

.. code-block:: python

   import pyrove
   import numpy as np

   v = pyrove.vec3(1.0, 2.0, 3.0)
   arr = v.to_numpy()

   print(arr.shape)  # (3,)
   print(arr.dtype)  # float32
   print(arr)  # [1. 2. 3.]

Matrices
~~~~~~~~

.. code-block:: python

   m = pyrove.mat4()
   m.identity()
   arr = m.to_numpy()

   print(arr.shape)  # (4, 4)
   print(arr.dtype)  # float32

Quaternions
~~~~~~~~~~~

.. code-block:: python

   q = pyrove.quat(0.0, 0.707, 0.0, 0.707)
   arr = q.to_numpy()

   print(arr.shape)  # (4,)
   print(arr)  # [0.    0.707 0.    0.707]

Converting from NumPy
---------------------

Use the ``from_numpy`` static method to create pyrove objects from NumPy arrays.

Vectors
~~~~~~~

.. code-block:: python

   import pyrove
   import numpy as np

   # From NumPy array
   arr = np.array([1.0, 2.0, 3.0], dtype=np.float32)
   v = pyrove.vec3.from_numpy(arr)

   print(v)  # vec3(1.0, 2.0, 3.0)

Matrices
~~~~~~~~

.. code-block:: python

   # Create identity matrix in NumPy
   arr = np.eye(4, dtype=np.float32)
   m = pyrove.mat4.from_numpy(arr)

   # Create transformation matrix
   arr = np.array([
       [1, 0, 0, 5],
       [0, 1, 0, 0],
       [0, 0, 1, 0],
       [0, 0, 0, 1]
   ], dtype=np.float32)
   m = pyrove.mat4.from_numpy(arr)

Quaternions
~~~~~~~~~~~

.. code-block:: python

   # [x, y, z, w] format
   arr = np.array([0.0, 0.707, 0.0, 0.707], dtype=np.float32)
   q = pyrove.quat.from_numpy(arr)

Bulk Operations
---------------

For processing many vectors, convert to NumPy, perform operations, then convert back.

.. code-block:: python

   import pyrove
   import numpy as np

   # Create many vectors
   vectors = [pyrove.vec3(i, i*2, i*3) for i in range(1000)]

   # Convert to NumPy array
   arr = np.array([v.to_numpy() for v in vectors])
   print(arr.shape)  # (1000, 3)

   # Perform NumPy operations
   normalized = arr / np.linalg.norm(arr, axis=1, keepdims=True)

   # Convert back
   vectors_normalized = [
       pyrove.vec3.from_numpy(normalized[i])
       for i in range(len(normalized))
   ]

Working with Arrays of Transformations
---------------------------------------

.. code-block:: python

   import pyrove
   import numpy as np

   # Create array of transformation matrices
   num_objects = 100
   matrices = []

   for i in range(num_objects):
       m = pyrove.mat4()
       m.identity()
       m.translate(i * 2.0, 0.0, 0.0)
       matrices.append(m.to_numpy())

   # Stack into 3D array
   matrix_array = np.stack(matrices)
   print(matrix_array.shape)  # (100, 4, 4)

   # Perform batch operations
   # ... process with NumPy ...

   # Convert back
   transformed_matrices = [
       pyrove.mat4.from_numpy(matrix_array[i])
       for i in range(num_objects)
   ]

Precision Considerations
------------------------

Float vs Double
~~~~~~~~~~~~~~~

.. code-block:: python

   import pyrove
   import numpy as np

   # Float precision (default)
   arr_f32 = np.array([1.0, 2.0, 3.0], dtype=np.float32)
   v_float = pyrove.vec3.from_numpy(arr_f32)

   # Double precision
   arr_f64 = np.array([1.0, 2.0, 3.0], dtype=np.float64)
   v_double = pyrove.dvec3.from_numpy(arr_f64)

Conversion Performance
~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: python

   # Avoid repeated conversions in loops
   # BAD:
   for i in range(1000):
       v = pyrove.vec3(i, i, i)
       arr = v.to_numpy()  # Expensive
       # ... process arr ...

   # GOOD:
   # Process in NumPy, convert once at the end
   arr = np.array([[i, i, i] for i in range(1000)])
   # ... process arr ...
   vectors = [pyrove.vec3.from_numpy(arr[i]) for i in range(len(arr))]

Example: Particle System
-------------------------

.. code-block:: python

   import pyrove
   import numpy as np

   class ParticleSystem:
       def __init__(self, num_particles):
           # Store positions as NumPy array for bulk operations
           self.positions = np.random.rand(num_particles, 3).astype(np.float32)
           self.velocities = np.random.rand(num_particles, 3).astype(np.float32)

       def update(self, dt):
           # Update using NumPy (fast)
           self.positions += self.velocities * dt

           # Bounds checking
           self.positions = np.clip(self.positions, -10, 10)

       def get_particle(self, index):
           # Convert single particle to pyrove when needed
           return pyrove.vec3.from_numpy(self.positions[index])

       def transform_all(self, matrix):
           # Transform all particles at once
           # Convert matrix to NumPy
           m_np = matrix.to_numpy()[:3, :3]  # 3x3 rotation part

           # Apply transformation
           self.positions = self.positions @ m_np.T

   # Usage
   particles = ParticleSystem(10000)
   particles.update(0.016)  # Update at 60 FPS

   # Transform all particles
   transform = pyrove.mat4()
   transform.rotate(pyrove.vec3(0, 1, 0), 0.1)
   particles.transform_all(transform)
