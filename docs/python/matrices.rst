Working with Matrices
=====================

Matrix classes provide transformation operations for 2D and 3D graphics.

Creating Matrices
-----------------

.. code-block:: python

   import pyrove

   # 3x3 matrix (for 2D transformations)
   m3 = pyrove.mat3()
   m3.identity()

   # 4x4 matrix (for 3D transformations)
   m4 = pyrove.mat4()
   m4.identity()

Building Transformations
------------------------

Translation
~~~~~~~~~~~

.. code-block:: python

   m = pyrove.mat4()
   m.identity()
   m.translate(1.0, 2.0, 3.0)

   # Or with a vector
   offset = pyrove.vec3(1.0, 2.0, 3.0)
   m.translate(offset)

Rotation
~~~~~~~~

.. code-block:: python

   import math

   m = pyrove.mat4()
   m.identity()

   # Rotate around Y axis
   axis = pyrove.vec3(0.0, 1.0, 0.0)
   angle = math.pi / 4  # 45 degrees
   m.rotate(axis, angle)

   # Or from quaternion
   q = pyrove.quat(0.0, 0.707, 0.0, 0.707)
   m.rotation(q)

Scaling
~~~~~~~

.. code-block:: python

   m = pyrove.mat4()
   m.identity()
   m.scale(2.0, 2.0, 2.0)

   # Non-uniform scaling
   m.scale(2.0, 1.0, 0.5)

Combining Transformations
--------------------------

Order Matters
~~~~~~~~~~~~~

.. code-block:: python

   m = pyrove.mat4()
   m.identity()

   # Build complex transformation
   m.translate(10.0, 0.0, 0.0)  # First: move right
   m.rotate(pyrove.vec3(0.0, 1.0, 0.0), 1.57)  # Then: rotate
   m.scale(2.0, 2.0, 2.0)  # Finally: scale

   # Apply to vector
   v = pyrove.vec3(1.0, 0.0, 0.0)
   v_transformed = m * v

Matrix Multiplication
~~~~~~~~~~~~~~~~~~~~~

.. code-block:: python

   # Create separate transformations
   translate = pyrove.mat4()
   translate.identity()
   translate.translation(1.0, 2.0, 3.0)

   rotate = pyrove.mat4()
   rotate.identity()
   rotate.rotation(pyrove.vec3(0.0, 1.0, 0.0), 1.57)

   # Combine them
   combined = translate * rotate

Camera and Projection
---------------------

Look-At Matrix
~~~~~~~~~~~~~~

.. code-block:: python

   view = pyrove.mat4()

   eye = pyrove.vec3(0.0, 5.0, 10.0)  # Camera position
   target = pyrove.vec3(0.0, 0.0, 0.0)  # Look at origin
   up = pyrove.vec3(0.0, 1.0, 0.0)  # Up direction

   view.lookat(eye, target, up)

Perspective Projection
~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: python

   import math

   proj = pyrove.mat4()

   fovy = math.pi / 4  # 45 degree field of view
   aspect = 16.0 / 9.0  # Aspect ratio
   near = 0.1
   far = 100.0

   proj.perspective(fovy, aspect, near, far)

Orthographic Projection
~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: python

   proj = pyrove.mat4()

   width = 10.0
   height = 10.0
   near = 0.1
   far = 100.0

   proj.ortho(width, height, near, far)

Complete Camera Setup
~~~~~~~~~~~~~~~~~~~~~

.. code-block:: python

   # Model transformation
   model = pyrove.mat4()
   model.identity()
   model.translate(0.0, 0.0, -5.0)

   # View matrix (camera)
   view = pyrove.mat4()
   view.lookat(
       pyrove.vec3(0.0, 2.0, 5.0),  # Eye
       pyrove.vec3(0.0, 0.0, 0.0),  # Target
       pyrove.vec3(0.0, 1.0, 0.0)   # Up
   )

   # Projection matrix
   proj = pyrove.mat4()
   proj.perspective(1.57, 16.0/9.0, 0.1, 100.0)

   # Combine: MVP matrix
   mvp = proj * view * model

   # Transform vertex
   vertex = pyrove.vec3(1.0, 1.0, 1.0)
   clip_space = mvp * vertex

Matrix Operations
-----------------

Inverse
~~~~~~~

.. code-block:: python

   m = pyrove.mat4()
   m.translate(1.0, 2.0, 3.0)

   # Get inverse
   m_inv = m.inverse()

   # Verify: m * m_inv ≈ identity
   identity = m * m_inv

   # In-place inversion
   success = m.invert()
   if not success:
       print("Matrix is singular!")

Transpose
~~~~~~~~~

.. code-block:: python

   m = pyrove.mat4()
   # ... set up matrix ...

   # Get transpose
   m_t = m.transpose()

   # Or transpose in place
   m.transpose_inplace()

Determinant
~~~~~~~~~~~

.. code-block:: python

   m = pyrove.mat4()
   # ... set up matrix ...

   det = m.determinant()

   if abs(det) < 1e-6:
       print("Matrix is nearly singular")

Element Access
--------------

.. code-block:: python

   m = pyrove.mat4()
   m.identity()

   # Get element
   value = m.get(0, 0)  # m[0][0]

   # Set element
   m.set(1, 2, 5.0)  # m[1][2] = 5.0

   # Get column/row (mat4 only)
   col = m.column(0)  # vec4
   col3 = m.projected_column(0)  # vec3
   row3 = m.projected_row(0)  # vec3

Double Precision
----------------

.. code-block:: python

   # Float precision
   m_float = pyrove.mat4()

   # Double precision
   m_double = pyrove.dmat4()

Example: Object Hierarchy
--------------------------

.. code-block:: python

   import pyrove

   class Transform:
       def __init__(self):
           self.matrix = pyrove.mat4()
           self.matrix.identity()
           self.parent = None

       def get_world_matrix(self):
           if self.parent is None:
               return self.matrix
           return self.parent.get_world_matrix() * self.matrix

   # Create hierarchy
   root = Transform()
   root.matrix.translate(0.0, 1.0, 0.0)

   child = Transform()
   child.parent = root
   child.matrix.rotate(pyrove.vec3(0.0, 1.0, 0.0), 1.57)

   # Get final world transformation
   world_matrix = child.get_world_matrix()
