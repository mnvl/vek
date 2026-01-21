AABB Classes
============

The AABB (Axis-Aligned Bounding Box) classes provide efficient bounding volumes
for spatial queries and collision detection. Both 2D and 3D variants are available
in float and double precision.

aabb2 - 2D AABB
---------------

Float precision 2D axis-aligned bounding box (rectangle).

Constructor
~~~~~~~~~~~

.. code-block:: python

   aabb2()                    # Default (uninitialized)
   aabb2(centre)              # Degenerate AABB from single point
   aabb2(lo, hi)              # From two corner points
   aabb2(centre, radius)      # From center and half-extent

Attributes
~~~~~~~~~~

.. py:attribute:: aabb2.lo
   :type: vec2

   Minimum corner (lower bounds in all dimensions)

.. py:attribute:: aabb2.hi
   :type: vec2

   Maximum corner (upper bounds in all dimensions)

Methods
~~~~~~~

.. py:method:: aabb2.null()

   Reset AABB to "null" state for accumulation. Sets lo to maximum and hi to
   minimum values, preparing for extend() calls.

.. py:method:: aabb2.normalize()

   Ensure lo <= hi in all dimensions by swapping coordinates where necessary.

.. py:method:: aabb2.diagonal() -> vec2

   Get the diagonal vector (hi - lo).

   :return: Vector representing AABB dimensions
   :rtype: vec2

.. py:method:: aabb2.centre() -> vec2

   Get the center point of the AABB.

   :return: Center point
   :rtype: vec2

.. py:method:: aabb2.extend(point)

   Extend AABB to include a point.

   :param vec2 point: Point to include

.. py:method:: aabb2.extend(other)

   Extend AABB to include another AABB (union).

   :param aabb2 other: AABB to include

.. py:method:: aabb2.contains(point) -> bool

   Test if a point is inside the AABB.

   :param vec2 point: Point to test
   :return: True if point is inside or on the boundary
   :rtype: bool

.. py:method:: aabb2.contains(other) -> bool

   Test if another AABB is fully contained.

   :param aabb2 other: AABB to test
   :return: True if other is completely inside this AABB
   :rtype: bool

.. py:method:: aabb2.test_intersection(other) -> bool

   Test intersection with another AABB.

   :param aabb2 other: AABB to test against
   :return: True if the AABBs overlap
   :rtype: bool

aabb3 - 3D AABB
---------------

Float precision 3D axis-aligned bounding box.

Constructor
~~~~~~~~~~~

.. code-block:: python

   aabb3()                    # Default (uninitialized)
   aabb3(centre)              # Degenerate AABB from single point
   aabb3(lo, hi)              # From two corner points
   aabb3(centre, radius)      # From center and half-extent

Attributes
~~~~~~~~~~

.. py:attribute:: aabb3.lo
   :type: vec3

   Minimum corner (lower bounds in all dimensions)

.. py:attribute:: aabb3.hi
   :type: vec3

   Maximum corner (upper bounds in all dimensions)

Methods
~~~~~~~

.. py:method:: aabb3.null()

   Reset AABB to "null" state for accumulation.

.. py:method:: aabb3.normalize()

   Ensure lo <= hi in all dimensions.

.. py:method:: aabb3.diagonal() -> vec3

   Get the diagonal vector (hi - lo).

   :return: Vector representing AABB dimensions
   :rtype: vec3

.. py:method:: aabb3.centre() -> vec3

   Get the center point of the AABB.

   :return: Center point
   :rtype: vec3

.. py:method:: aabb3.extend(point)

   Extend AABB to include a point.

   :param vec3 point: Point to include

.. py:method:: aabb3.extend(other)

   Extend AABB to include another AABB.

   :param aabb3 other: AABB to include

.. py:method:: aabb3.contains(point) -> bool

   Test if a point is inside the AABB.

   :param vec3 point: Point to test
   :return: True if point is inside or on the boundary
   :rtype: bool

.. py:method:: aabb3.contains(other) -> bool

   Test if another AABB is fully contained.

   :param aabb3 other: AABB to test
   :return: True if other is completely inside this AABB
   :rtype: bool

.. py:method:: aabb3.test_intersection(other) -> bool

   Test intersection with another AABB.

   :param aabb3 other: AABB to test against
   :return: True if the AABBs overlap
   :rtype: bool

Double Precision Variants
-------------------------

All AABB types have double-precision variants:

* ``aabb2d`` - Double precision 2D AABB
* ``aabb3d`` - Double precision 3D AABB

They have the same API as their float counterparts, but use ``double`` instead of ``float``.

Example
~~~~~~~

.. code-block:: python

   import pyrove

   # Create AABBs
   box1 = pyrove.aabb3(pyrove.vec3(0, 0, 0), pyrove.vec3(1, 1, 1))
   box2 = pyrove.aabb3(pyrove.vec3(0.5, 0.5, 0.5), pyrove.vec3(1.5, 1.5, 1.5))

   # Test intersection
   if box1.test_intersection(box2):
       print("Boxes overlap!")

   # Build AABB from points
   bounds = pyrove.aabb3()
   bounds.null()  # Initialize for accumulation
   bounds.extend(pyrove.vec3(1, 2, 3))
   bounds.extend(pyrove.vec3(-1, 0, 5))
   bounds.extend(pyrove.vec3(0, 4, 1))
   print(f"Center: {bounds.centre()}")
   print(f"Size: {bounds.diagonal()}")

   # Point containment test
   point = pyrove.vec3(0.5, 0.5, 0.5)
   if box1.contains(point):
       print("Point is inside box1")
