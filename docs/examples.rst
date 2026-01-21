Examples
========

Practical examples demonstrating common use cases.

3D Object Transformation
------------------------

.. code-block:: python

   import pyrove

   class Object3D:
       def __init__(self):
           self.position = pyrove.vec3(0, 0, 0)
           self.rotation = pyrove.quat(0, 0, 0, 1)
           self.scale = pyrove.vec3(1, 1, 1)

       def get_matrix(self):
           m = pyrove.mat4()
           m.identity()
           m.translate(self.position)
           m.rotate(self.rotation)
           m.scale(self.scale)
           return m

       def move(self, offset):
           self.position += offset

       def rotate_around_y(self, angle):
           axis = pyrove.vec3(0, 1, 0)
           rot = pyrove.quat(0, 0, 0, 1)
           # Simplified - in practice, create quaternion from axis-angle
           m = pyrove.mat4()
           m.rotation(axis, angle)

   # Create and manipulate object
   obj = Object3D()
   obj.move(pyrove.vec3(1, 0, 0))
   obj.rotate_around_y(1.57)  # 90 degrees

Ray-Sphere Intersection
------------------------

.. code-block:: python

   import pyrove
   import math

   def ray_sphere_intersection(ray_origin, ray_dir, sphere_center, sphere_radius):
       \"\"\"Check if ray intersects sphere.\"\"\"
       oc = ray_origin - sphere_center
       a = ray_dir.dot(ray_dir)
       b = 2.0 * oc.dot(ray_dir)
       c = oc.dot(oc) - sphere_radius * sphere_radius
       discriminant = b * b - 4 * a * c

       if discriminant < 0:
           return None  # No intersection

       t = (-b - math.sqrt(discriminant)) / (2.0 * a)
       if t < 0:
           return None  # Intersection behind ray

       return ray_origin + ray_dir * t

   # Test
   origin = pyrove.vec3(0, 0, 0)
   direction = pyrove.vec3(1, 0, 0)
   direction.normalize()

   sphere_center = pyrove.vec3(5, 0, 0)
   sphere_radius = 1.0

   hit_point = ray_sphere_intersection(origin, direction, sphere_center, sphere_radius)
   if hit_point:
       print(f"Hit at {hit_point}")

Camera Controller
-----------------

.. code-block:: python

   import pyrove
   import math

   class FPSCamera:
       def __init__(self):
           self.position = pyrove.vec3(0, 0, 0)
           self.yaw = 0.0  # Rotation around Y
           self.pitch = 0.0  # Rotation around X

       def get_view_matrix(self):
           # Calculate forward vector
           forward = pyrove.vec3(
               math.cos(self.yaw) * math.cos(self.pitch),
               math.sin(self.pitch),
               math.sin(self.yaw) * math.cos(self.pitch)
           )
           forward.normalize()

           target = self.position + forward
           up = pyrove.vec3(0, 1, 0)

           view = pyrove.mat4()
           view.lookat(self.position, target, up)
           return view

       def move_forward(self, distance):
           forward = pyrove.vec3(
               math.cos(self.yaw),
               0,
               math.sin(self.yaw)
           )
           self.position += forward * distance

       def rotate(self, yaw_delta, pitch_delta):
           self.yaw += yaw_delta
           self.pitch += pitch_delta
           self.pitch = max(-math.pi/2, min(math.pi/2, self.pitch))

   # Usage
   camera = FPSCamera()
   camera.move_forward(5.0)
   camera.rotate(0.1, 0.05)
   view_matrix = camera.get_view_matrix()

Billboard Effect
----------------

.. code-block:: python

   import pyrove

   def create_billboard_matrix(camera_position, billboard_position):
       \"\"\"Create matrix to make object face camera.\"\"\"
       # Calculate direction from billboard to camera
       direction = camera_position - billboard_position
       direction.y = 0  # Keep upright
       direction.normalize()

       # Calculate right vector
       up = pyrove.vec3(0, 1, 0)
       right = pyrove.cross(up, direction)
       right.normalize()

       # Recalculate up for perfect perpendicularity
       up = pyrove.cross(direction, right)

       # Build matrix
       m = pyrove.mat4()
       m.identity()

       # Set orientation (columns of rotation matrix)
       for i in range(3):
           m.set(0, i, right.i[i])
           m.set(1, i, up.i[i])
           m.set(2, i, direction.i[i])

       # Set position
       for i in range(3):
           m.set(3, i, billboard_position.i[i])

       return m

Smooth Damping
--------------

.. code-block:: python

   import pyrove

   class SmoothedTransform:
       def __init__(self):
           self.current = pyrove.vec3(0, 0, 0)
           self.target = pyrove.vec3(0, 0, 0)
           self.velocity = pyrove.vec3(0, 0, 0)

       def smooth_damp(self, dt, smooth_time=0.3):
           \"\"\"Smoothly interpolate towards target.\"\"\"
           omega = 2.0 / smooth_time
           x = omega * dt
           exp = 1.0 / (1.0 + x + 0.48 * x * x + 0.235 * x * x * x)

           change = self.current - self.target
           temp = (self.velocity + change * omega) * dt
           self.velocity = (self.velocity - temp * omega) * exp
           self.current = self.target + (change + temp) * exp

       def set_target(self, target):
           self.target = target

   # Usage in game loop
   transform = SmoothedTransform()
   transform.set_target(pyrove.vec3(10, 0, 0))

   dt = 1.0 / 60.0  # 60 FPS
   for _ in range(60):
       transform.smooth_damp(dt)
       print(transform.current)
