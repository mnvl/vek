Triangle Class
==============

The Triangle class represents a triangle in 3D space defined by three vertices.
It provides extensive geometric properties including area, perimeter, centers
(centroid, circumcenter, incenter, orthocenter), and special circles
(circumcircle, incircle, excircles, Feuerbach/nine-point circle). It also
supports containment tests and ray-triangle intersection. Available in float
and double precision.

triangle3 - 3D Triangle
-----------------------

Float precision 3D triangle.

Constructor
~~~~~~~~~~~

.. code-block:: python

   triangle3()               # Default constructor (uninitialized)
   triangle3(a, b, c)        # From three vertex points

Attributes
~~~~~~~~~~

.. py:attribute:: triangle3.A
   :type: vec3

   First vertex of the triangle

.. py:attribute:: triangle3.B
   :type: vec3

   Second vertex of the triangle

.. py:attribute:: triangle3.C
   :type: vec3

   Third vertex of the triangle

Basic Methods
~~~~~~~~~~~~~

.. py:method:: triangle3.construct(a, b, c)

   Set triangle vertices.

   :param vec3 a: First vertex
   :param vec3 b: Second vertex
   :param vec3 c: Third vertex

.. py:method:: triangle3.get_normal() -> vec3

   Calculate the triangle's normal vector using cross product (B-A) × (C-A).

   :return: Normal vector (not normalized)
   :rtype: vec3

.. py:method:: triangle3.get_plane() -> plane

   Get the plane containing the triangle.

   :return: Plane object
   :rtype: plane

.. py:method:: triangle3.perimeter() -> float

   Calculate the perimeter (sum of edge lengths).

   :return: Perimeter
   :rtype: float

.. py:method:: triangle3.area() -> float

   Calculate the triangle's area.

   :return: Area
   :rtype: float

Triangle Centers
~~~~~~~~~~~~~~~~

.. py:method:: triangle3.cog() -> vec3

   Calculate the center of gravity (centroid) - the average of the three vertices.

   :return: Centroid point
   :rtype: vec3

.. py:method:: triangle3.orthocenter() -> vec3

   Calculate the orthocenter - the intersection point of the three altitudes.

   :return: Orthocenter point
   :rtype: vec3

.. py:method:: triangle3.circumcenter() -> vec3

   Calculate the circumcenter - the center of the circumscribed circle
   (equidistant from all three vertices).

   :return: Circumcenter point
   :rtype: vec3

.. py:method:: triangle3.incenter() -> vec3

   Calculate the incenter - the center of the inscribed circle
   (equidistant from all three edges).

   :return: Incenter point
   :rtype: vec3

Circle Properties
~~~~~~~~~~~~~~~~~

.. py:method:: triangle3.circumcircle_radius() -> float

   Calculate the radius of the circumscribed circle.

   :return: Circumcircle radius
   :rtype: float

.. py:method:: triangle3.get_circumcircle() -> sphere3

   Get the circumscribed circle as a sphere.

   :return: Circumcircle sphere
   :rtype: sphere3

.. py:method:: triangle3.incircle_radius() -> float

   Calculate the radius of the inscribed circle.

   :return: Incircle radius
   :rtype: float

.. py:method:: triangle3.get_incircle() -> sphere3

   Get the inscribed circle as a sphere.

   :return: Incircle sphere
   :rtype: sphere3

Excircles
~~~~~~~~~

.. py:method:: triangle3.excenter_A() -> vec3

   Calculate the excircle center opposite to vertex A.

   :return: Excenter point
   :rtype: vec3

.. py:method:: triangle3.excircle_A_radius() -> float

   Calculate the radius of the excircle opposite to vertex A.

   :return: Excircle radius
   :rtype: float

.. py:method:: triangle3.excenter_B() -> vec3

   Calculate the excircle center opposite to vertex B.

   :return: Excenter point
   :rtype: vec3

.. py:method:: triangle3.excircle_B_radius() -> float

   Calculate the radius of the excircle opposite to vertex B.

   :return: Excircle radius
   :rtype: float

.. py:method:: triangle3.excenter_C() -> vec3

   Calculate the excircle center opposite to vertex C.

   :return: Excenter point
   :rtype: vec3

.. py:method:: triangle3.excircle_C_radius() -> float

   Calculate the radius of the excircle opposite to vertex C.

   :return: Excircle radius
   :rtype: float

Feuerbach Circle (Nine-Point Circle)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. py:method:: triangle3.feuerbach_center() -> vec3

   Calculate the center of the nine-point circle (Feuerbach circle), which
   passes through nine notable points: the midpoints of the sides, the feet
   of the altitudes, and the midpoints of the segments from vertices to the orthocenter.

   :return: Feuerbach circle center
   :rtype: vec3

.. py:method:: triangle3.feuerbach_radius() -> float

   Calculate the radius of the nine-point circle (always half the circumradius).

   :return: Feuerbach circle radius
   :rtype: float

Containment and Intersection
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. py:method:: triangle3.contains_on_plane(point) -> bool

   Test if a point on the triangle's plane is inside the triangle (2D test).
   Assumes the point already lies on the triangle's plane.

   :param vec3 point: Point to test (should be on triangle's plane)
   :return: True if point is inside the triangle
   :rtype: bool

.. py:method:: triangle3.contains(point) -> bool

   Test if a 3D point is inside the triangle (projects point onto plane first).

   :param vec3 point: Point to test
   :return: True if point is inside the triangle
   :rtype: bool

.. py:method:: triangle3.trace(ray, t_min, t_max) -> bool

   Test ray-triangle intersection within parameter range [t_min, t_max].

   :param ray3 ray: Ray to test
   :param float t_min: Minimum ray parameter
   :param float t_max: Maximum ray parameter
   :return: True if ray intersects triangle within range
   :rtype: bool

Double Precision Variant
------------------------

* ``triangle3d`` - Double precision 3D triangle

The double-precision variant has the same API as the float version, but uses
``double`` instead of ``float``.

Example
~~~~~~~

.. code-block:: python

   import pyrove

   # Create a triangle
   v1 = pyrove.vec3(0, 0, 0)
   v2 = pyrove.vec3(1, 0, 0)
   v3 = pyrove.vec3(0, 1, 0)
   tri = pyrove.triangle3(v1, v2, v3)

   # Basic properties
   print(f"Area: {tri.area()}")
   print(f"Perimeter: {tri.perimeter()}")
   print(f"Normal: {tri.get_normal()}")

   # Triangle centers
   centroid = tri.cog()
   circumcenter = tri.circumcenter()
   incenter = tri.incenter()
   orthocenter = tri.orthocenter()
   print(f"Centroid: {centroid}")

   # Circle properties
   circumradius = tri.circumcircle_radius()
   inradius = tri.incircle_radius()
   circumcircle = tri.get_circumcircle()
   incircle = tri.get_incircle()
   print(f"Circumradius: {circumradius}")

   # Excircles
   excenter_a = tri.excenter_A()
   exradius_a = tri.excircle_A_radius()
   print(f"Excircle A center: {excenter_a}, radius: {exradius_a}")

   # Nine-point circle (Feuerbach circle)
   feuerbach_center = tri.feuerbach_center()
   feuerbach_radius = tri.feuerbach_radius()
   print(f"Nine-point circle: center={feuerbach_center}, radius={feuerbach_radius}")

   # Containment test
   test_point = pyrove.vec3(0.25, 0.25, 0)
   if tri.contains(test_point):
       print("Point is inside the triangle")

   # Ray-triangle intersection
   ray = pyrove.ray3(pyrove.vec3(0.25, 0.25, 10), pyrove.vec3(0, 0, -1))
   if tri.trace(ray, 0.0, 100.0):
       print("Ray intersects the triangle")

   # Get plane containing triangle
   plane = tri.get_plane()
   print(f"Triangle plane: {plane.A}x + {plane.B}y + {plane.C}z + {plane.D} = 0")

   # Double precision example
   tri_d = pyrove.triangle3d(
       pyrove.vec3d(0, 0, 0),
       pyrove.vec3d(2, 0, 0),
       pyrove.vec3d(0, 2, 0)
   )
   area_d = tri_d.area()
