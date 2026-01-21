OBB Classes
===========

The OBB (Oriented Bounding Box) classes provide rotatable bounding volumes that
can better fit rotated objects compared to axis-aligned bounding boxes. Both 2D
and 3D variants are available in float and double precision.

obb2 - 2D OBB
-------------

Float precision 2D oriented bounding box.

A 2D OBB is defined by an origin point and two edge vectors (tangent and normal).
The four vertices are:

* ``origin``
* ``origin + tangent``
* ``origin + tangent + normal``
* ``origin + normal``

Constructor
~~~~~~~~~~~

.. code-block:: python

   obb2()           # Default (uninitialized)
   obb2(aabb)       # From axis-aligned bounding box

Attributes
~~~~~~~~~~

.. py:attribute:: obb2.origin
   :type: vec2

   Origin corner of the OBB

.. py:attribute:: obb2.tangent
   :type: vec2

   First edge vector (width direction)

.. py:attribute:: obb2.normal
   :type: vec2

   Second edge vector (height direction)

Methods
~~~~~~~

.. py:method:: obb2.world_to_local_point(point) -> vec2

   Transform a point from world to local OBB coordinates.
   In local space, origin maps to (0, 0) and the opposite corner to (1, 1).

   :param vec2 point: Point in world coordinates
   :return: Point in local coordinates
   :rtype: vec2

.. py:method:: obb2.world_to_local_vector(vector) -> vec2

   Transform a direction vector from world to local OBB coordinates.

   :param vec2 vector: Vector in world coordinates
   :return: Vector in local coordinates
   :rtype: vec2

.. py:method:: obb2.contains(point) -> bool

   Test if a point is inside the OBB.

   :param vec2 point: Point to test
   :return: True if point is inside or on the boundary
   :rtype: bool

.. py:method:: obb2.get_vertex(vertex_number) -> vec2

   Get vertex position by index (0-3).

   :param int vertex_number: Vertex index
   :return: Vertex position
   :rtype: vec2

.. py:method:: obb2.get_normal(side_number) -> vec2

   Get outward normal for a side (0-3).

   :param int side_number: Side index
   :return: Normalized outward-facing normal vector
   :rtype: vec2

.. py:method:: obb2.get_aabb() -> aabb2

   Get axis-aligned bounding box containing this OBB.

   :return: Bounding AABB
   :rtype: aabb2

obb3 - 3D OBB
-------------

Float precision 3D oriented bounding box.

A 3D OBB is defined by an origin point and three edge vectors (tangent, normal, binormal).
The eight vertices are all combinations of ``origin + {0|tangent} + {0|normal} + {0|binormal}``.

Constructor
~~~~~~~~~~~

.. code-block:: python

   obb3()                                      # Default (uninitialized)
   obb3(aabb)                                  # From axis-aligned bounding box
   obb3(origin, tangent, normal, binormal)     # From origin and edge vectors

Attributes
~~~~~~~~~~

.. py:attribute:: obb3.origin
   :type: vec3

   Origin corner of the OBB

.. py:attribute:: obb3.tangent
   :type: vec3

   First edge vector (X-like direction)

.. py:attribute:: obb3.normal
   :type: vec3

   Second edge vector (Y-like direction)

.. py:attribute:: obb3.binormal
   :type: vec3

   Third edge vector (Z-like direction)

Methods
~~~~~~~

.. py:method:: obb3.get_centre() -> vec3

   Get the center point of the OBB.

   :return: Center point
   :rtype: vec3

.. py:method:: obb3.get_volume() -> float

   Get the volume of the OBB.

   :return: Volume (product of edge lengths)
   :rtype: float

.. py:method:: obb3.get_aabb() -> aabb3

   Get axis-aligned bounding box containing this OBB.

   :return: Bounding AABB
   :rtype: aabb3

.. py:method:: obb3.get_vertex(vertex_number) -> vec3

   Get vertex position by index (0-7). Vertices are numbered using binary encoding:
   bit 0 = include tangent, bit 1 = include normal, bit 2 = include binormal.

   :param int vertex_number: Vertex index
   :return: Vertex position
   :rtype: vec3

.. py:method:: obb3.contains(point) -> bool

   Test if a point is inside the OBB.

   :param vec3 point: Point to test
   :return: True if point is inside or on the boundary
   :rtype: bool

.. py:method:: obb3.test_intersection(other) -> bool

   Test intersection with another OBB using the separating axis theorem (SAT).

   :param obb3 other: OBB to test against
   :return: True if the OBBs intersect
   :rtype: bool

Double Precision Variants
-------------------------

All OBB types have double-precision variants:

* ``obb2d`` - Double precision 2D OBB
* ``obb3d`` - Double precision 3D OBB

They have the same API as their float counterparts, but use ``double`` instead of ``float``.

Example
~~~~~~~

.. code-block:: python

   import pyrove

   # Create OBB from AABB
   aabb = pyrove.aabb3(pyrove.vec3(0, 0, 0), pyrove.vec3(2, 1, 3))
   box = pyrove.obb3(aabb)

   # Create rotated OBB from edge vectors
   origin = pyrove.vec3(0, 0, 0)
   tangent = pyrove.vec3(1, 1, 0)    # 45-degree rotation in XY plane
   normal = pyrove.vec3(-1, 1, 0)
   binormal = pyrove.vec3(0, 0, 1)
   rotated_box = pyrove.obb3(origin, tangent, normal, binormal)

   # Test intersection
   if box.test_intersection(rotated_box):
       print("OBBs intersect!")

   # Get bounding AABB
   bounding_aabb = rotated_box.get_aabb()
   print(f"Bounding AABB: lo={bounding_aabb.lo}, hi={bounding_aabb.hi}")

   # Point containment test
   point = pyrove.vec3(0.5, 0.5, 0.5)
   if box.contains(point):
       print("Point is inside the OBB")

   # Get volume
   print(f"Volume: {box.get_volume()}")

   # Get all vertices
   for i in range(8):
       vertex = box.get_vertex(i)
       print(f"Vertex {i}: {vertex}")
