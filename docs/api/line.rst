Line Classes
============

The Line classes represent line segments defined by two endpoint coordinates.
They provide methods for length calculation, distance queries, and containment
tests. Both 2D and 3D variants are available in float and double precision.

line2 - 2D Line Segment
-----------------------

Float precision 2D line segment.

Constructor
~~~~~~~~~~~

.. code-block:: python

   line2()           # Default constructor (uninitialized)
   line2(a, b)       # From two endpoint vectors

Attributes
~~~~~~~~~~

.. py:attribute:: line2.A
   :type: vec2

   First endpoint of the line segment

.. py:attribute:: line2.B
   :type: vec2

   Second endpoint of the line segment

Methods
~~~~~~~

.. py:method:: line2.construct(a, b)

   Set line segment endpoints.

   :param vec2 a: First endpoint
   :param vec2 b: Second endpoint

.. py:method:: line2.direction() -> vec2

   Get the direction vector from A to B (B - A).

   :return: Direction vector
   :rtype: vec2

.. py:method:: line2.get_ray() -> ray2

   Convert line segment to ray representation starting at A.

   :return: Ray with origin at A and direction towards B
   :rtype: ray2

.. py:method:: line2.get_aabb() -> aabb2

   Get the axis-aligned bounding box of the line segment.

   :return: Bounding box
   :rtype: aabb2

.. py:method:: line2.length_sq() -> float

   Get squared length of the line segment (faster, avoids square root).

   :return: Squared length
   :rtype: float

.. py:method:: line2.contains(point) -> bool

   Test if a point lies on the line segment.

   :param vec2 point: Point to test
   :return: True if point lies on the segment
   :rtype: bool

.. py:method:: line2.distance_sq(point) -> float

   Calculate squared distance from line segment to point.

   :param vec2 point: Point to measure distance to
   :return: Squared distance
   :rtype: float

.. py:method:: line2.distance(point) -> float

   Calculate distance from line segment to point.

   :param vec2 point: Point to measure distance to
   :return: Distance
   :rtype: float

line3 - 3D Line Segment
-----------------------

Float precision 3D line segment.

Constructor
~~~~~~~~~~~

.. code-block:: python

   line3()           # Default constructor (uninitialized)
   line3(a, b)       # From two endpoint vectors

Attributes
~~~~~~~~~~

.. py:attribute:: line3.A
   :type: vec3

   First endpoint of the line segment

.. py:attribute:: line3.B
   :type: vec3

   Second endpoint of the line segment

Methods
~~~~~~~

.. py:method:: line3.construct(a, b)

   Set line segment endpoints.

   :param vec3 a: First endpoint
   :param vec3 b: Second endpoint

.. py:method:: line3.direction() -> vec3

   Get the direction vector from A to B (B - A).

   :return: Direction vector
   :rtype: vec3

.. py:method:: line3.get_ray() -> ray3

   Convert line segment to ray representation starting at A.

   :return: Ray with origin at A and direction towards B
   :rtype: ray3

.. py:method:: line3.get_aabb() -> aabb3

   Get the axis-aligned bounding box of the line segment.

   :return: Bounding box
   :rtype: aabb3

.. py:method:: line3.length_sq() -> float

   Get squared length of the line segment (faster, avoids square root).

   :return: Squared length
   :rtype: float

.. py:method:: line3.contains(point) -> bool

   Test if a point lies on the line segment.

   :param vec3 point: Point to test
   :return: True if point lies on the segment
   :rtype: bool

.. py:method:: line3.distance_sq(point) -> float

   Calculate squared distance from line segment to point.

   :param vec3 point: Point to measure distance to
   :return: Squared distance
   :rtype: float

.. py:method:: line3.distance(point) -> float

   Calculate distance from line segment to point.

   :param vec3 point: Point to measure distance to
   :return: Distance
   :rtype: float

Double Precision Variants
-------------------------

All line types have double-precision variants:

* ``line2d`` - Double precision 2D line segment
* ``line3d`` - Double precision 3D line segment

They have the same API as their float counterparts, but use ``double`` instead of ``float``.

Example
~~~~~~~

.. code-block:: python

   import pyrove

   # Create a 3D line segment
   line = pyrove.line3(pyrove.vec3(0, 0, 0), pyrove.vec3(10, 0, 0))

   # Get direction and length
   direction = line.direction()  # vec3(10, 0, 0)
   length_sq = line.length_sq()  # 100.0

   # Find distance from line to a point
   point = pyrove.vec3(5, 3, 0)
   dist = line.distance(point)
   print(f"Distance from line to point: {dist}")

   # Test if point lies on segment
   midpoint = pyrove.vec3(5, 0, 0)
   if line.contains(midpoint):
       print("Midpoint is on the line segment")

   # Convert to ray
   ray = line.get_ray()

   # Get bounding box
   bbox = line.get_aabb()
   print(f"AABB: min={bbox.lo}, max={bbox.hi}")

   # 2D example
   line2d = pyrove.line2(pyrove.vec2(0, 0), pyrove.vec2(5, 5))
   center = (line2d.A + line2d.B) * 0.5  # Calculate center point
