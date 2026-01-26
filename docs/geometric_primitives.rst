Geometric Primitives
====================

The rove library provides a comprehensive set of geometric primitive classes
for 2D and 3D space. These primitives are commonly used in computer graphics,
collision detection, ray tracing, and robotics applications.

Overview
--------

The library includes six main geometric primitive types:

* **Ray** - Half-infinite lines for ray casting and tracing
* **Line** - Line segments with finite endpoints
* **Plane** - Infinite planes in 3D space
* **Triangle** - Triangular polygons with extensive geometric properties
* **Capsule** - Swept spheres (cylinders with hemispherical caps)
* **Frustum** - View frustums for camera culling

All geometric primitives support both float and double precision, and come in
2D and 3D variants where applicable.

Precision and Dimensions
-------------------------

The naming convention follows this pattern:

* **Float precision**: ``ray2``, ``ray3``, ``line2``, ``line3``, etc.
* **Double precision**: ``ray2d``, ``ray3d``, ``line2d``, ``line3d``, etc.
* **Plane and Frustum**: 3D only (``plane``, ``planed``, ``frustum``, ``frustumd``)

Choose float precision for most graphics applications where performance is
critical. Use double precision when you need higher accuracy for scientific
computing or when dealing with very large coordinate spaces.

Ray Casting and Tracing
------------------------

Rays are fundamental for many algorithms including ray tracing, collision
detection, and picking.

.. code-block:: python

   import pyrove

   # Create a ray from origin in direction
   origin = pyrove.vec3(0, 0, 0)
   direction = pyrove.vec3(1, 0, 0)
   ray = pyrove.ray3(origin, direction)

   # Get point along ray
   point = ray.apply(5.0)  # Point at t=5

   # Calculate distance from ray to a point
   test_point = pyrove.vec3(10, 5, 0)
   distance = ray.distance(test_point)

   # Test if point lies on ray
   if ray.contains(pyrove.vec3(3, 0, 0)):
       print("Point is on the ray")

Ray-Plane Intersection
-----------------------

A common operation is finding where a ray intersects a plane.

.. code-block:: python

   import pyrove

   # Create a horizontal plane at y=0
   plane = pyrove.plane(pyrove.vec3(0, 0, 0), pyrove.vec3(0, 1, 0))

   # Create a ray shooting downward
   ray = pyrove.ray3(pyrove.vec3(5, 10, 5), pyrove.vec3(0, -1, 0))

   # Test intersection
   if plane.test_intersection(ray):
       # Calculate intersection parameter
       t = plane.trace(ray)

       # Get intersection point
       hit_point = ray.apply(t)
       print(f"Ray hits plane at {hit_point}")

   # Check if ray is parallel to plane
   if plane.parallel(ray):
       print("Ray will never intersect plane")

Ray-Triangle Intersection
--------------------------

For ray tracing and picking, testing ray-triangle intersection is essential.

.. code-block:: python

   import pyrove

   # Define a triangle
   tri = pyrove.triangle3(
       pyrove.vec3(0, 0, 0),
       pyrove.vec3(1, 0, 0),
       pyrove.vec3(0, 1, 0)
   )

   # Create a ray
   ray = pyrove.ray3(pyrove.vec3(0.25, 0.25, 10), pyrove.vec3(0, 0, -1))

   # Test intersection within range
   if tri.trace(ray, 0.0, 100.0):
       print("Ray hits triangle!")

   # You can also test point containment
   point = pyrove.vec3(0.25, 0.25, 0)
   if tri.contains(point):
       print("Point is inside triangle")

Line Segments and Distance Queries
-----------------------------------

Line segments are useful for edges, paths, and distance calculations.

.. code-block:: python

   import pyrove

   # Create a line segment
   line = pyrove.line3(pyrove.vec3(0, 0, 0), pyrove.vec3(10, 0, 0))

   # Calculate distance from line to point
   point = pyrove.vec3(5, 3, 0)
   distance = line.distance(point)

   # Get line direction and length
   direction = line.direction()
   length_squared = line.length_sq()

   # Convert to ray
   ray = line.get_ray()

   # Get bounding box
   bbox = line.get_aabb()

Triangle Properties and Centers
--------------------------------

The triangle class provides many geometric properties and special points.

.. code-block:: python

   import pyrove

   # Create a triangle
   tri = pyrove.triangle3(
       pyrove.vec3(0, 0, 0),
       pyrove.vec3(3, 0, 0),
       pyrove.vec3(0, 4, 0)
   )

   # Basic properties
   area = tri.area()
   perimeter = tri.perimeter()
   normal = tri.get_normal()

   # Four classical triangle centers
   centroid = tri.cog()              # Center of mass
   circumcenter = tri.circumcenter() # Center of circumscribed circle
   incenter = tri.incenter()         # Center of inscribed circle
   orthocenter = tri.orthocenter()   # Intersection of altitudes

   # Circle properties
   circumradius = tri.circumcircle_radius()
   inradius = tri.incircle_radius()

   # Get actual circle objects
   circumcircle = tri.get_circumcircle()  # Returns sphere3
   incircle = tri.get_incircle()          # Returns sphere3

   # Excircles (circles tangent to one side and extensions of other two)
   excenter_a = tri.excenter_A()
   exradius_a = tri.excircle_A_radius()

   # Nine-point circle (Feuerbach circle)
   feuerbach_center = tri.feuerbach_center()
   feuerbach_radius = tri.feuerbach_radius()

Capsule Collision Detection
----------------------------

Capsules are excellent for swept sphere tests and character collision.

.. code-block:: python

   import pyrove

   # Create a capsule representing a character
   # (vertical cylinder with hemispherical caps)
   bottom = pyrove.vec3(0, 0, 0)
   top = pyrove.vec3(0, 2, 0)
   radius = 0.5
   character = pyrove.capsule3(bottom, top, radius)

   # Test if point is inside capsule
   test_point = pyrove.vec3(0.3, 1.0, 0.2)
   if character.contains(test_point):
       print("Collision detected!")

   # Calculate distance from surface
   obstacle = pyrove.vec3(2, 1, 0)
   distance = character.distance(obstacle)
   print(f"Distance to obstacle: {distance}")

   # Capsules can represent swept spheres
   # (the volume traced by a moving sphere)
   sphere_start = pyrove.vec3(0, 0, 0)
   sphere_end = pyrove.vec3(10, 0, 0)
   sphere_radius = 1.0
   swept_volume = pyrove.capsule3(sphere_start, sphere_end, sphere_radius)

View Frustum Culling
--------------------

Frustums are used to cull objects outside the camera's view volume.

.. code-block:: python

   import pyrove

   # Create view and projection matrices
   proj = pyrove.mat4()
   proj.perspective(3.14159 / 4.0, 16.0 / 9.0, 0.1, 100.0)

   view = pyrove.mat4()
   view.look_at(
       pyrove.vec3(0, 5, 10),  # camera position
       pyrove.vec3(0, 0, 0),   # look at target
       pyrove.vec3(0, 1, 0)    # up vector
   )

   # Create frustum from view-projection matrix
   view_proj = proj * view
   frustum = pyrove.frustum(view_proj)

   # Test if object is visible
   object_center = pyrove.vec3(0, 0, 0)
   if frustum.contains(object_center):
       print("Object is in view")

   # More accurate test with bounding box
   object_bbox = pyrove.aabb3(
       pyrove.vec3(-1, -1, -1),
       pyrove.vec3(1, 1, 1)
   )
   if frustum.test_intersection(object_bbox):
       print("Object should be rendered")
   else:
       print("Object culled (not visible)")

   # Access individual frustum planes
   near_plane = frustum.get_plane(pyrove.PLANE_NEAR)
   left_plane = frustum.get_plane(pyrove.PLANE_LEFT)

   # Update frustum when camera moves
   new_view_proj = get_new_camera_matrix()
   frustum.load(new_view_proj)

Point Classification
--------------------

Planes provide methods to classify points relative to the plane.

.. code-block:: python

   import pyrove

   # Create a plane (XZ plane at y=0)
   plane = pyrove.plane(pyrove.vec3(0, 0, 0), pyrove.vec3(0, 1, 0))

   # Classify points
   above = pyrove.vec3(0, 5, 0)
   on_plane = pyrove.vec3(3, 0, 7)
   below = pyrove.vec3(0, -3, 0)

   # Returns: 1=POSITIVE, 2=INTERSECTS, 3=NEGATIVE
   print(plane.classify(above))      # 1 (positive side)
   print(plane.classify(on_plane))   # 2 (on plane)
   print(plane.classify(below))      # 3 (negative side)

   # Signed distance from plane
   distance = plane.apply(above)     # Returns positive value
   print(f"Distance: {distance}")

   # Check if point is on plane
   if plane.contains(on_plane):
       print("Point is on the plane")

Working with 2D Primitives
---------------------------

The 2D variants are useful for 2D games and UI applications.

.. code-block:: python

   import pyrove

   # 2D ray for 2D ray casting
   ray2d = pyrove.ray2(pyrove.vec2(0, 0), pyrove.vec2(1, 0))
   point2d = ray2d.apply(5.0)

   # 2D line segment
   line2d = pyrove.line2(pyrove.vec2(0, 0), pyrove.vec2(10, 10))
   distance2d = line2d.distance(pyrove.vec2(5, 0))

   # 2D capsule (rounded rectangle)
   capsule2d = pyrove.capsule2(
       pyrove.vec2(0, 0),
       pyrove.vec2(10, 0),
       2.0
   )
   if capsule2d.contains(pyrove.vec2(5, 1)):
       print("Point inside 2D capsule")

Combining with Bounding Volumes
--------------------------------

Geometric primitives work seamlessly with bounding volumes (AABB, OBB, Sphere).

.. code-block:: python

   import pyrove

   # Get AABB from line segment
   line = pyrove.line3(pyrove.vec3(0, 0, 0), pyrove.vec3(10, 5, 3))
   bbox = line.get_aabb()
   print(f"Bounding box: {bbox.lo} to {bbox.hi}")

   # Get plane from triangle
   tri = pyrove.triangle3(
       pyrove.vec3(0, 0, 0),
       pyrove.vec3(1, 0, 0),
       pyrove.vec3(0, 1, 0)
   )
   plane = tri.get_plane()

   # Get sphere from triangle (circumcircle)
   circumsphere = tri.get_circumcircle()

   # Frustum culling with AABB and OBB
   frustum = pyrove.frustum(view_projection_matrix)

   # Test AABB
   if frustum.test_intersection(aabb):
       print("AABB visible")

   # Test OBB (for rotated objects)
   if frustum.test_intersection(obb):
       print("OBB visible")

Best Practices
--------------

1. **Use appropriate precision**: Float is sufficient for most graphics, use double for scientific accuracy
2. **Avoid unnecessary square roots**: Use ``distance_sq()`` and ``length_sq()`` when you only need to compare distances
3. **Normalize directions**: While ray directions don't need to be normalized, normalized directions make parameter t represent actual distance
4. **Cache geometric properties**: Triangle centers and radii don't change unless vertices change
5. **Update frustums efficiently**: Only call ``frustum.load()`` when the camera actually moves
6. **Use capsules for characters**: Capsules provide smooth collision without getting stuck on corners
7. **Batch intersection tests**: Test cheap point/sphere containment before expensive ray-triangle tests

Common Patterns
---------------

**Mouse Picking**

.. code-block:: python

   # Convert screen coordinates to world ray
   ray = screen_to_world_ray(mouse_x, mouse_y, camera)

   # Test against scene geometry
   closest_hit = None
   closest_t = float('inf')

   for triangle in scene_triangles:
       if triangle.trace(ray, 0.0, closest_t):
           # Found a hit closer than previous hits
           # Update closest_hit and closest_t
           pass

**Character Controller**

.. code-block:: python

   # Represent character as capsule
   character = pyrove.capsule3(feet_position, head_position, radius)

   # Test against world geometry
   for obstacle in world_obstacles:
       distance = character.distance(obstacle)
       if distance < 0.01:  # Small threshold for contact
           # Handle collision
           pass

**Spatial Queries**

.. code-block:: python

   # Find all triangles in view
   visible_triangles = []
   for triangle in all_triangles:
       bbox = get_triangle_bbox(triangle)
       if frustum.test_intersection(bbox):
           visible_triangles.append(triangle)

See Also
--------

* :doc:`api/ray` - Complete Ray API reference
* :doc:`api/line` - Complete Line API reference
* :doc:`api/plane` - Complete Plane API reference
* :doc:`api/triangle` - Complete Triangle API reference
* :doc:`api/capsule` - Complete Capsule API reference
* :doc:`api/frustum` - Complete Frustum API reference
* :doc:`api/aabb` - Axis-aligned bounding boxes
* :doc:`api/obb` - Oriented bounding boxes
