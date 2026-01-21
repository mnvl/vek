Matrix Classes
==============

Matrix classes for 2D and 3D transformations. Both float and double precision types are available.

mat3 - 3x3 Matrix
-----------------

3x3 matrix for 2D transformations and 3D rotations.

Constructor
~~~~~~~~~~~

.. code-block:: python

   mat3()  # Uninitialized matrix

Methods
~~~~~~~

Initialization
^^^^^^^^^^^^^^

.. py:method:: mat3.identity()

   Set to identity matrix

.. py:method:: mat3.zero()

   Set all elements to zero

Transformations
^^^^^^^^^^^^^^^

.. py:method:: mat3.translation(x, y)
              mat3.translation(vec2)

   Set to translation matrix

   :param float x: X translation
   :param float y: Y translation

.. py:method:: mat3.translate(x, y)
              mat3.translate(vec2)

   Apply translation (multiply by translation matrix)

.. py:method:: mat3.scaling(x, y)
              mat3.scaling(vec2)

   Set to scaling matrix

.. py:method:: mat3.scale(x, y)
              mat3.scale(vec2)

   Apply scaling

.. py:method:: mat3.rotation(axis, angle)
              mat3.rotation(quaternion)

   Set to rotation matrix

   :param vec3 axis: Rotation axis
   :param float angle: Rotation angle in radians
   :param quat quaternion: Rotation as quaternion

.. py:method:: mat3.rotate(axis, angle)
              mat3.rotate(quaternion)

   Apply rotation

Matrix Operations
^^^^^^^^^^^^^^^^^

.. py:method:: mat3.determinant() -> float

   Compute determinant

   :return: Determinant value
   :rtype: float

.. py:method:: mat3.trace() -> float

   Compute trace (sum of diagonal elements)

   :return: Trace value
   :rtype: float

.. py:method:: mat3.inverse() -> mat3

   Return inverse matrix

   :return: Inverse matrix
   :rtype: mat3
   :raises RuntimeError: If matrix is singular

.. py:method:: mat3.invert() -> bool

   Invert matrix in place

   :return: False if matrix is singular
   :rtype: bool

.. py:method:: mat3.transpose() -> mat3

   Return transposed matrix

   :return: Transposed matrix
   :rtype: mat3

Element Access
^^^^^^^^^^^^^^

.. py:method:: mat3.get(i, j) -> float

   Get matrix element at [i][j]

   :param int i: Column index (0-2)
   :param int j: Row index (0-2)
   :return: Element value
   :rtype: float

.. py:method:: mat3.set(i, j, value)

   Set matrix element at [i][j]

   :param int i: Column index (0-2)
   :param int j: Row index (0-2)
   :param float value: New value

Operators
~~~~~~~~~

.. code-block:: python

   m1 * m2           # Matrix multiplication
   m * scalar        # Scalar multiplication
   scalar * m        # Scalar multiplication
   m / scalar        # Scalar division
   m *= scalar       # In-place scalar multiplication
   m *= m2           # In-place matrix multiplication
   m /= scalar       # In-place scalar division
   m * vec2          # Transform vec2 (returns vec3)
   m * vec3          # Transform vec3

Static Methods
~~~~~~~~~~~~~~

.. py:staticmethod:: mat3.from_numpy(array) -> mat3

   Create mat3 from NumPy array of shape (3, 3)

.. py:method:: mat3.to_numpy() -> ndarray

   Convert mat3 to NumPy array

mat4 - 4x4 Matrix
-----------------

4x4 matrix for 3D transformations, projections, and view matrices.

Constructor
~~~~~~~~~~~

.. code-block:: python

   mat4()  # Uninitialized matrix

Methods
~~~~~~~

Initialization
^^^^^^^^^^^^^^

.. py:method:: mat4.identity()

   Set to identity matrix

.. py:method:: mat4.zero()

   Set all elements to zero

Transformations
^^^^^^^^^^^^^^^

.. py:method:: mat4.translation(x, y, z)
              mat4.translation(vec3)

   Set to translation matrix

.. py:method:: mat4.translate(x, y, z)
              mat4.translate(vec3)

   Apply translation

.. py:method:: mat4.scaling(x, y, z)
              mat4.scaling(vec3)

   Set to scaling matrix

.. py:method:: mat4.scale(x, y, z)
              mat4.scale(vec3)

   Apply scaling

.. py:method:: mat4.rotation(axis, angle)
              mat4.rotation(quaternion)

   Set to rotation matrix

.. py:method:: mat4.rotate(axis, angle)
              mat4.rotate(quaternion)

   Apply rotation

Camera and Projection
^^^^^^^^^^^^^^^^^^^^^

.. py:method:: mat4.lookat(eye, at, up)

   Set to look-at view matrix

   :param vec3 eye: Camera position
   :param vec3 at: Target position
   :param vec3 up: Up direction

.. py:method:: mat4.ortho(width, height, z_near, z_far)

   Set to orthographic projection matrix

   :param float width: View width
   :param float height: View height
   :param float z_near: Near clipping plane
   :param float z_far: Far clipping plane

.. py:method:: mat4.perspective(fovy, aspect, z_near, z_far)

   Set to perspective projection matrix

   :param float fovy: Field of view (Y axis) in radians
   :param float aspect: Aspect ratio (width/height)
   :param float z_near: Near clipping plane
   :param float z_far: Far clipping plane

Matrix Operations
^^^^^^^^^^^^^^^^^

.. py:method:: mat4.determinant() -> float

   Compute determinant

.. py:method:: mat4.inverse() -> mat4

   Return inverse matrix

   :raises RuntimeError: If matrix is singular

.. py:method:: mat4.invert() -> bool

   Invert matrix in place

.. py:method:: mat4.transpose() -> mat4

   Return transposed matrix

.. py:method:: mat4.transpose_inplace()

   Transpose matrix in place

Column and Row Access
^^^^^^^^^^^^^^^^^^^^^

.. py:method:: mat4.column(index) -> vec4

   Get column as vec4

   :param int index: Column index (0-3)
   :return: Column vector
   :rtype: vec4

.. py:method:: mat4.projected_column(index) -> vec3

   Get column as vec3 (drops w component)

   :param int index: Column index (0-3)
   :return: Column vector
   :rtype: vec3

.. py:method:: mat4.projected_row(index) -> vec3

   Get row as vec3 (drops w component)

   :param int index: Row index (0-3)
   :return: Row vector
   :rtype: vec3

Element Access
^^^^^^^^^^^^^^

.. py:method:: mat4.get(i, j) -> float

   Get matrix element at [i][j]

.. py:method:: mat4.set(i, j, value)

   Set matrix element at [i][j]

Operators
~~~~~~~~~

Same as mat3, plus:

.. code-block:: python

   m * vec3          # Transform vec3 (returns vec3)
   m * vec4          # Transform vec4

Static Methods
~~~~~~~~~~~~~~

.. py:staticmethod:: mat4.from_numpy(array) -> mat4

   Create mat4 from NumPy array of shape (4, 4)

.. py:method:: mat4.to_numpy() -> ndarray

   Convert mat4 to NumPy array

Double Precision Variants
--------------------------

* ``mat3d`` - Double precision 3x3 matrix
* ``mat4d`` - Double precision 4x4 matrix

Same API as float versions, using ``double`` precision.

Example
~~~~~~~

.. code-block:: python

   import pyrove

   # Create transformation matrix
   m = pyrove.mat4()
   m.identity()

   # Build transformation
   m.translate(1.0, 2.0, 3.0)
   m.rotate(pyrove.vec3(0.0, 1.0, 0.0), 1.57)  # 90° around Y
   m.scale(2.0, 2.0, 2.0)

   # Transform a point
   p = pyrove.vec3(1.0, 0.0, 0.0)
   p_transformed = m * p

   # Create perspective projection
   proj = pyrove.mat4()
   proj.perspective(1.57, 16.0/9.0, 0.1, 100.0)
