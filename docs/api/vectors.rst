Vector Classes
==============

The vector classes provide N-dimensional vector operations with specializations
for 2D, 3D, and 4D vectors. Both float and double precision types are available.

vec2 - 2D Vector
----------------

Float precision 2D vector.

Constructor
~~~~~~~~~~~

.. code-block:: python

   vec2()              # Zero vector
   vec2(x, y)          # Component constructor

Attributes
~~~~~~~~~~

.. py:attribute:: vec2.x
   :type: float

   X component (can also access as ``u``, ``s``, or ``A``)

.. py:attribute:: vec2.y
   :type: float

   Y component (can also access as ``v``, ``t``, or ``B``)

Methods
~~~~~~~

.. py:method:: vec2.set(x, y)

   Set vector components

   :param float x: X component
   :param float y: Y component

.. py:method:: vec2.set_all(a)

   Set all components to the same value

   :param float a: Value for all components

.. py:method:: vec2.length() -> float

   Calculate Euclidean length

   :return: Vector magnitude
   :rtype: float

.. py:method:: vec2.length_sq() -> float

   Calculate squared length (faster, avoids square root)

   :return: Squared vector magnitude
   :rtype: float

.. py:method:: vec2.length_manhattan() -> float

   Calculate Manhattan distance (|x| + |y|)

   :return: Manhattan distance
   :rtype: float

.. py:method:: vec2.normalize()

   Normalize vector to unit length in place

.. py:method:: vec2.negate()

   Negate all components in place

.. py:method:: vec2.perpendicular() -> vec2

   Get perpendicular vector (rotated 90° counterclockwise)

   :return: Perpendicular vector
   :rtype: vec2

.. py:method:: vec2.is_collinear(v, epsilon=EPSILON) -> bool

   Test if this vector is collinear (parallel) with another

   :param vec2 v: Vector to test against
   :param float epsilon: Tolerance for comparison
   :return: True if vectors are parallel within epsilon
   :rtype: bool

.. py:method:: vec2.dot(other) -> float

   Compute dot product with another vector

   :param vec2 other: Other vector
   :return: Dot product
   :rtype: float

Operators
~~~~~~~~~

.. code-block:: python

   -v           # Negation
   v1 + v2      # Addition
   v1 - v2      # Subtraction
   v1 * v2      # Component-wise multiplication
   v1 / v2      # Component-wise division
   v * scalar   # Scalar multiplication
   scalar * v   # Scalar multiplication (commutative)
   v / scalar   # Scalar division
   v1 == v2     # Equality
   v1 != v2     # Inequality

Static Methods
~~~~~~~~~~~~~~

.. py:staticmethod:: vec2.from_numpy(array) -> vec2

   Create vec2 from NumPy array

   :param array: NumPy array of shape (2,)
   :return: New vec2
   :rtype: vec2

.. py:method:: vec2.to_numpy() -> ndarray

   Convert vec2 to NumPy array

   :return: NumPy array of shape (2,)
   :rtype: numpy.ndarray

vec3 - 3D Vector
----------------

Float precision 3D vector.

Constructor
~~~~~~~~~~~

.. code-block:: python

   vec3()              # Zero vector
   vec3(x, y, z)       # Component constructor

Attributes
~~~~~~~~~~

.. py:attribute:: vec3.x
   :type: float

   X component (can also access as ``r`` or ``A``)

.. py:attribute:: vec3.y
   :type: float

   Y component (can also access as ``g`` or ``B``)

.. py:attribute:: vec3.z
   :type: float

   Z component (can also access as ``b`` or ``C``)

Methods
~~~~~~~

.. py:method:: vec3.set(x, y, z)

   Set vector components

.. py:method:: vec3.set_all(a)

   Set all components to the same value

.. py:method:: vec3.length() -> float

   Calculate Euclidean length

.. py:method:: vec3.length_sq() -> float

   Calculate squared length

.. py:method:: vec3.normalize()

   Normalize vector to unit length in place

.. py:method:: vec3.negate()

   Negate all components in place

.. py:method:: vec3.is_collinear(v, epsilon=EPSILON) -> bool

   Test if this vector is collinear (parallel) with another

.. py:method:: vec3.dot(other) -> float

   Compute dot product with another vector

.. py:method:: vec3.cross(other) -> vec3

   Compute cross product with another vector

   :param vec3 other: Other vector
   :return: Cross product (perpendicular to both vectors)
   :rtype: vec3

Operators
~~~~~~~~~

Same as vec2, plus support for cross product in free functions.

Static Methods
~~~~~~~~~~~~~~

.. py:staticmethod:: vec3.from_numpy(array) -> vec3

   Create vec3 from NumPy array of shape (3,)

.. py:method:: vec3.to_numpy() -> ndarray

   Convert vec3 to NumPy array

vec4 - 4D Vector
----------------

Float precision 4D vector (commonly used for homogeneous coordinates).

Constructor
~~~~~~~~~~~

.. code-block:: python

   vec4()                 # Zero vector
   vec4(x, y, z, w)       # Component constructor

Attributes
~~~~~~~~~~

.. py:attribute:: vec4.x
   :type: float

   X component (can also access as ``r`` or ``A``)

.. py:attribute:: vec4.y
   :type: float

   Y component (can also access as ``g`` or ``B``)

.. py:attribute:: vec4.z
   :type: float

   Z component (can also access as ``b`` or ``C``)

.. py:attribute:: vec4.w
   :type: float

   W component (can also access as ``a`` or ``D``)

Methods
~~~~~~~

.. py:method:: vec4.set(x, y, z, w)

   Set vector components

.. py:method:: vec4.set_all(a)

   Set all components to the same value

.. py:method:: vec4.length() -> float

   Calculate Euclidean length

.. py:method:: vec4.length_sq() -> float

   Calculate squared length

.. py:method:: vec4.normalize()

   Normalize vector to unit length in place

.. py:method:: vec4.negate()

   Negate all components in place

.. py:method:: vec4.dot(other) -> float

   Compute dot product with another vector

Operators
~~~~~~~~~

Same as vec2 (no cross product for 4D).

Static Methods
~~~~~~~~~~~~~~

.. py:staticmethod:: vec4.from_numpy(array) -> vec4

   Create vec4 from NumPy array of shape (4,)

.. py:method:: vec4.to_numpy() -> ndarray

   Convert vec4 to NumPy array

Double Precision Variants
--------------------------

All vector types have double-precision variants:

* ``vec2d`` - Double precision 2D vector
* ``vec3d`` - Double precision 3D vector
* ``vec4d`` - Double precision 4D vector

They have the same API as their float counterparts, but use ``double`` instead of ``float``.

Example
~~~~~~~

.. code-block:: python

   import pyrove

   # Float precision (default)
   v_float = pyrove.vec3(1.0, 2.0, 3.0)

   # Double precision
   v_double = pyrove.vec3d(1.0, 2.0, 3.0)
