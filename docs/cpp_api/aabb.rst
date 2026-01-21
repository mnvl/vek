AABB
====

Axis-aligned bounding box (N-dimensional).

Overview
--------

The ``aabb`` template class represents an axis-aligned bounding box, defined by
minimum (``lo``) and maximum (``hi``) corner points. AABBs are fundamental
primitives for:

- **Broad-phase collision detection**: Fast overlap tests before expensive narrow-phase
- **Spatial partitioning**: Building octrees, BVH, and other spatial data structures
- **View frustum culling**: Quick rejection of objects outside the view

Template Parameters
-------------------

- ``N``: Dimensionality (2 for rectangle, 3 for box)
- ``T``: Scalar type (default ``scalar`` which is typically ``float``)

Common Specializations
----------------------

- ``aabb<2, float>``: 2D rectangle (screen bounds, UI elements)
- ``aabb<3, float>``: 3D box (game objects, spatial queries)
- ``aabb<3, double>``: High-precision 3D box

Usage Example
-------------

.. code-block:: cpp

   #include "aabb.h"
   using namespace rove;

   // Create AABB from two corners
   aabb<3, float> box(vec3(0, 0, 0), vec3(10, 5, 8));

   // Build AABB from points
   aabb<3, float> bounds;
   bounds.null();  // Initialize for accumulation
   bounds.extend(vec3(1, 2, 3));
   bounds.extend(vec3(-1, 0, 5));

   // Test intersection
   if (box.test_intersection(bounds)) {
       // Boxes overlap
   }

   // Point containment
   if (box.contains(vec3(5, 2, 4))) {
       // Point is inside
   }

API Reference
-------------

.. doxygenfile:: aabb.h
   :project: rove
