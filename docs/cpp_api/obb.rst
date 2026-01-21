OBB
===

Oriented bounding box with 2D and 3D specializations.

Overview
--------

The ``obb`` template class represents an oriented bounding box that can be
rotated arbitrarily in space. Unlike axis-aligned bounding boxes (AABBs),
OBBs can provide tighter fits for rotated objects, but at the cost of more
expensive intersection tests.

An OBB is defined by:

- **Origin**: A corner point of the box
- **Edge vectors**: Tangent, normal (and binormal for 3D) vectors defining the box edges

Template Parameters
-------------------

- ``N``: Dimensionality (2 or 3, with explicit specializations)
- ``T``: Scalar type (default ``scalar`` which is typically ``float``)

2D OBB (obb<2, T>)
------------------

A 2D OBB is defined by an origin and two edge vectors (tangent and normal).
The four vertices are:

- ``origin``
- ``origin + tangent``
- ``origin + tangent + normal``
- ``origin + normal``

3D OBB (obb<3, T>)
------------------

A 3D OBB is defined by an origin and three edge vectors (tangent, normal, binormal).
The eight vertices are all combinations of ``origin + {0|tangent} + {0|normal} + {0|binormal}``.

Key Features
------------

- **Intersection tests**: SAT-based intersection with other OBBs and triangles
- **Collision detection**: Sweep tests with moving objects
- **Coordinate transformation**: World-to-local and local-to-world transforms
- **AABB conversion**: Get bounding AABB for broad-phase culling

Usage Example
-------------

.. code-block:: cpp

   #include "obb.h"
   using namespace rove;

   // Create OBB from AABB
   aabb<3, float> aabb(vec3(0, 0, 0), vec3(2, 1, 3));
   obb<3, float> box(aabb);

   // Create rotated OBB from edge vectors
   obb<3, float> rotated(
       vec3(0, 0, 0),           // origin
       vec3(1, 1, 0),           // tangent (45° in XY)
       vec3(-1, 1, 0),          // normal
       vec3(0, 0, 1)            // binormal
   );

   // Test intersection
   if (box.test_intersection(rotated)) {
       // OBBs intersect
   }

   // Point containment
   if (box.contains(vec3(1, 0.5, 1.5))) {
       // Point is inside
   }

   // Get bounding AABB for broad-phase
   aabb<3, float> bounds;
   box.get_aabb(bounds);

   // Collision detection with moving OBB
   vec3 velocity(1, 0, 0);
   if (box.test_collision(rotated, velocity, 0.0f, 1.0f)) {
       // Collision will occur in time [0, 1]
   }

API Reference
-------------

.. doxygenfile:: obb.h
   :project: rove
