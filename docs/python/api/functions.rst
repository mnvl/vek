Free Functions
==============

Global functions for vector and quaternion operations.

Vector Functions
----------------

dot
~~~

.. py:function:: dot(a, b) -> float

   Compute dot product of two vectors

   Overloaded for vec2, vec3, vec4 (float precision only).

   :param a: First vector
   :type a: vec2, vec3, or vec4
   :param b: Second vector (same type as a)
   :type b: vec2, vec3, or vec4
   :return: Dot product (a · b)
   :rtype: float

   Example:

   .. code-block:: python

      import pyrove

      v1 = pyrove.vec3(1.0, 0.0, 0.0)
      v2 = pyrove.vec3(0.0, 1.0, 0.0)
      dot = pyrove.dot(v1, v2)  # 0.0

cross
~~~~~

.. py:function:: cross(a, b) -> vec3

   Compute cross product of two 3D vectors

   Returns a vector perpendicular to both inputs, following the right-hand rule.

   :param vec3 a: First vector
   :param vec3 b: Second vector
   :return: Cross product (a × b)
   :rtype: vec3

   Example:

   .. code-block:: python

      import pyrove

      v1 = pyrove.vec3(1.0, 0.0, 0.0)
      v2 = pyrove.vec3(0.0, 1.0, 0.0)
      cross = pyrove.cross(v1, v2)  # vec3(0, 0, 1)

normalize
~~~~~~~~~

.. py:function:: normalize(v) -> vector

   Return normalized copy of vector

   Overloaded for vec2, vec3, vec4 (float precision only).

   :param v: Input vector
   :type v: vec2, vec3, or vec4
   :return: Unit vector in same direction
   :rtype: Same as input type

   Example:

   .. code-block:: python

      import pyrove

      v = pyrove.vec3(3.0, 4.0, 0.0)
      v_norm = pyrove.normalize(v)  # vec3(0.6, 0.8, 0.0)
      print(v_norm.length())  # 1.0

Quaternion Functions
--------------------

dot
~~~

.. py:function:: dot(p, q) -> float

   Compute dot product of two quaternions

   :param quat p: First quaternion
   :param quat q: Second quaternion
   :return: Dot product (p · q)
   :rtype: float

   Example:

   .. code-block:: python

      import pyrove

      q1 = pyrove.quat(0.0, 0.0, 0.0, 1.0)
      q2 = pyrove.quat(0.0, 0.707, 0.0, 0.707)
      dot = pyrove.dot(q1, q2)

slerp
~~~~~

.. py:function:: slerp(p, q, t) -> quat

   Spherical linear interpolation between quaternions

   Provides smooth, constant-velocity rotation interpolation along the
   shortest arc on the 4D unit hypersphere.

   :param quat p: Start quaternion
   :param quat q: End quaternion
   :param float t: Interpolation parameter [0, 1] where 0=start, 1=end
   :return: Interpolated quaternion
   :rtype: quat

   Example:

   .. code-block:: python

      import pyrove

      q1 = pyrove.quat(0.0, 0.0, 0.0, 1.0)  # No rotation
      q2 = pyrove.quat(0.0, 0.707, 0.0, 0.707)  # 90° around Y

      # Interpolate to 50% of the rotation
      q_mid = pyrove.slerp(q1, q2, 0.5)  # 45° around Y

Note on Precision
-----------------

The free functions currently only support float precision types (``vec2``, ``vec3``,
``vec4``, ``quat``). For double precision operations, use the methods on the
double precision types directly:

.. code-block:: python

   import pyrove

   # Float precision - can use free functions
   v1 = pyrove.vec3(1.0, 2.0, 3.0)
   v2 = pyrove.vec3(4.0, 5.0, 6.0)
   dot = pyrove.dot(v1, v2)

   # Double precision - use methods
   dv1 = pyrove.dvec3(1.0, 2.0, 3.0)
   dv2 = pyrove.dvec3(4.0, 5.0, 6.0)
   dot = dv1.dot(dv2)
