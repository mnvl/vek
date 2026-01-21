Quaternion Classes
==================

Quaternions for representing and interpolating 3D rotations.

quat - Quaternion
-----------------

Represents a rotation in 3D space as a quaternion.

Constructor
~~~~~~~~~~~

.. code-block:: python

   quat()                 # Uninitialized quaternion
   quat(x, y, z, w)       # Component constructor

Attributes
~~~~~~~~~~

.. py:attribute:: quat.x
   :type: float

   X component (vector part)

.. py:attribute:: quat.y
   :type: float

   Y component (vector part)

.. py:attribute:: quat.z
   :type: float

   Z component (vector part)

.. py:attribute:: quat.w
   :type: float

   W component (scalar part)

Methods
~~~~~~~

.. py:method:: quat.identity()

   Set to identity quaternion (0, 0, 0, 1) representing no rotation

.. py:method:: quat.norm() -> float

   Compute quaternion magnitude

   :return: Norm (length) of quaternion
   :rtype: float

.. py:method:: quat.normalize()

   Normalize quaternion to unit length in place

.. py:method:: quat.normalized() -> quat

   Return normalized copy of quaternion

   :return: Unit quaternion
   :rtype: quat

.. py:method:: quat.scale(s)

   Scale all components by scalar

   :param float s: Scale factor

.. py:method:: quat.dot(other) -> float

   Compute dot product with another quaternion

   :param quat other: Other quaternion
   :return: Dot product
   :rtype: float

Operators
~~~~~~~~~

.. code-block:: python

   -q           # Negation
   q1 + q2      # Addition
   q1 - q2      # Subtraction
   q * scalar   # Scalar multiplication
   scalar * q   # Scalar multiplication

Static Methods
~~~~~~~~~~~~~~

.. py:staticmethod:: quat.from_numpy(array) -> quat

   Create quat from NumPy array of shape (4,)

   :param array: NumPy array [x, y, z, w]
   :return: New quaternion
   :rtype: quat

.. py:method:: quat.to_numpy() -> ndarray

   Convert quaternion to NumPy array

   :return: NumPy array of shape (4,)
   :rtype: numpy.ndarray

quat_slerper - SLERP Interpolator
----------------------------------

Helper class for spherical linear interpolation between quaternions.

Constructor
~~~~~~~~~~~

.. code-block:: python

   quat_slerper()  # Create empty slerper

Methods
~~~~~~~

.. py:method:: quat_slerper.setup(p, q)

   Setup interpolation between two quaternions

   Normalizes both quaternions and precomputes interpolation parameters.
   Automatically chooses the shorter rotation path.

   :param quat p: Start quaternion
   :param quat q: End quaternion

.. py:method:: quat_slerper.interpolate(t) -> quat

   Interpolate between quaternions

   :param float t: Interpolation parameter [0, 1] where 0=start, 1=end
   :return: Interpolated quaternion
   :rtype: quat

Example
~~~~~~~

.. code-block:: python

   import pyrove

   q1 = pyrove.quat(0.0, 0.0, 0.0, 1.0)  # Identity
   q2 = pyrove.quat(0.0, 0.707, 0.0, 0.707)  # 90° around Y

   # Setup interpolator
   slerper = pyrove.quat_slerper()
   slerper.setup(q1, q2)

   # Interpolate at various points
   q_25 = slerper.interpolate(0.25)   # 25% of the way
   q_50 = slerper.interpolate(0.5)    # Midpoint
   q_75 = slerper.interpolate(0.75)   # 75% of the way

Double Precision Variants
--------------------------

* ``dquat`` - Double precision quaternion
* ``dquat_slerper`` - Double precision SLERP interpolator

Same API as float versions.

Example
~~~~~~~

.. code-block:: python

   import pyrove

   # Create quaternions
   q1 = pyrove.quat(0.0, 0.0, 0.0, 1.0)
   q2 = pyrove.quat(0.0, 0.707, 0.0, 0.707)

   # Use as rotation for matrix
   m = pyrove.mat4()
   m.rotation(q1)

   # Interpolate rotations
   q_mid = pyrove.slerp(q1, q2, 0.5)
