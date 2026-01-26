Frustum Class
=============

The Frustum class represents a view frustum in 3D space, typically used for
camera culling in rendering. It is defined by six planes (left, right, top,
bottom, near, far) and can be constructed from a view-projection matrix.
It provides methods for containment and intersection tests with points and
bounding volumes. Available in float and double precision.

frustum - 3D View Frustum
-------------------------

Float precision 3D frustum.

Constructor
~~~~~~~~~~~

.. code-block:: python

   frustum()                  # Default constructor (uninitialized)
   frustum(matrix)            # From 4x4 view-projection matrix

Attributes
~~~~~~~~~~

.. py:attribute:: frustum.planes
   :type: list[plane]

   List of six planes defining the frustum boundaries.
   Use plane indices to access: ``PLANE_LEFT``, ``PLANE_RIGHT``, ``PLANE_TOP``,
   ``PLANE_BOTTOM``, ``PLANE_NEAR``, ``PLANE_FAR``

Plane Index Constants
~~~~~~~~~~~~~~~~~~~~~

The following constants are available both as class attributes and module-level constants:

.. py:data:: frustum.PLANE_LEFT
   :type: int
   :value: 0

   Index for the left plane

.. py:data:: frustum.PLANE_RIGHT
   :type: int
   :value: 1

   Index for the right plane

.. py:data:: frustum.PLANE_TOP
   :type: int
   :value: 2

   Index for the top plane

.. py:data:: frustum.PLANE_BOTTOM
   :type: int
   :value: 3

   Index for the bottom plane

.. py:data:: frustum.PLANE_NEAR
   :type: int
   :value: 4

   Index for the near plane

.. py:data:: frustum.PLANE_FAR
   :type: int
   :value: 5

   Index for the far plane

The constants are also available at module level:

.. code-block:: python

   pyrove.PLANE_LEFT    # 0
   pyrove.PLANE_RIGHT   # 1
   pyrove.PLANE_TOP     # 2
   pyrove.PLANE_BOTTOM  # 3
   pyrove.PLANE_NEAR    # 4
   pyrove.PLANE_FAR     # 5

Methods
~~~~~~~

.. py:method:: frustum.load(matrix)

   Load frustum planes from a 4x4 view-projection matrix.
   Extracts the six frustum planes from the matrix.

   :param mat4 matrix: View-projection transformation matrix

.. py:method:: frustum.contains(point) -> bool

   Test if a point is inside the frustum.

   :param vec3 point: Point to test
   :return: True if point is inside all six planes
   :rtype: bool

.. py:method:: frustum.test_intersection(aabb) -> bool

   Test if an axis-aligned bounding box intersects the frustum.

   :param aabb3 aabb: Bounding box to test
   :return: True if AABB is partially or fully inside the frustum
   :rtype: bool

.. py:method:: frustum.test_intersection(obb) -> bool

   Test if an oriented bounding box intersects the frustum.

   :param obb3 obb: Oriented bounding box to test
   :return: True if OBB is partially or fully inside the frustum
   :rtype: bool

.. py:method:: frustum.get_plane(index) -> plane

   Get a specific plane by index.

   :param int index: Plane index (0-5, use constants like PLANE_LEFT)
   :return: The requested plane
   :rtype: plane

Double Precision Variant
------------------------

* ``frustumd`` - Double precision 3D frustum

The double-precision variant has the same API as the float version, but uses
``double`` instead of ``float``.

Example
~~~~~~~

.. code-block:: python

   import pyrove

   # Create projection matrix (perspective)
   fov = 3.14159 / 4.0  # 45 degrees
   aspect = 16.0 / 9.0
   near = 0.1
   far = 100.0

   proj = pyrove.mat4()
   proj.perspective(fov, aspect, near, far)

   # Create view matrix (looking at origin from (0, 0, 10))
   view = pyrove.mat4()
   view.look_at(
       pyrove.vec3(0, 0, 10),  # eye
       pyrove.vec3(0, 0, 0),   # target
       pyrove.vec3(0, 1, 0)    # up
   )

   # Combine into view-projection matrix
   view_proj = proj * view

   # Create frustum from view-projection matrix
   frust = pyrove.frustum(view_proj)

   # Test if point is in frustum
   point = pyrove.vec3(0, 0, 0)
   if frust.contains(point):
       print("Point is visible in frustum")

   # Test AABB intersection (frustum culling)
   bbox = pyrove.aabb3(pyrove.vec3(-1, -1, -1), pyrove.vec3(1, 1, 1))
   if frust.test_intersection(bbox):
       print("Object should be rendered (in frustum)")
   else:
       print("Object culled (outside frustum)")

   # Access individual planes
   near_plane = frust.get_plane(pyrove.PLANE_NEAR)
   left_plane = frust.get_plane(pyrove.PLANE_LEFT)

   # Or use class constants
   near_plane = frust.get_plane(frustum.PLANE_NEAR)

   # Access all planes
   for i, plane in enumerate(frust.planes):
       print(f"Plane {i}: {plane.A}x + {plane.B}y + {plane.C}z + {plane.D} = 0")

   # Update frustum when camera moves
   new_view_proj = get_updated_camera_matrix()
   frust.load(new_view_proj)

   # Test OBB intersection (for rotated objects)
   obb = pyrove.obb3()
   # ... configure OBB ...
   if frust.test_intersection(obb):
       print("Oriented box is visible")

   # Double precision example
   frust_d = pyrove.frustumd(view_proj_matrix_d)
   visible = frust_d.contains(pyrove.vec3d(0, 0, -5))
