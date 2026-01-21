Working with Quaternions
========================

Quaternions provide smooth rotation interpolation without gimbal lock.

Creating Quaternions
--------------------

.. code-block:: python

   import pyrove

   # Identity (no rotation)
   q = pyrove.quat(0.0, 0.0, 0.0, 1.0)
   q.identity()

   # From components (x, y, z, w)
   q = pyrove.quat(0.0, 0.707, 0.0, 0.707)  # 90° around Y

Quaternion Operations
---------------------

Normalization
~~~~~~~~~~~~~

.. code-block:: python

   q = pyrove.quat(1.0, 2.0, 3.0, 4.0)

   # Normalize in place
   q.normalize()

   # Or create normalized copy
   q_norm = q.normalized()

   # Check norm
   print(q.norm())  # Should be ~1.0 after normalization

Using with Matrices
-------------------

.. code-block:: python

   # Create quaternion rotation
   q = pyrove.quat(0.0, 0.707, 0.0, 0.707)  # 90° around Y

   # Convert to matrix
   m = pyrove.mat4()
   m.rotation(q)

   # Apply rotation
   v = pyrove.vec3(1.0, 0.0, 0.0)
   v_rotated = m * v

Spherical Linear Interpolation (SLERP)
---------------------------------------

SLERP provides smooth interpolation between rotations.

Using the slerp Function
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: python

   import pyrove

   # Start and end rotations
   q1 = pyrove.quat(0.0, 0.0, 0.0, 1.0)  # No rotation
   q2 = pyrove.quat(0.0, 0.707, 0.0, 0.707)  # 90° around Y

   # Interpolate at 50%
   q_mid = pyrove.slerp(q1, q2, 0.5)  # 45° around Y

   # Animate over time
   for t in [0.0, 0.25, 0.5, 0.75, 1.0]:
       q = pyrove.slerp(q1, q2, t)
       print(f"At t={t}: {q}")

Using quat_slerper
~~~~~~~~~~~~~~~~~~

For repeated interpolation, use ``quat_slerper`` for better performance:

.. code-block:: python

   slerper = pyrove.quat_slerper()
   slerper.setup(q1, q2)

   # Now interpolate multiple times
   q_25 = slerper.interpolate(0.25)
   q_50 = slerper.interpolate(0.5)
   q_75 = slerper.interpolate(0.75)

Example: Smooth Camera Rotation
--------------------------------

.. code-block:: python

   import pyrove
   import time

   class Camera:
       def __init__(self):
           self.orientation = pyrove.quat(0.0, 0.0, 0.0, 1.0)

       def rotate_to(self, target_quat, duration):
           \"\"\"Smoothly rotate camera to target orientation.\"\"\"
           start_quat = self.orientation
           slerper = pyrove.quat_slerper()
           slerper.setup(start_quat, target_quat)

           start_time = time.time()
           while True:
               elapsed = time.time() - start_time
               t = min(elapsed / duration, 1.0)

               self.orientation = slerper.interpolate(t)

               if t >= 1.0:
                   break

               time.sleep(0.016)  # ~60 FPS

       def get_view_matrix(self):
           m = pyrove.mat4()
           m.rotation(self.orientation)
           return m

   # Usage
   camera = Camera()

   # Rotate 90° around Y over 2 seconds
   target = pyrove.quat(0.0, 0.707, 0.0, 0.707)
   camera.rotate_to(target, 2.0)

Example: Quaternion Chain
--------------------------

.. code-block:: python

   import pyrove

   # Combine multiple rotations
   q1 = pyrove.quat(0.707, 0.0, 0.0, 0.707)  # 90° around X
   q2 = pyrove.quat(0.0, 0.707, 0.0, 0.707)  # 90° around Y

   # Convert to matrices and multiply
   m1 = pyrove.mat4()
   m1.rotation(q1)

   m2 = pyrove.mat4()
   m2.rotation(q2)

   combined = m1 * m2

Double Precision
----------------

.. code-block:: python

   # Double precision quaternion
   dq = pyrove.dquat(0.0, 0.0, 0.0, 1.0)

   # Double precision slerper
   dslerper = pyrove.dquat_slerper()
