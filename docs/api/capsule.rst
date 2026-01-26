Capsule Classes
===============

The Capsule classes represent a capsule shape defined by a line segment (axis)
and a radius. A capsule is effectively a cylinder with hemispherical caps,
useful for collision detection and swept sphere tests. Both 2D and 3D variants
are available in float and double precision.

capsule2 - 2D Capsule
---------------------

Float precision 2D capsule (a line segment with circular caps).

Constructor
~~~~~~~~~~~

.. code-block:: python

   capsule2()                      # Default constructor (uninitialized)
   capsule2(a, b, radius)          # From two endpoints and radius

Attributes
~~~~~~~~~~

.. py:attribute:: capsule2.axe
   :type: line2

   The central axis of the capsule as a line segment

.. py:attribute:: capsule2.radius
   :type: float

   The radius of the capsule (distance from axis to surface)

Methods
~~~~~~~

.. py:method:: capsule2.contains(point) -> bool

   Test if a point is inside the capsule.

   :param vec2 point: Point to test
   :return: True if point is inside or on the surface
   :rtype: bool

.. py:method:: capsule2.distance(point) -> float

   Calculate the distance from the capsule surface to a point.
   Returns 0 if the point is inside the capsule.

   :param vec2 point: Point to measure distance to
   :return: Distance (0 if inside)
   :rtype: float

capsule3 - 3D Capsule
---------------------

Float precision 3D capsule (a line segment with hemispherical caps).

Constructor
~~~~~~~~~~~

.. code-block:: python

   capsule3()                      # Default constructor (uninitialized)
   capsule3(a, b, radius)          # From two endpoints and radius

Attributes
~~~~~~~~~~

.. py:attribute:: capsule3.axe
   :type: line3

   The central axis of the capsule as a line segment

.. py:attribute:: capsule3.radius
   :type: float

   The radius of the capsule (distance from axis to surface)

Methods
~~~~~~~

.. py:method:: capsule3.contains(point) -> bool

   Test if a point is inside the capsule.

   :param vec3 point: Point to test
   :return: True if point is inside or on the surface
   :rtype: bool

.. py:method:: capsule3.distance(point) -> float

   Calculate the distance from the capsule surface to a point.
   Returns 0 if the point is inside the capsule.

   :param vec3 point: Point to measure distance to
   :return: Distance (0 if inside)
   :rtype: float

Double Precision Variants
-------------------------

All capsule types have double-precision variants:

* ``capsule2d`` - Double precision 2D capsule
* ``capsule3d`` - Double precision 3D capsule

They have the same API as their float counterparts, but use ``double`` instead of ``float``.

Example
~~~~~~~

.. code-block:: python

   import pyrove

   # Create a 3D capsule from two endpoints and radius
   point_a = pyrove.vec3(0, 0, 0)
   point_b = pyrove.vec3(0, 10, 0)
   cap = pyrove.capsule3(point_a, point_b, 2.0)

   # Access capsule properties
   print(f"Axis: {cap.axe.A} to {cap.axe.B}")
   print(f"Radius: {cap.radius}")

   # Test if point is inside
   test_point = pyrove.vec3(0, 5, 1)
   if cap.contains(test_point):
       print("Point is inside the capsule")

   # Calculate distance from surface
   outside_point = pyrove.vec3(5, 5, 0)
   dist = cap.distance(outside_point)
   print(f"Distance from capsule surface: {dist}")

   # Capsule can be used for swept sphere collision detection
   # The capsule represents the volume swept by a sphere moving along a line
   start = pyrove.vec3(0, 0, 0)
   end = pyrove.vec3(10, 0, 0)
   sphere_radius = 1.5
   swept_volume = pyrove.capsule3(start, end, sphere_radius)

   # Check if any point along the path is within the capsule
   collision_point = pyrove.vec3(5, 1, 0)
   if swept_volume.contains(collision_point):
       print("Collision detected!")

   # 2D example
   cap2d = pyrove.capsule2(pyrove.vec2(0, 0), pyrove.vec2(5, 0), 1.0)
   point2d = pyrove.vec2(2.5, 0.5)
   if cap2d.contains(point2d):
       print("2D point is inside the capsule")

   # Double precision example
   cap_d = pyrove.capsule3d(
       pyrove.vec3d(0, 0, 0),
       pyrove.vec3d(0, 0, 10),
       1.5
   )
   dist_d = cap_d.distance(pyrove.vec3d(0, 2, 5))
