Working with Vectors
====================

Vector classes are the foundation of pyrove, providing efficient N-dimensional
vector operations for 2D, 3D, and 4D spaces.

Creating Vectors
----------------

Basic Creation
~~~~~~~~~~~~~~

.. code-block:: python

   import pyrove

   # 2D vector
   v2 = pyrove.vec2(1.0, 2.0)

   # 3D vector
   v3 = pyrove.vec3(1.0, 2.0, 3.0)

   # 4D vector (homogeneous coordinates)
   v4 = pyrove.vec4(1.0, 2.0, 3.0, 1.0)

   # Zero vectors
   zero2 = pyrove.vec2()
   zero3 = pyrove.vec3()

Component Access
~~~~~~~~~~~~~~~~

Vectors support multiple naming conventions for components:

.. code-block:: python

   v = pyrove.vec3(1.0, 2.0, 3.0)

   # Cartesian coordinates
   print(v.x, v.y, v.z)  # 1.0 2.0 3.0

   # RGB color (same memory)
   print(v.r, v.g, v.b)  # 1.0 2.0 3.0

   # Generic labels
   print(v.A, v.B, v.C)  # 1.0 2.0 3.0

Vector Arithmetic
-----------------

Basic Operations
~~~~~~~~~~~~~~~~

.. code-block:: python

   v1 = pyrove.vec3(1.0, 2.0, 3.0)
   v2 = pyrove.vec3(4.0, 5.0, 6.0)

   # Addition and subtraction
   v3 = v1 + v2  # vec3(5, 7, 9)
   v4 = v1 - v2  # vec3(-3, -3, -3)

   # Scalar multiplication
   v5 = v1 * 2.0    # vec3(2, 4, 6)
   v6 = 2.0 * v1    # vec3(2, 4, 6) - commutative

   # Scalar division
   v7 = v1 / 2.0    # vec3(0.5, 1.0, 1.5)

   # Component-wise operations
   v8 = v1 * v2     # vec3(4, 10, 18)
   v9 = v1 / v2     # vec3(0.25, 0.4, 0.5)

   # Negation
   v10 = -v1        # vec3(-1, -2, -3)

In-Place Operations
~~~~~~~~~~~~~~~~~~~

.. code-block:: python

   v = pyrove.vec3(1.0, 2.0, 3.0)

   # Modify in place
   v.set(4.0, 5.0, 6.0)
   v.set_all(1.0)  # Set all components to 1.0

   # In-place arithmetic
   v.normalize()
   v.negate()

Vector Products
---------------

Dot Product
~~~~~~~~~~~

The dot product measures how parallel two vectors are:

.. code-block:: python

   import pyrove

   v1 = pyrove.vec3(1.0, 0.0, 0.0)
   v2 = pyrove.vec3(0.0, 1.0, 0.0)

   # Perpendicular vectors
   dot = pyrove.dot(v1, v2)  # 0.0

   v3 = pyrove.vec3(2.0, 0.0, 0.0)
   dot = pyrove.dot(v1, v3)  # 2.0 - parallel

   # Also available as method
   dot = v1.dot(v2)

Cross Product (3D)
~~~~~~~~~~~~~~~~~~

The cross product creates a vector perpendicular to both inputs:

.. code-block:: python

   import pyrove

   x_axis = pyrove.vec3(1.0, 0.0, 0.0)
   y_axis = pyrove.vec3(0.0, 1.0, 0.0)

   # Cross product gives Z axis
   z_axis = pyrove.cross(x_axis, y_axis)  # vec3(0, 0, 1)

   # Also available as method
   z_axis = x_axis.cross(y_axis)

   # Right-hand rule applies
   reverse = pyrove.cross(y_axis, x_axis)  # vec3(0, 0, -1)

Length and Normalization
-------------------------

Vector Length
~~~~~~~~~~~~~

.. code-block:: python

   v = pyrove.vec3(3.0, 4.0, 0.0)

   # Euclidean length
   length = v.length()  # 5.0

   # Squared length (faster - avoids sqrt)
   length_sq = v.length_sq()  # 25.0

   # Manhattan distance (for vec2 only)
   v2 = pyrove.vec2(3.0, 4.0)
   manhattan = v2.length_manhattan()  # 7.0

Normalization
~~~~~~~~~~~~~

.. code-block:: python

   v = pyrove.vec3(3.0, 4.0, 0.0)

   # Normalize in place
   v.normalize()
   print(v)  # vec3(0.6, 0.8, 0)
   print(v.length())  # 1.0

   # Or create normalized copy
   v = pyrove.vec3(3.0, 4.0, 0.0)
   v_norm = pyrove.normalize(v)

Special Operations
------------------

Perpendicular Vectors (2D)
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: python

   v = pyrove.vec2(1.0, 0.0)
   perp = v.perpendicular()  # vec2(0, -1) - rotated 90° CCW

Collinearity Test
~~~~~~~~~~~~~~~~~

.. code-block:: python

   v1 = pyrove.vec3(1.0, 0.0, 0.0)
   v2 = pyrove.vec3(2.0, 0.0, 0.0)
   v3 = pyrove.vec3(0.0, 1.0, 0.0)

   # Check if vectors are parallel
   v1.is_collinear(v2)  # True
   v1.is_collinear(v3)  # False

   # With custom epsilon
   v1.is_collinear(v2, epsilon=1e-6)

Double Precision
----------------

For applications requiring higher precision:

.. code-block:: python

   # Float precision (default)
   v_float = pyrove.vec3(1.0, 2.0, 3.0)

   # Double precision
   v_double = pyrove.dvec3(1.0, 2.0, 3.0)

   # All operations work the same way
   length = v_double.length()
   v_double.normalize()

Best Practices
--------------

1. **Use appropriate precision**: Float is usually sufficient and faster
2. **Prefer squared length**: Use ``length_sq()`` for comparisons to avoid sqrt
3. **Normalize when needed**: Many algorithms require unit vectors
4. **Watch for zero vectors**: Normalizing zero vectors yields identity
5. **Use NumPy for bulk operations**: Convert to NumPy for large datasets

Example: Distance Calculation
------------------------------

.. code-block:: python

   import pyrove

   def distance(p1, p2):
       \"\"\"Calculate distance between two points.\"\"\"
       diff = p2 - p1
       return diff.length()

   def distance_fast(p1, p2):
       \"\"\"Faster comparison using squared distance.\"\"\"
       diff = p2 - p1
       return diff.length_sq()  # For comparisons only

   p1 = pyrove.vec3(0.0, 0.0, 0.0)
   p2 = pyrove.vec3(3.0, 4.0, 0.0)

   dist = distance(p1, p2)  # 5.0
   dist_sq = distance_fast(p1, p2)  # 25.0

   # Check if closer than threshold
   threshold = 10.0
   if distance_fast(p1, p2) < threshold * threshold:
       print("Close enough!")
