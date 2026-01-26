Plane Class
===========

The Plane class represents an infinite plane in 3D space, defined by the equation
Ax + By + Cz + D = 0. It provides methods for point classification, ray intersection
tests, and containment queries. Available in float and double precision.

plane - 3D Plane
----------------

Float precision 3D plane.

Constructor
~~~~~~~~~~~

.. code-block:: python

   plane()                    # Default constructor (uninitialized)
   plane(origin, normal)      # From a point on the plane and normal vector
   plane(d, normal)           # From distance d and normal vector

Attributes
~~~~~~~~~~

.. py:attribute:: plane.A
   :type: float

   Coefficient A in plane equation Ax + By + Cz + D = 0

.. py:attribute:: plane.B
   :type: float

   Coefficient B in plane equation Ax + By + Cz + D = 0

.. py:attribute:: plane.C
   :type: float

   Coefficient C in plane equation Ax + By + Cz + D = 0

.. py:attribute:: plane.D
   :type: float

   Coefficient D in plane equation Ax + By + Cz + D = 0

Methods
~~~~~~~

.. py:method:: plane.set(origin, normal)

   Set plane from a point on the plane and normal vector.

   :param vec3 origin: A point on the plane
   :param vec3 normal: Normal vector to the plane

.. py:method:: plane.set(d, normal)

   Set plane from distance and normal vector.

   :param float d: Distance from origin
   :param vec3 normal: Normal vector to the plane

.. py:method:: plane.apply(point) -> float

   Evaluate the plane equation at a given point (Ax + By + Cz + D).

   :param vec3 point: Point to evaluate
   :return: Signed distance (0 if on plane, positive on one side, negative on other)
   :rtype: float

.. py:method:: plane.test_intersection(ray) -> bool

   Test if a ray intersects the plane.

   :param ray3 ray: Ray to test
   :return: True if ray intersects the plane
   :rtype: bool

.. py:method:: plane.contains(point) -> bool

   Test if a point lies on the plane.

   :param vec3 point: Point to test
   :return: True if point is on the plane (within tolerance)
   :rtype: bool

.. py:method:: plane.contains(ray) -> bool

   Test if a ray lies entirely on the plane.

   :param ray3 ray: Ray to test
   :return: True if ray is on the plane
   :rtype: bool

.. py:method:: plane.parallel(ray) -> bool

   Test if a ray is parallel to the plane (no intersection).

   :param ray3 ray: Ray to test
   :return: True if ray is parallel to the plane
   :rtype: bool

.. py:method:: plane.trace(ray) -> float

   Calculate the parameter t where a ray intersects the plane.
   The intersection point is ray.apply(t).

   :param ray3 ray: Ray to trace
   :return: Parameter t (may be negative if intersection is behind ray origin)
   :rtype: float

.. py:method:: plane.get_origin() -> vec3

   Get a point on the plane (projects origin onto plane).

   :return: Point on the plane
   :rtype: vec3

.. py:method:: plane.get_normal() -> vec3

   Get the plane normal vector.

   :return: Normal vector (A, B, C)
   :rtype: vec3

.. py:method:: plane.is_correct() -> bool

   Check if the plane equation is valid (normal vector is not zero).

   :return: True if plane is valid
   :rtype: bool

.. py:method:: plane.classify(point) -> int

   Classify a point's position relative to the plane.

   :param vec3 point: Point to classify
   :return: Classification code (1=POSITIVE side, 2=INTERSECTS/on plane, 3=NEGATIVE side)
   :rtype: int

Double Precision Variant
------------------------

* ``planed`` - Double precision 3D plane

The double-precision variant has the same API as the float version, but uses
``double`` instead of ``float``.

Example
~~~~~~~

.. code-block:: python

   import pyrove

   # Create a plane from a point and normal
   origin = pyrove.vec3(0, 0, 0)
   normal = pyrove.vec3(0, 1, 0)  # Horizontal plane (XZ plane)
   p = pyrove.plane(origin, normal)

   # Get plane coefficients
   print(f"Plane equation: {p.A}x + {p.B}y + {p.C}z + {p.D} = 0")

   # Test if point is on plane
   test_point = pyrove.vec3(5, 0, 3)
   if p.contains(test_point):
       print("Point is on the plane")

   # Classify point position
   above = pyrove.vec3(0, 5, 0)
   classification = p.classify(above)
   # classification == 1 (POSITIVE side)

   # Ray-plane intersection
   ray = pyrove.ray3(pyrove.vec3(0, 10, 0), pyrove.vec3(0, -1, 0))
   if p.test_intersection(ray):
       t = p.trace(ray)
       intersection = ray.apply(t)
       print(f"Ray intersects plane at t={t}, point={intersection}")

   # Check if ray is parallel to plane
   parallel_ray = pyrove.ray3(pyrove.vec3(0, 5, 0), pyrove.vec3(1, 0, 0))
   if p.parallel(parallel_ray):
       print("Ray is parallel to plane (no intersection)")

   # Evaluate plane equation at a point
   distance = p.apply(pyrove.vec3(0, 7, 0))
   print(f"Signed distance from plane: {distance}")

   # Double precision example
   pd = pyrove.planed(pyrove.vec3d(0, 0, 0), pyrove.vec3d(0, 0, 1))
   normal_d = pd.get_normal()
