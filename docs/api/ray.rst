Ray Classes
===========

The Ray classes represent half-infinite lines defined by an origin point and a
direction vector. They are commonly used for ray casting, ray tracing, and
intersection tests. Both 2D and 3D variants are available in float and double precision.

ray2 - 2D Ray
-------------

Float precision 2D ray.

Constructor
~~~~~~~~~~~

.. code-block:: python

   ray2()                    # Default constructor (uninitialized)
   ray2(origin, direction)   # From origin point and direction vector

Attributes
~~~~~~~~~~

.. py:attribute:: ray2.r0
   :type: vec2

   Ray origin point

.. py:attribute:: ray2.a
   :type: vec2

   Ray direction vector (not necessarily normalized)

Methods
~~~~~~~

.. py:method:: ray2.set(origin, direction)

   Set ray parameters.

   :param vec2 origin: Origin point
   :param vec2 direction: Direction vector

.. py:method:: ray2.apply(t) -> vec2

   Get a point on the ray at parameter t using the formula: r0 + a * t

   :param float t: Parameter value (t >= 0 for points on the ray)
   :return: Point on the ray
   :rtype: vec2

.. py:method:: ray2.contains(point, epsilon=EPSILON) -> bool

   Test if a point lies on the ray within tolerance.

   :param vec2 point: Point to test
   :param float epsilon: Tolerance for comparison (default: EPSILON)
   :return: True if point lies on the ray
   :rtype: bool

.. py:method:: ray2.distance_sq(point) -> float

   Calculate squared distance from ray to point (faster, avoids square root).

   :param vec2 point: Point to measure distance to
   :return: Squared distance
   :rtype: float

.. py:method:: ray2.distance(point) -> float

   Calculate distance from ray to point.

   :param vec2 point: Point to measure distance to
   :return: Distance
   :rtype: float

ray3 - 3D Ray
-------------

Float precision 3D ray.

Constructor
~~~~~~~~~~~

.. code-block:: python

   ray3()                    # Default constructor (uninitialized)
   ray3(origin, direction)   # From origin point and direction vector

Attributes
~~~~~~~~~~

.. py:attribute:: ray3.r0
   :type: vec3

   Ray origin point

.. py:attribute:: ray3.a
   :type: vec3

   Ray direction vector (not necessarily normalized)

Methods
~~~~~~~

.. py:method:: ray3.set(origin, direction)

   Set ray parameters.

   :param vec3 origin: Origin point
   :param vec3 direction: Direction vector

.. py:method:: ray3.apply(t) -> vec3

   Get a point on the ray at parameter t using the formula: r0 + a * t

   :param float t: Parameter value (t >= 0 for points on the ray)
   :return: Point on the ray
   :rtype: vec3

.. py:method:: ray3.contains(point, epsilon=EPSILON) -> bool

   Test if a point lies on the ray within tolerance.

   :param vec3 point: Point to test
   :param float epsilon: Tolerance for comparison (default: EPSILON)
   :return: True if point lies on the ray
   :rtype: bool

.. py:method:: ray3.distance_sq(point) -> float

   Calculate squared distance from ray to point (faster, avoids square root).

   :param vec3 point: Point to measure distance to
   :return: Squared distance
   :rtype: float

.. py:method:: ray3.distance(point) -> float

   Calculate distance from ray to point.

   :param vec3 point: Point to measure distance to
   :return: Distance
   :rtype: float

Double Precision Variants
-------------------------

All ray types have double-precision variants:

* ``ray2d`` - Double precision 2D ray
* ``ray3d`` - Double precision 3D ray

They have the same API as their float counterparts, but use ``double`` instead of ``float``.

Example
~~~~~~~

.. code-block:: python

   import pyrove

   # Create a 3D ray from origin pointing in direction
   origin = pyrove.vec3(0, 0, 0)
   direction = pyrove.vec3(1, 0, 0)
   ray = pyrove.ray3(origin, direction)

   # Get point at parameter t=5
   point = ray.apply(5.0)  # Returns vec3(5, 0, 0)

   # Calculate distance from ray to a point
   test_point = pyrove.vec3(10, 5, 0)
   dist = ray.distance(test_point)
   print(f"Distance: {dist}")

   # Test if point lies on ray
   on_ray_point = pyrove.vec3(3, 0, 0)
   if ray.contains(on_ray_point):
       print("Point is on the ray")

   # Double precision example
   ray_d = pyrove.ray3d(pyrove.vec3d(0, 0, 0), pyrove.vec3d(0, 1, 0))
   point_d = ray_d.apply(2.5)  # vec3d(0, 2.5, 0)
