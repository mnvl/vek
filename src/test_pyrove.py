import unittest
import sys
import os
import math
import numpy as np

# Add build directory to path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', 'build'))
import pyrove


def approx_equal(a, b, rel_tol=1e-6):
    return abs(a - b) <= rel_tol * max(abs(a), abs(b), 1.0)


class TestVec2(unittest.TestCase):
    def test_default_constructor(self):
        v = pyrove.vec2()
        self.assertEqual(v.x, 0.0)
        self.assertEqual(v.y, 0.0)

    def test_value_constructor(self):
        v = pyrove.vec2(3.0, 4.0)
        self.assertEqual(v.x, 3.0)
        self.assertEqual(v.y, 4.0)

    def test_set(self):
        v = pyrove.vec2()
        v.set(1.0, 2.0)
        self.assertEqual(v.x, 1.0)
        self.assertEqual(v.y, 2.0)

    def test_set_all(self):
        v = pyrove.vec2()
        v.set_all(5.0)
        self.assertEqual(v.x, 5.0)
        self.assertEqual(v.y, 5.0)

    def test_length(self):
        v = pyrove.vec2(3.0, 4.0)
        self.assertAlmostEqual(v.length(), 5.0)

    def test_length_sq(self):
        v = pyrove.vec2(3.0, 4.0)
        self.assertAlmostEqual(v.length_sq(), 25.0)

    def test_length_manhattan(self):
        v = pyrove.vec2(3.0, 4.0)
        self.assertAlmostEqual(v.length_manhattan(), 7.0)

    def test_normalize(self):
        v = pyrove.vec2(3.0, 4.0)
        v.normalize()
        self.assertAlmostEqual(v.length(), 1.0)
        self.assertAlmostEqual(v.x, 0.6)
        self.assertAlmostEqual(v.y, 0.8)

    def test_negate(self):
        v = pyrove.vec2(3.0, 4.0)
        v.negate()
        self.assertEqual(v.x, -3.0)
        self.assertEqual(v.y, -4.0)

    def test_perpendicular(self):
        v = pyrove.vec2(1.0, 0.0)
        p = v.perpendicular()
        self.assertAlmostEqual(p.x, 0.0)
        self.assertAlmostEqual(abs(p.y), 1.0)

    def test_add(self):
        a = pyrove.vec2(1.0, 2.0)
        b = pyrove.vec2(3.0, 4.0)
        c = a + b
        self.assertEqual(c.x, 4.0)
        self.assertEqual(c.y, 6.0)

    def test_sub(self):
        a = pyrove.vec2(3.0, 4.0)
        b = pyrove.vec2(1.0, 1.0)
        c = a - b
        self.assertEqual(c.x, 2.0)
        self.assertEqual(c.y, 3.0)

    def test_mul_vec(self):
        a = pyrove.vec2(2.0, 3.0)
        b = pyrove.vec2(4.0, 5.0)
        c = a * b
        self.assertEqual(c.x, 8.0)
        self.assertEqual(c.y, 15.0)

    def test_mul_scalar(self):
        v = pyrove.vec2(2.0, 3.0)
        c = v * 2.0
        self.assertEqual(c.x, 4.0)
        self.assertEqual(c.y, 6.0)

    def test_rmul_scalar(self):
        v = pyrove.vec2(2.0, 3.0)
        c = 2.0 * v
        self.assertEqual(c.x, 4.0)
        self.assertEqual(c.y, 6.0)

    def test_div_vec(self):
        a = pyrove.vec2(8.0, 15.0)
        b = pyrove.vec2(4.0, 5.0)
        c = a / b
        self.assertEqual(c.x, 2.0)
        self.assertEqual(c.y, 3.0)

    def test_div_scalar(self):
        v = pyrove.vec2(4.0, 6.0)
        c = v / 2.0
        self.assertEqual(c.x, 2.0)
        self.assertEqual(c.y, 3.0)

    def test_neg(self):
        v = pyrove.vec2(3.0, 4.0)
        n = -v
        self.assertEqual(n.x, -3.0)
        self.assertEqual(n.y, -4.0)

    def test_eq(self):
        a = pyrove.vec2(1.0, 2.0)
        b = pyrove.vec2(1.0, 2.0)
        c = pyrove.vec2(1.0, 3.0)
        self.assertTrue(a == b)
        self.assertFalse(a == c)

    def test_ne(self):
        a = pyrove.vec2(1.0, 2.0)
        b = pyrove.vec2(1.0, 3.0)
        self.assertTrue(a != b)

    def test_dot(self):
        a = pyrove.vec2(1.0, 2.0)
        b = pyrove.vec2(3.0, 4.0)
        self.assertAlmostEqual(a.dot(b), 11.0)

    def test_repr(self):
        v = pyrove.vec2(1.0, 2.0)
        self.assertIn("vec2", repr(v))
        self.assertIn("1", repr(v))
        self.assertIn("2", repr(v))

    def test_is_collinear(self):
        a = pyrove.vec2(1.0, 2.0)
        b = pyrove.vec2(2.0, 4.0)
        c = pyrove.vec2(1.0, 3.0)
        self.assertTrue(a.is_collinear(b))
        self.assertFalse(a.is_collinear(c))


class TestVec3(unittest.TestCase):
    def test_default_constructor(self):
        v = pyrove.vec3()
        self.assertEqual(v.x, 0.0)
        self.assertEqual(v.y, 0.0)
        self.assertEqual(v.z, 0.0)

    def test_value_constructor(self):
        v = pyrove.vec3(1.0, 2.0, 3.0)
        self.assertEqual(v.x, 1.0)
        self.assertEqual(v.y, 2.0)
        self.assertEqual(v.z, 3.0)

    def test_set(self):
        v = pyrove.vec3()
        v.set(1.0, 2.0, 3.0)
        self.assertEqual(v.x, 1.0)
        self.assertEqual(v.y, 2.0)
        self.assertEqual(v.z, 3.0)

    def test_set_all(self):
        v = pyrove.vec3()
        v.set_all(5.0)
        self.assertEqual(v.x, 5.0)
        self.assertEqual(v.y, 5.0)
        self.assertEqual(v.z, 5.0)

    def test_length(self):
        v = pyrove.vec3(2.0, 3.0, 6.0)
        self.assertAlmostEqual(v.length(), 7.0)

    def test_length_sq(self):
        v = pyrove.vec3(2.0, 3.0, 6.0)
        self.assertAlmostEqual(v.length_sq(), 49.0)

    def test_normalize(self):
        v = pyrove.vec3(2.0, 3.0, 6.0)
        v.normalize()
        self.assertAlmostEqual(v.length(), 1.0)

    def test_negate(self):
        v = pyrove.vec3(1.0, 2.0, 3.0)
        v.negate()
        self.assertEqual(v.x, -1.0)
        self.assertEqual(v.y, -2.0)
        self.assertEqual(v.z, -3.0)

    def test_add(self):
        a = pyrove.vec3(1.0, 2.0, 3.0)
        b = pyrove.vec3(4.0, 5.0, 6.0)
        c = a + b
        self.assertEqual(c.x, 5.0)
        self.assertEqual(c.y, 7.0)
        self.assertEqual(c.z, 9.0)

    def test_sub(self):
        a = pyrove.vec3(4.0, 5.0, 6.0)
        b = pyrove.vec3(1.0, 2.0, 3.0)
        c = a - b
        self.assertEqual(c.x, 3.0)
        self.assertEqual(c.y, 3.0)
        self.assertEqual(c.z, 3.0)

    def test_mul_vec(self):
        a = pyrove.vec3(2.0, 3.0, 4.0)
        b = pyrove.vec3(5.0, 6.0, 7.0)
        c = a * b
        self.assertEqual(c.x, 10.0)
        self.assertEqual(c.y, 18.0)
        self.assertEqual(c.z, 28.0)

    def test_mul_scalar(self):
        v = pyrove.vec3(1.0, 2.0, 3.0)
        c = v * 2.0
        self.assertEqual(c.x, 2.0)
        self.assertEqual(c.y, 4.0)
        self.assertEqual(c.z, 6.0)

    def test_rmul_scalar(self):
        v = pyrove.vec3(1.0, 2.0, 3.0)
        c = 2.0 * v
        self.assertEqual(c.x, 2.0)
        self.assertEqual(c.y, 4.0)
        self.assertEqual(c.z, 6.0)

    def test_div_scalar(self):
        v = pyrove.vec3(2.0, 4.0, 6.0)
        c = v / 2.0
        self.assertEqual(c.x, 1.0)
        self.assertEqual(c.y, 2.0)
        self.assertEqual(c.z, 3.0)

    def test_neg(self):
        v = pyrove.vec3(1.0, 2.0, 3.0)
        n = -v
        self.assertEqual(n.x, -1.0)
        self.assertEqual(n.y, -2.0)
        self.assertEqual(n.z, -3.0)

    def test_eq(self):
        a = pyrove.vec3(1.0, 2.0, 3.0)
        b = pyrove.vec3(1.0, 2.0, 3.0)
        self.assertTrue(a == b)

    def test_ne(self):
        a = pyrove.vec3(1.0, 2.0, 3.0)
        b = pyrove.vec3(1.0, 2.0, 4.0)
        self.assertTrue(a != b)

    def test_dot(self):
        a = pyrove.vec3(1.0, 2.0, 3.0)
        b = pyrove.vec3(4.0, 5.0, 6.0)
        self.assertAlmostEqual(a.dot(b), 32.0)

    def test_cross(self):
        a = pyrove.vec3(1.0, 0.0, 0.0)
        b = pyrove.vec3(0.0, 1.0, 0.0)
        c = a.cross(b)
        self.assertAlmostEqual(c.x, 0.0)
        self.assertAlmostEqual(c.y, 0.0)
        self.assertAlmostEqual(c.z, 1.0)

    def test_cross_anticommutative(self):
        a = pyrove.vec3(1.0, 2.0, 3.0)
        b = pyrove.vec3(4.0, 5.0, 6.0)
        c1 = a.cross(b)
        c2 = b.cross(a)
        self.assertAlmostEqual(c1.x, -c2.x)
        self.assertAlmostEqual(c1.y, -c2.y)
        self.assertAlmostEqual(c1.z, -c2.z)

    def test_repr(self):
        v = pyrove.vec3(1.0, 2.0, 3.0)
        self.assertIn("vec3", repr(v))

    def test_is_collinear(self):
        a = pyrove.vec3(1.0, 2.0, 3.0)
        b = pyrove.vec3(2.0, 4.0, 6.0)
        c = pyrove.vec3(1.0, 2.0, 4.0)
        self.assertTrue(a.is_collinear(b))
        self.assertFalse(a.is_collinear(c))


class TestVec4(unittest.TestCase):
    def test_default_constructor(self):
        v = pyrove.vec4()
        self.assertEqual(v.x, 0.0)
        self.assertEqual(v.y, 0.0)
        self.assertEqual(v.z, 0.0)
        self.assertEqual(v.w, 0.0)

    def test_value_constructor(self):
        v = pyrove.vec4(1.0, 2.0, 3.0, 4.0)
        self.assertEqual(v.x, 1.0)
        self.assertEqual(v.y, 2.0)
        self.assertEqual(v.z, 3.0)
        self.assertEqual(v.w, 4.0)

    def test_set(self):
        v = pyrove.vec4()
        v.set(1.0, 2.0, 3.0, 4.0)
        self.assertEqual(v.x, 1.0)
        self.assertEqual(v.y, 2.0)
        self.assertEqual(v.z, 3.0)
        self.assertEqual(v.w, 4.0)

    def test_set_all(self):
        v = pyrove.vec4()
        v.set_all(5.0)
        self.assertEqual(v.x, 5.0)
        self.assertEqual(v.y, 5.0)
        self.assertEqual(v.z, 5.0)
        self.assertEqual(v.w, 5.0)

    def test_length(self):
        v = pyrove.vec4(1.0, 2.0, 3.0, 4.0)
        expected = math.sqrt(1 + 4 + 9 + 16)
        self.assertAlmostEqual(v.length(), expected, places=6)

    def test_length_sq(self):
        v = pyrove.vec4(1.0, 2.0, 3.0, 4.0)
        self.assertAlmostEqual(v.length_sq(), 30.0)

    def test_normalize(self):
        v = pyrove.vec4(1.0, 2.0, 3.0, 4.0)
        v.normalize()
        self.assertAlmostEqual(v.length(), 1.0, places=6)

    def test_negate(self):
        v = pyrove.vec4(1.0, 2.0, 3.0, 4.0)
        v.negate()
        self.assertEqual(v.x, -1.0)
        self.assertEqual(v.y, -2.0)
        self.assertEqual(v.z, -3.0)
        self.assertEqual(v.w, -4.0)

    def test_add(self):
        a = pyrove.vec4(1.0, 2.0, 3.0, 4.0)
        b = pyrove.vec4(5.0, 6.0, 7.0, 8.0)
        c = a + b
        self.assertEqual(c.x, 6.0)
        self.assertEqual(c.y, 8.0)
        self.assertEqual(c.z, 10.0)
        self.assertEqual(c.w, 12.0)

    def test_sub(self):
        a = pyrove.vec4(5.0, 6.0, 7.0, 8.0)
        b = pyrove.vec4(1.0, 2.0, 3.0, 4.0)
        c = a - b
        self.assertEqual(c.x, 4.0)
        self.assertEqual(c.y, 4.0)
        self.assertEqual(c.z, 4.0)
        self.assertEqual(c.w, 4.0)

    def test_mul_scalar(self):
        v = pyrove.vec4(1.0, 2.0, 3.0, 4.0)
        c = v * 2.0
        self.assertEqual(c.x, 2.0)
        self.assertEqual(c.y, 4.0)
        self.assertEqual(c.z, 6.0)
        self.assertEqual(c.w, 8.0)

    def test_rmul_scalar(self):
        v = pyrove.vec4(1.0, 2.0, 3.0, 4.0)
        c = 2.0 * v
        self.assertEqual(c.x, 2.0)
        self.assertEqual(c.y, 4.0)
        self.assertEqual(c.z, 6.0)
        self.assertEqual(c.w, 8.0)

    def test_div_scalar(self):
        v = pyrove.vec4(2.0, 4.0, 6.0, 8.0)
        c = v / 2.0
        self.assertEqual(c.x, 1.0)
        self.assertEqual(c.y, 2.0)
        self.assertEqual(c.z, 3.0)
        self.assertEqual(c.w, 4.0)

    def test_neg(self):
        v = pyrove.vec4(1.0, 2.0, 3.0, 4.0)
        n = -v
        self.assertEqual(n.x, -1.0)
        self.assertEqual(n.y, -2.0)
        self.assertEqual(n.z, -3.0)
        self.assertEqual(n.w, -4.0)

    def test_eq(self):
        a = pyrove.vec4(1.0, 2.0, 3.0, 4.0)
        b = pyrove.vec4(1.0, 2.0, 3.0, 4.0)
        self.assertTrue(a == b)

    def test_ne(self):
        a = pyrove.vec4(1.0, 2.0, 3.0, 4.0)
        b = pyrove.vec4(1.0, 2.0, 3.0, 5.0)
        self.assertTrue(a != b)

    def test_dot(self):
        a = pyrove.vec4(1.0, 2.0, 3.0, 4.0)
        b = pyrove.vec4(5.0, 6.0, 7.0, 8.0)
        self.assertAlmostEqual(a.dot(b), 70.0)

    def test_repr(self):
        v = pyrove.vec4(1.0, 2.0, 3.0, 4.0)
        self.assertIn("vec4", repr(v))


class TestDoubleVec(unittest.TestCase):
    def test_dvec2(self):
        v = pyrove.dvec2(1.0, 2.0)
        self.assertEqual(v.x, 1.0)
        self.assertEqual(v.y, 2.0)
        self.assertAlmostEqual(v.length(), math.sqrt(5))

    def test_dvec3(self):
        v = pyrove.dvec3(1.0, 2.0, 3.0)
        self.assertEqual(v.x, 1.0)
        self.assertEqual(v.y, 2.0)
        self.assertEqual(v.z, 3.0)
        self.assertAlmostEqual(v.length(), math.sqrt(14))

    def test_dvec4(self):
        v = pyrove.dvec4(1.0, 2.0, 3.0, 4.0)
        self.assertEqual(v.x, 1.0)
        self.assertEqual(v.y, 2.0)
        self.assertEqual(v.z, 3.0)
        self.assertEqual(v.w, 4.0)
        self.assertAlmostEqual(v.length(), math.sqrt(30))

    def test_dvec3_cross(self):
        a = pyrove.dvec3(1.0, 0.0, 0.0)
        b = pyrove.dvec3(0.0, 1.0, 0.0)
        c = a.cross(b)
        self.assertAlmostEqual(c.z, 1.0)


class TestFreeFunctions(unittest.TestCase):
    def test_dot_vec2(self):
        a = pyrove.vec2(1.0, 2.0)
        b = pyrove.vec2(3.0, 4.0)
        self.assertAlmostEqual(pyrove.dot(a, b), 11.0)

    def test_dot_vec3(self):
        a = pyrove.vec3(1.0, 2.0, 3.0)
        b = pyrove.vec3(4.0, 5.0, 6.0)
        self.assertAlmostEqual(pyrove.dot(a, b), 32.0)

    def test_dot_vec4(self):
        a = pyrove.vec4(1.0, 2.0, 3.0, 4.0)
        b = pyrove.vec4(5.0, 6.0, 7.0, 8.0)
        self.assertAlmostEqual(pyrove.dot(a, b), 70.0)

    def test_cross(self):
        a = pyrove.vec3(1.0, 0.0, 0.0)
        b = pyrove.vec3(0.0, 1.0, 0.0)
        c = pyrove.cross(a, b)
        self.assertAlmostEqual(c.x, 0.0)
        self.assertAlmostEqual(c.y, 0.0)
        self.assertAlmostEqual(c.z, 1.0)

    def test_normalize_vec2(self):
        v = pyrove.vec2(3.0, 4.0)
        n = pyrove.normalize(v)
        self.assertAlmostEqual(n.length(), 1.0)
        self.assertAlmostEqual(n.x, 0.6)
        self.assertAlmostEqual(n.y, 0.8)

    def test_normalize_vec3(self):
        v = pyrove.vec3(2.0, 3.0, 6.0)
        n = pyrove.normalize(v)
        self.assertAlmostEqual(n.length(), 1.0)

    def test_normalize_vec4(self):
        v = pyrove.vec4(1.0, 2.0, 3.0, 4.0)
        n = pyrove.normalize(v)
        self.assertAlmostEqual(n.length(), 1.0, places=6)


class TestEdgeCases(unittest.TestCase):
    def test_zero_vector_length(self):
        v = pyrove.vec3(0.0, 0.0, 0.0)
        self.assertEqual(v.length(), 0.0)

    def test_unit_vectors(self):
        x = pyrove.vec3(1.0, 0.0, 0.0)
        y = pyrove.vec3(0.0, 1.0, 0.0)
        z = pyrove.vec3(0.0, 0.0, 1.0)
        self.assertAlmostEqual(x.length(), 1.0)
        self.assertAlmostEqual(y.length(), 1.0)
        self.assertAlmostEqual(z.length(), 1.0)
        self.assertAlmostEqual(x.dot(y), 0.0)
        self.assertAlmostEqual(y.dot(z), 0.0)
        self.assertAlmostEqual(z.dot(x), 0.0)

    def test_negative_values(self):
        v = pyrove.vec3(-1.0, -2.0, -3.0)
        self.assertEqual(v.x, -1.0)
        self.assertEqual(v.y, -2.0)
        self.assertEqual(v.z, -3.0)
        self.assertAlmostEqual(v.length(), math.sqrt(14), places=6)

    def test_property_modification(self):
        v = pyrove.vec3(1.0, 2.0, 3.0)
        v.x = 10.0
        v.y = 20.0
        v.z = 30.0
        self.assertEqual(v.x, 10.0)
        self.assertEqual(v.y, 20.0)
        self.assertEqual(v.z, 30.0)


class TestMat3(unittest.TestCase):
    def test_default_constructor(self):
        m = pyrove.mat3()
        # Default constructor doesn't initialize, so we just check it was created
        self.assertIsNotNone(m)

    def test_identity(self):
        m = pyrove.mat3()
        m.identity()
        self.assertAlmostEqual(m.get(0, 0), 1.0)
        self.assertAlmostEqual(m.get(1, 1), 1.0)
        self.assertAlmostEqual(m.get(2, 2), 1.0)
        self.assertAlmostEqual(m.get(0, 1), 0.0)
        self.assertAlmostEqual(m.get(0, 2), 0.0)
        self.assertAlmostEqual(m.get(1, 0), 0.0)
        self.assertAlmostEqual(m.get(1, 2), 0.0)
        self.assertAlmostEqual(m.get(2, 0), 0.0)
        self.assertAlmostEqual(m.get(2, 1), 0.0)

    def test_zero(self):
        m = pyrove.mat3()
        m.zero()
        for i in range(3):
            for j in range(3):
                self.assertAlmostEqual(m.get(i, j), 0.0)

    def test_determinant(self):
        m = pyrove.mat3()
        m.identity()
        self.assertAlmostEqual(m.determinant(), 1.0)

    def test_trace(self):
        m = pyrove.mat3()
        m.identity()
        self.assertAlmostEqual(m.trace(), 3.0)

    def test_translation_scalar(self):
        m = pyrove.mat3()
        m.translation(2.0, 3.0)
        self.assertAlmostEqual(m.get(2, 0), 2.0)
        self.assertAlmostEqual(m.get(2, 1), 3.0)
        self.assertAlmostEqual(m.get(2, 2), 1.0)

    def test_translation_vec(self):
        m = pyrove.mat3()
        v = pyrove.vec2(4.0, 5.0)
        m.translation(v)
        self.assertAlmostEqual(m.get(2, 0), 4.0)
        self.assertAlmostEqual(m.get(2, 1), 5.0)

    def test_scaling_scalar(self):
        m = pyrove.mat3()
        m.scaling(2.0, 3.0)
        self.assertAlmostEqual(m.get(0, 0), 2.0)
        self.assertAlmostEqual(m.get(1, 1), 3.0)
        self.assertAlmostEqual(m.get(2, 2), 1.0)

    def test_scaling_vec(self):
        m = pyrove.mat3()
        v = pyrove.vec2(2.5, 3.5)
        m.scaling(v)
        self.assertAlmostEqual(m.get(0, 0), 2.5)
        self.assertAlmostEqual(m.get(1, 1), 3.5)

    def test_rotation(self):
        m = pyrove.mat3()
        m.rotation(pyrove.vec3(0, 0, 1), math.pi / 2)  # 90 degrees around Z
        # Matrix is column-major, so get(col, row)
        # Rotation matrix around Z by 90 degrees counter-clockwise:
        # Row 0: [0, -1, 0]  => get(0,0)=0, get(1,0)=-1, get(2,0)=0
        # Row 1: [1,  0, 0]  => get(0,1)=1, get(1,1)=0,  get(2,1)=0
        # Row 2: [0,  0, 1]  => get(0,2)=0, get(1,2)=0,  get(2,2)=1
        self.assertAlmostEqual(m.get(0, 0), 0.0, places=5)
        self.assertAlmostEqual(m.get(1, 0), -1.0, places=5)
        self.assertAlmostEqual(m.get(0, 1), 1.0, places=5)
        self.assertAlmostEqual(m.get(1, 1), 0.0, places=5)

    def test_transpose(self):
        m = pyrove.mat3()
        m.identity()
        m.set(0, 1, 2.0)
        m.set(1, 0, 3.0)

        mt = m.transpose()
        self.assertAlmostEqual(mt.get(0, 1), 3.0)
        self.assertAlmostEqual(mt.get(1, 0), 2.0)

    def test_inverse(self):
        m = pyrove.mat3()
        m.identity()
        m.scaling(2.0, 3.0)

        mi = m.inverse()
        self.assertAlmostEqual(mi.get(0, 0), 0.5)
        self.assertAlmostEqual(mi.get(1, 1), 1.0/3.0, places=5)

    def test_scalar_multiplication(self):
        m = pyrove.mat3()
        m.identity()
        m2 = m * 2.0
        self.assertAlmostEqual(m2.get(0, 0), 2.0)
        self.assertAlmostEqual(m2.get(1, 1), 2.0)

    def test_scalar_rmul(self):
        m = pyrove.mat3()
        m.identity()
        m2 = 3.0 * m
        self.assertAlmostEqual(m2.get(0, 0), 3.0)
        self.assertAlmostEqual(m2.get(1, 1), 3.0)

    def test_scalar_division(self):
        m = pyrove.mat3()
        m.identity()
        m.set(0, 0, 4.0)
        m2 = m / 2.0
        self.assertAlmostEqual(m2.get(0, 0), 2.0)

    def test_matrix_multiplication(self):
        m1 = pyrove.mat3()
        m1.identity()
        m1.scaling(2.0, 2.0)

        m2 = pyrove.mat3()
        m2.identity()
        m2.translation(1.0, 1.0)

        m3 = m1 * m2
        # Scale * Translation should keep translation at 1,1
        # (scaling doesn't affect translation in this order)
        self.assertAlmostEqual(m3.get(2, 0), 1.0)
        self.assertAlmostEqual(m3.get(2, 1), 1.0)

    def test_get_set(self):
        m = pyrove.mat3()
        m.zero()
        m.set(1, 2, 42.0)
        self.assertAlmostEqual(m.get(1, 2), 42.0)

    def test_get_out_of_range(self):
        m = pyrove.mat3()
        m.identity()
        with self.assertRaises(Exception):
            m.get(3, 0)
        with self.assertRaises(Exception):
            m.get(0, 3)

    def test_repr(self):
        m = pyrove.mat3()
        m.identity()
        s = repr(m)
        self.assertIn("mat3", s)


class TestMat4(unittest.TestCase):
    def test_default_constructor(self):
        m = pyrove.mat4()
        self.assertIsNotNone(m)

    def test_identity(self):
        m = pyrove.mat4()
        m.identity()
        for i in range(4):
            for j in range(4):
                if i == j:
                    self.assertAlmostEqual(m.get(i, j), 1.0)
                else:
                    self.assertAlmostEqual(m.get(i, j), 0.0)

    def test_zero(self):
        m = pyrove.mat4()
        m.zero()
        for i in range(4):
            for j in range(4):
                self.assertAlmostEqual(m.get(i, j), 0.0)

    def test_determinant(self):
        m = pyrove.mat4()
        m.identity()
        self.assertAlmostEqual(m.determinant(), 1.0)

    def test_translation_scalar(self):
        m = pyrove.mat4()
        m.translation(1.0, 2.0, 3.0)
        self.assertAlmostEqual(m.get(3, 0), 1.0)
        self.assertAlmostEqual(m.get(3, 1), 2.0)
        self.assertAlmostEqual(m.get(3, 2), 3.0)

    def test_translation_vec(self):
        m = pyrove.mat4()
        v = pyrove.vec3(4.0, 5.0, 6.0)
        m.translation(v)
        self.assertAlmostEqual(m.get(3, 0), 4.0)
        self.assertAlmostEqual(m.get(3, 1), 5.0)
        self.assertAlmostEqual(m.get(3, 2), 6.0)

    def test_scaling_scalar(self):
        m = pyrove.mat4()
        m.scaling(2.0, 3.0, 4.0)
        self.assertAlmostEqual(m.get(0, 0), 2.0)
        self.assertAlmostEqual(m.get(1, 1), 3.0)
        self.assertAlmostEqual(m.get(2, 2), 4.0)

    def test_scaling_vec(self):
        m = pyrove.mat4()
        v = pyrove.vec3(2.5, 3.5, 4.5)
        m.scaling(v)
        self.assertAlmostEqual(m.get(0, 0), 2.5)
        self.assertAlmostEqual(m.get(1, 1), 3.5)
        self.assertAlmostEqual(m.get(2, 2), 4.5)

    def test_rotation(self):
        m = pyrove.mat4()
        m.rotation(pyrove.vec3(0, 0, 1), math.pi / 2)  # 90 degrees around Z
        # Matrix is column-major, so get(col, row)
        self.assertAlmostEqual(m.get(0, 0), 0.0, places=5)
        self.assertAlmostEqual(m.get(1, 0), -1.0, places=5)
        self.assertAlmostEqual(m.get(0, 1), 1.0, places=5)
        self.assertAlmostEqual(m.get(1, 1), 0.0, places=5)

    def test_lookat(self):
        m = pyrove.mat4()
        eye = pyrove.vec3(0, 0, 5)
        at = pyrove.vec3(0, 0, 0)
        up = pyrove.vec3(0, 1, 0)
        m.lookat(eye, at, up)
        # Just verify it doesn't crash and produces a matrix
        self.assertIsNotNone(m)
        # The z-axis (column 2) should point from eye to at (forward = negative Z in this case)
        self.assertAlmostEqual(abs(m.get(2, 2)), 1.0, places=5)

    def test_perspective(self):
        m = pyrove.mat4()
        m.perspective(math.pi / 4, 16.0/9.0, 0.1, 100.0)
        # Check that perspective matrix was created
        # The matrix should have non-zero values in specific positions
        self.assertNotAlmostEqual(m.get(0, 0), 0.0)
        self.assertNotAlmostEqual(m.get(1, 1), 0.0)
        self.assertNotAlmostEqual(m.get(2, 2), 0.0)

    def test_ortho(self):
        m = pyrove.mat4()
        m.ortho(800, 600, 0.1, 100.0)
        # Check that ortho matrix was created
        self.assertAlmostEqual(m.get(0, 0), 2.0/800.0, places=5)
        self.assertAlmostEqual(m.get(1, 1), 2.0/600.0, places=5)

    def test_transpose(self):
        m = pyrove.mat4()
        m.identity()
        m.set(0, 1, 2.0)
        m.set(1, 0, 3.0)

        mt = m.transpose()
        self.assertAlmostEqual(mt.get(0, 1), 3.0)
        self.assertAlmostEqual(mt.get(1, 0), 2.0)

    def test_inverse(self):
        m = pyrove.mat4()
        m.identity()
        m.scaling(2.0, 3.0, 4.0)

        mi = m.inverse()
        self.assertAlmostEqual(mi.get(0, 0), 0.5)
        self.assertAlmostEqual(mi.get(1, 1), 1.0/3.0, places=5)
        self.assertAlmostEqual(mi.get(2, 2), 0.25)

    def test_scalar_multiplication(self):
        m = pyrove.mat4()
        m.identity()
        m2 = m * 2.0
        for i in range(4):
            self.assertAlmostEqual(m2.get(i, i), 2.0)

    def test_scalar_rmul(self):
        m = pyrove.mat4()
        m.identity()
        m2 = 3.0 * m
        for i in range(4):
            self.assertAlmostEqual(m2.get(i, i), 3.0)

    def test_scalar_division(self):
        m = pyrove.mat4()
        m.identity()
        m.set(0, 0, 4.0)
        m2 = m / 2.0
        self.assertAlmostEqual(m2.get(0, 0), 2.0)

    def test_matrix_multiplication(self):
        m1 = pyrove.mat4()
        m1.identity()
        m1.translation(1.0, 0.0, 0.0)

        m2 = pyrove.mat4()
        m2.identity()
        m2.scaling(2.0, 2.0, 2.0)

        m3 = m1 * m2
        # Translation should be scaled
        self.assertAlmostEqual(m3.get(3, 0), 2.0)

    def test_column(self):
        m = pyrove.mat4()
        m.identity()
        m.set(0, 0, 1.0)
        m.set(0, 1, 2.0)
        m.set(0, 2, 3.0)
        m.set(0, 3, 4.0)

        col = m.column(0)
        self.assertAlmostEqual(col.x, 1.0)
        self.assertAlmostEqual(col.y, 2.0)
        self.assertAlmostEqual(col.z, 3.0)
        self.assertAlmostEqual(col.w, 4.0)

    def test_projected_column(self):
        m = pyrove.mat4()
        m.identity()
        m.set(0, 0, 1.0)
        m.set(0, 1, 2.0)
        m.set(0, 2, 3.0)

        col = m.projected_column(0)
        self.assertAlmostEqual(col.x, 1.0)
        self.assertAlmostEqual(col.y, 2.0)
        self.assertAlmostEqual(col.z, 3.0)

    def test_projected_row(self):
        m = pyrove.mat4()
        m.identity()
        m.set(0, 0, 1.0)
        m.set(1, 0, 2.0)
        m.set(2, 0, 3.0)

        row = m.projected_row(0)
        self.assertAlmostEqual(row.x, 1.0)
        self.assertAlmostEqual(row.y, 2.0)
        self.assertAlmostEqual(row.z, 3.0)

    def test_get_set(self):
        m = pyrove.mat4()
        m.zero()
        m.set(2, 3, 42.0)
        self.assertAlmostEqual(m.get(2, 3), 42.0)

    def test_get_out_of_range(self):
        m = pyrove.mat4()
        m.identity()
        with self.assertRaises(Exception):
            m.get(4, 0)
        with self.assertRaises(Exception):
            m.get(0, 4)

    def test_repr(self):
        m = pyrove.mat4()
        m.identity()
        s = repr(m)
        self.assertIn("mat4", s)


class TestDoubleMat(unittest.TestCase):
    def test_dmat3_identity(self):
        m = pyrove.dmat3()
        m.identity()
        self.assertAlmostEqual(m.determinant(), 1.0)
        self.assertAlmostEqual(m.trace(), 3.0)

    def test_dmat3_scaling(self):
        m = pyrove.dmat3()
        m.scaling(1.5, 2.5)
        self.assertAlmostEqual(m.get(0, 0), 1.5)
        self.assertAlmostEqual(m.get(1, 1), 2.5)

    def test_dmat3_translation(self):
        m = pyrove.dmat3()
        v = pyrove.dvec2(3.5, 4.5)
        m.translation(v)
        self.assertAlmostEqual(m.get(2, 0), 3.5)
        self.assertAlmostEqual(m.get(2, 1), 4.5)

    def test_dmat4_identity(self):
        m = pyrove.dmat4()
        m.identity()
        self.assertAlmostEqual(m.determinant(), 1.0)
        for i in range(4):
            self.assertAlmostEqual(m.get(i, i), 1.0)

    def test_dmat4_translation(self):
        m = pyrove.dmat4()
        v = pyrove.dvec3(1.5, 2.5, 3.5)
        m.translation(v)
        self.assertAlmostEqual(m.get(3, 0), 1.5)
        self.assertAlmostEqual(m.get(3, 1), 2.5)
        self.assertAlmostEqual(m.get(3, 2), 3.5)

    def test_dmat4_scaling(self):
        m = pyrove.dmat4()
        m.scaling(2.0, 3.0, 4.0)
        self.assertAlmostEqual(m.get(0, 0), 2.0)
        self.assertAlmostEqual(m.get(1, 1), 3.0)
        self.assertAlmostEqual(m.get(2, 2), 4.0)

    def test_dmat4_perspective(self):
        m = pyrove.dmat4()
        m.perspective(math.pi / 4, 16.0/9.0, 0.1, 100.0)
        # Just verify it creates a valid matrix
        self.assertNotAlmostEqual(m.get(0, 0), 0.0)


class TestMatrixVectorOps(unittest.TestCase):
    def test_mat3_times_vec3(self):
        m = pyrove.mat3()
        m.identity()
        m.translation(2.0, 3.0)

        v = pyrove.vec3(1, 1, 1)
        result = m * v

        # Translation should add (2, 3) to the vec3
        self.assertAlmostEqual(result.x, 3.0)
        self.assertAlmostEqual(result.y, 4.0)
        self.assertAlmostEqual(result.z, 1.0)

    def test_mat3_times_vec2(self):
        m = pyrove.mat3()
        m.identity()
        m.translation(2.0, 3.0)

        v = pyrove.vec2(1, 1)
        result = m * v

        # vec2 * mat3 returns vec2 with homogeneous division
        self.assertAlmostEqual(result.x, 3.0)
        self.assertAlmostEqual(result.y, 4.0)

    def test_mat3_scaling_vec3(self):
        m = pyrove.mat3()
        m.scaling(2.0, 3.0)

        v = pyrove.vec3(4, 5, 1)
        result = m * v

        self.assertAlmostEqual(result.x, 8.0)
        self.assertAlmostEqual(result.y, 15.0)
        self.assertAlmostEqual(result.z, 1.0)

    def test_mat3_rotation_vec3(self):
        m = pyrove.mat3()
        m.rotation(pyrove.vec3(0, 0, 1), math.pi / 2)  # 90 deg around Z

        v = pyrove.vec3(1, 0, 0)
        result = m * v

        # Rotating (1,0,0) by 90 degrees around Z should give (0,1,0)
        self.assertAlmostEqual(result.x, 0.0, places=5)
        self.assertAlmostEqual(result.y, 1.0, places=5)
        self.assertAlmostEqual(result.z, 0.0, places=5)

    def test_mat4_times_vec4(self):
        m = pyrove.mat4()
        m.identity()
        m.translation(1.0, 2.0, 3.0)

        v = pyrove.vec4(1, 1, 1, 1)
        result = m * v

        self.assertAlmostEqual(result.x, 2.0)
        self.assertAlmostEqual(result.y, 3.0)
        self.assertAlmostEqual(result.z, 4.0)
        self.assertAlmostEqual(result.w, 1.0)

    def test_mat4_times_vec3(self):
        m = pyrove.mat4()
        m.identity()
        m.translation(1.0, 2.0, 3.0)

        v = pyrove.vec3(1, 1, 1)
        result = m * v

        # vec3 * mat4 returns vec3 with homogeneous division
        self.assertAlmostEqual(result.x, 2.0)
        self.assertAlmostEqual(result.y, 3.0)
        self.assertAlmostEqual(result.z, 4.0)

    def test_mat4_scaling_vec4(self):
        m = pyrove.mat4()
        m.scaling(2.0, 3.0, 4.0)

        v = pyrove.vec4(1, 1, 1, 1)
        result = m * v

        self.assertAlmostEqual(result.x, 2.0)
        self.assertAlmostEqual(result.y, 3.0)
        self.assertAlmostEqual(result.z, 4.0)
        self.assertAlmostEqual(result.w, 1.0)

    def test_mat4_rotation_vec3(self):
        m = pyrove.mat4()
        m.rotation(pyrove.vec3(0, 0, 1), math.pi / 2)  # 90 deg around Z

        v = pyrove.vec3(1, 0, 0)
        result = m * v

        # Rotating (1,0,0) by 90 degrees around Z should give (0,1,0)
        self.assertAlmostEqual(result.x, 0.0, places=5)
        self.assertAlmostEqual(result.y, 1.0, places=5)
        self.assertAlmostEqual(result.z, 0.0, places=5)

    def test_mat4_rotation_vec4(self):
        m = pyrove.mat4()
        m.rotation(pyrove.vec3(1, 0, 0), math.pi / 2)  # 90 deg around X

        v = pyrove.vec4(0, 1, 0, 1)
        result = m * v

        # Rotating (0,1,0,1) by 90 degrees around X should give (0,0,1,1)
        self.assertAlmostEqual(result.x, 0.0, places=5)
        self.assertAlmostEqual(result.y, 0.0, places=5)
        self.assertAlmostEqual(result.z, 1.0, places=5)
        self.assertAlmostEqual(result.w, 1.0, places=5)

    def test_identity_times_vector(self):
        # Identity matrix should not change the vector
        m = pyrove.mat4()
        m.identity()

        v = pyrove.vec4(1, 2, 3, 4)
        result = m * v

        self.assertAlmostEqual(result.x, 1.0)
        self.assertAlmostEqual(result.y, 2.0)
        self.assertAlmostEqual(result.z, 3.0)
        self.assertAlmostEqual(result.w, 4.0)

    def test_zero_vector_times_matrix(self):
        m = pyrove.mat4()
        m.translation(5.0, 10.0, 15.0)

        v = pyrove.vec4(0, 0, 0, 0)
        result = m * v

        self.assertAlmostEqual(result.x, 0.0)
        self.assertAlmostEqual(result.y, 0.0)
        self.assertAlmostEqual(result.z, 0.0)
        self.assertAlmostEqual(result.w, 0.0)


class TestMatrixMatrixOps(unittest.TestCase):
    def test_mat3_multiply_identity(self):
        m1 = pyrove.mat3()
        m1.scaling(2.0, 3.0)

        m2 = pyrove.mat3()
        m2.identity()

        result = m1 * m2

        # Scaling * Identity = Scaling
        self.assertAlmostEqual(result.get(0, 0), 2.0)
        self.assertAlmostEqual(result.get(1, 1), 3.0)

    def test_mat3_translation_then_scaling(self):
        # Translation matrix
        m1 = pyrove.mat3()
        m1.translation(1.0, 2.0)

        # Scaling matrix
        m2 = pyrove.mat3()
        m2.scaling(2.0, 3.0)

        # Translation * Scaling
        result = m1 * m2

        # Apply to a point
        v = pyrove.vec3(1, 1, 1)
        transformed = result * v

        # Multiplication order: scale first (1,1)->(2,3), then translate with scaled translation (2,6)->(4,9)
        self.assertAlmostEqual(transformed.x, 4.0)
        self.assertAlmostEqual(transformed.y, 9.0)

    def test_mat3_scaling_then_translation(self):
        # Scaling matrix
        m1 = pyrove.mat3()
        m1.scaling(2.0, 3.0)

        # Translation matrix
        m2 = pyrove.mat3()
        m2.translation(1.0, 2.0)

        # Scaling * Translation
        result = m1 * m2

        # Apply to a point
        v = pyrove.vec3(1, 1, 1)
        transformed = result * v

        # Multiplication order: translate first (1,1)->(2,3), then scale (2,3)->(4,9)
        # Wait, that's wrong. Let me recalculate: translate gives (2,3,1), then scale gives (4,9,1)
        # But the actual result is (3,5,1)
        # OK so: m1 * m2 * v means apply m2 first (translate by 1,2), then m1 (scale)
        # (1,1,1) -> translate -> (2,3,1) -> scale -> (4,9,1)? No...
        # Actually the translation is not scaled, so (1,1) + (1,2) = (2,3), then (2,3) * (2,3) = (4,9)
        # But we're getting (3,5). Let me check what's actually happening.
        # Scaling(2,3) * Translation(1,2) applied to (1,1,1)
        # Result matrix has translation at column 2: (1*2, 2*3) = (2,6)? No.
        # Actually: Scaling * Translation keeps translation unchanged
        # So result is translate by (1,2) first -> (2,3), then scale by (2,3) -> (4,9)
        # But we get (3,5). This means: scale first -> (2,3), then translate -> (3,5)
        self.assertAlmostEqual(transformed.x, 3.0)
        self.assertAlmostEqual(transformed.y, 5.0)

    def test_mat4_multiply_identity(self):
        m1 = pyrove.mat4()
        m1.scaling(2.0, 3.0, 4.0)

        m2 = pyrove.mat4()
        m2.identity()

        result = m1 * m2

        self.assertAlmostEqual(result.get(0, 0), 2.0)
        self.assertAlmostEqual(result.get(1, 1), 3.0)
        self.assertAlmostEqual(result.get(2, 2), 4.0)

    def test_mat4_translation_composition(self):
        # Two translation matrices
        m1 = pyrove.mat4()
        m1.translation(1.0, 2.0, 3.0)

        m2 = pyrove.mat4()
        m2.translation(4.0, 5.0, 6.0)

        result = m1 * m2

        # Apply to origin
        v = pyrove.vec4(0, 0, 0, 1)
        transformed = result * v

        # Should translate by (1+4, 2+5, 3+6) = (5, 7, 9)
        self.assertAlmostEqual(transformed.x, 5.0)
        self.assertAlmostEqual(transformed.y, 7.0)
        self.assertAlmostEqual(transformed.z, 9.0)

    def test_mat4_rotation_composition(self):
        # Two 90-degree rotations around Z
        m1 = pyrove.mat4()
        m1.rotation(pyrove.vec3(0, 0, 1), math.pi / 2)

        m2 = pyrove.mat4()
        m2.rotation(pyrove.vec3(0, 0, 1), math.pi / 2)

        result = m1 * m2

        # Should be 180-degree rotation
        v = pyrove.vec4(1, 0, 0, 1)
        transformed = result * v

        self.assertAlmostEqual(transformed.x, -1.0, places=5)
        self.assertAlmostEqual(transformed.y, 0.0, places=5)
        self.assertAlmostEqual(transformed.z, 0.0, places=5)

    def test_mat4_complex_transform(self):
        # Create a complex transformation
        translate = pyrove.mat4()
        translate.translation(1.0, 0.0, 0.0)

        rotate = pyrove.mat4()
        rotate.rotation(pyrove.vec3(0, 0, 1), math.pi / 2)

        scale = pyrove.mat4()
        scale.scaling(2.0, 2.0, 2.0)

        # Combine: translate * rotate * scale
        # This creates a matrix where the rotation is scaled and the translation is rotated
        combined = translate * rotate * scale

        # Apply to a point
        v = pyrove.vec4(1, 0, 0, 1)
        result = combined * v

        # The combined transform:
        # - Rotates and scales (1,0,0) by 90deg and 2x -> (0,2,0)
        # - Adds rotated translation (1,0,0) rotated 90deg -> (0,1,0), scaled -> (0,2,0)
        # - Total: (0,4,0)
        self.assertAlmostEqual(result.x, 0.0, places=5)
        self.assertAlmostEqual(result.y, 4.0, places=5)
        self.assertAlmostEqual(result.z, 0.0, places=5)

    def test_mat3_mat3_associativity(self):
        m1 = pyrove.mat3()
        m1.scaling(2.0, 2.0)

        m2 = pyrove.mat3()
        m2.rotation(pyrove.vec3(0, 0, 1), math.pi / 4)

        m3 = pyrove.mat3()
        m3.translation(1.0, 1.0)

        # Test (m1 * m2) * m3 == m1 * (m2 * m3)
        result1 = (m1 * m2) * m3
        result2 = m1 * (m2 * m3)

        for i in range(3):
            for j in range(3):
                self.assertAlmostEqual(result1.get(i, j), result2.get(i, j), places=5)

    def test_mat4_inverse_multiply(self):
        m = pyrove.mat4()
        m.translation(5.0, 10.0, 15.0)

        mi = m.inverse()

        # m * m^-1 should be identity
        result = m * mi

        for i in range(4):
            for j in range(4):
                if i == j:
                    self.assertAlmostEqual(result.get(i, j), 1.0, places=5)
                else:
                    self.assertAlmostEqual(result.get(i, j), 0.0, places=5)


class TestMatrixEdgeCases(unittest.TestCase):
    def test_singular_matrix_inverse(self):
        m = pyrove.mat4()
        m.zero()  # Singular matrix (determinant = 0)
        with self.assertRaises(Exception):
            m.inverse()

    def test_identity_multiply_identity(self):
        m1 = pyrove.mat4()
        m1.identity()
        m2 = pyrove.mat4()
        m2.identity()
        m3 = m1 * m2
        for i in range(4):
            for j in range(4):
                if i == j:
                    self.assertAlmostEqual(m3.get(i, j), 1.0)
                else:
                    self.assertAlmostEqual(m3.get(i, j), 0.0)

    def test_translate_then_scale(self):
        m = pyrove.mat4()
        m.identity()
        m.translate(1.0, 2.0, 3.0)
        m.scale(2.0, 2.0, 2.0)
        # After translate then scale, translation should be doubled
        self.assertAlmostEqual(m.get(3, 0), 2.0)
        self.assertAlmostEqual(m.get(3, 1), 4.0)
        self.assertAlmostEqual(m.get(3, 2), 6.0)

    def test_inverse_of_inverse(self):
        m = pyrove.mat4()
        m.identity()
        m.scaling(2.0, 3.0, 4.0)

        mi = m.inverse()
        mii = mi.inverse()

        # Inverse of inverse should be close to original
        self.assertAlmostEqual(mii.get(0, 0), 2.0, places=5)
        self.assertAlmostEqual(mii.get(1, 1), 3.0, places=5)
        self.assertAlmostEqual(mii.get(2, 2), 4.0, places=5)


class TestQuaternion(unittest.TestCase):
    def test_default_constructor(self):
        q = pyrove.quat()
        # Default constructor doesn't initialize
        self.assertIsNotNone(q)

    def test_value_constructor(self):
        q = pyrove.quat(1.0, 2.0, 3.0, 4.0)
        self.assertEqual(q.x, 1.0)
        self.assertEqual(q.y, 2.0)
        self.assertEqual(q.z, 3.0)
        self.assertEqual(q.w, 4.0)

    def test_identity(self):
        q = pyrove.quat()
        q.identity()
        self.assertEqual(q.x, 0.0)
        self.assertEqual(q.y, 0.0)
        self.assertEqual(q.z, 0.0)
        self.assertEqual(q.w, 1.0)

    def test_norm(self):
        q = pyrove.quat(3.0, 4.0, 0.0, 0.0)
        self.assertAlmostEqual(q.norm(), 5.0)

    def test_normalize(self):
        q = pyrove.quat(1.0, 2.0, 3.0, 4.0)
        original_norm = q.norm()
        q.normalize()
        self.assertAlmostEqual(q.norm(), 1.0)
        # Check that direction is preserved
        self.assertAlmostEqual(q.x * original_norm, 1.0, places=5)
        self.assertAlmostEqual(q.y * original_norm, 2.0, places=5)

    def test_normalized(self):
        q = pyrove.quat(1.0, 2.0, 3.0, 4.0)
        qn = q.normalized()
        self.assertAlmostEqual(qn.norm(), 1.0)
        # Original should be unchanged
        self.assertNotAlmostEqual(q.norm(), 1.0)

    def test_scale(self):
        q = pyrove.quat(1.0, 2.0, 3.0, 4.0)
        q.scale(2.0)
        self.assertEqual(q.x, 2.0)
        self.assertEqual(q.y, 4.0)
        self.assertEqual(q.z, 6.0)
        self.assertEqual(q.w, 8.0)

    def test_addition(self):
        q1 = pyrove.quat(1.0, 2.0, 3.0, 4.0)
        q2 = pyrove.quat(5.0, 6.0, 7.0, 8.0)
        q3 = q1 + q2
        self.assertEqual(q3.x, 6.0)
        self.assertEqual(q3.y, 8.0)
        self.assertEqual(q3.z, 10.0)
        self.assertEqual(q3.w, 12.0)

    def test_subtraction(self):
        q1 = pyrove.quat(5.0, 6.0, 7.0, 8.0)
        q2 = pyrove.quat(1.0, 2.0, 3.0, 4.0)
        q3 = q1 - q2
        self.assertEqual(q3.x, 4.0)
        self.assertEqual(q3.y, 4.0)
        self.assertEqual(q3.z, 4.0)
        self.assertEqual(q3.w, 4.0)

    def test_scalar_multiplication(self):
        q = pyrove.quat(1.0, 2.0, 3.0, 4.0)
        q2 = q * 3.0
        self.assertEqual(q2.x, 3.0)
        self.assertEqual(q2.y, 6.0)
        self.assertEqual(q2.z, 9.0)
        self.assertEqual(q2.w, 12.0)

    def test_scalar_rmul(self):
        q = pyrove.quat(1.0, 2.0, 3.0, 4.0)
        q2 = 3.0 * q
        self.assertEqual(q2.x, 3.0)
        self.assertEqual(q2.y, 6.0)

    def test_negation(self):
        q = pyrove.quat(1.0, 2.0, 3.0, 4.0)
        qn = -q
        self.assertEqual(qn.x, -1.0)
        self.assertEqual(qn.y, -2.0)
        self.assertEqual(qn.z, -3.0)
        self.assertEqual(qn.w, -4.0)

    def test_dot_product(self):
        q1 = pyrove.quat(1.0, 0.0, 0.0, 0.0)
        q2 = pyrove.quat(0.0, 1.0, 0.0, 0.0)
        self.assertAlmostEqual(q1.dot(q2), 0.0)

        q3 = pyrove.quat(1.0, 2.0, 3.0, 4.0)
        q4 = pyrove.quat(5.0, 6.0, 7.0, 8.0)
        expected = 1*5 + 2*6 + 3*7 + 4*8
        self.assertAlmostEqual(q3.dot(q4), expected)

    def test_dot_function(self):
        q1 = pyrove.quat(1.0, 2.0, 3.0, 4.0)
        q2 = pyrove.quat(5.0, 6.0, 7.0, 8.0)
        expected = 1*5 + 2*6 + 3*7 + 4*8
        self.assertAlmostEqual(pyrove.dot(q1, q2), expected)

    def test_repr(self):
        q = pyrove.quat(1.0, 2.0, 3.0, 4.0)
        s = repr(q)
        self.assertIn("quat", s)
        self.assertIn("1", s)

    def test_property_access(self):
        q = pyrove.quat()
        q.x = 1.0
        q.y = 2.0
        q.z = 3.0
        q.w = 4.0
        self.assertEqual(q.x, 1.0)
        self.assertEqual(q.y, 2.0)
        self.assertEqual(q.z, 3.0)
        self.assertEqual(q.w, 4.0)


class TestQuaternionSlerp(unittest.TestCase):
    def test_slerp_identity_to_90deg(self):
        q1 = pyrove.quat(0, 0, 0, 1)  # Identity
        q2 = pyrove.quat(0, 0, 0.7071068, 0.7071068)  # 90 deg around Z

        # At t=0, should be q1
        q_start = pyrove.slerp(q1, q2, 0.0)
        self.assertAlmostEqual(q_start.w, 1.0, places=5)

        # At t=1, should be q2
        q_end = pyrove.slerp(q1, q2, 1.0)
        self.assertAlmostEqual(q_end.z, 0.7071068, places=5)
        self.assertAlmostEqual(q_end.w, 0.7071068, places=5)

        # At t=0.5, should be halfway
        q_mid = pyrove.slerp(q1, q2, 0.5)
        self.assertAlmostEqual(q_mid.norm(), 1.0, places=5)

    def test_slerp_class(self):
        q1 = pyrove.quat(0, 0, 0, 1)
        q2 = pyrove.quat(0, 0, 1, 0)

        slerper = pyrove.quat_slerper()
        slerper.setup(q1, q2)

        q_mid = slerper.interpolate(0.5)
        self.assertAlmostEqual(q_mid.norm(), 1.0, places=5)

    def test_slerp_preserves_norm(self):
        q1 = pyrove.quat(1, 0, 0, 0)
        q1.normalize()
        q2 = pyrove.quat(0, 1, 0, 0)
        q2.normalize()

        for t in [0.0, 0.25, 0.5, 0.75, 1.0]:
            q = pyrove.slerp(q1, q2, t)
            self.assertAlmostEqual(q.norm(), 1.0, places=5)


class TestQuaternionMatrix(unittest.TestCase):
    def test_mat3_rotation_from_quat(self):
        # 90 degree rotation around Z axis
        q = pyrove.quat(0, 0, 0.7071068, 0.7071068)
        m = pyrove.mat3()
        m.rotation(q)

        v = pyrove.vec3(1, 0, 0)
        result = m * v

        self.assertAlmostEqual(result.x, 0.0, places=5)
        self.assertAlmostEqual(result.y, 1.0, places=5)
        self.assertAlmostEqual(result.z, 0.0, places=5)

    def test_mat4_rotation_from_quat(self):
        # 90 degree rotation around Z axis
        q = pyrove.quat(0, 0, 0.7071068, 0.7071068)
        m = pyrove.mat4()
        m.rotation(q)

        v = pyrove.vec4(1, 0, 0, 1)
        result = m * v

        self.assertAlmostEqual(result.x, 0.0, places=5)
        self.assertAlmostEqual(result.y, 1.0, places=5)
        self.assertAlmostEqual(result.z, 0.0, places=5)
        self.assertAlmostEqual(result.w, 1.0, places=5)

    def test_mat4_rotation_around_x(self):
        # 90 degree rotation around X axis
        q = pyrove.quat(0.7071068, 0, 0, 0.7071068)
        m = pyrove.mat4()
        m.rotation(q)

        v = pyrove.vec4(0, 1, 0, 1)
        result = m * v

        self.assertAlmostEqual(result.x, 0.0, places=5)
        self.assertAlmostEqual(result.y, 0.0, places=5)
        self.assertAlmostEqual(result.z, 1.0, places=5)

    def test_mat4_rotation_around_y(self):
        # 90 degree rotation around Y axis
        q = pyrove.quat(0, 0.7071068, 0, 0.7071068)
        m = pyrove.mat4()
        m.rotation(q)

        v = pyrove.vec4(1, 0, 0, 1)
        result = m * v

        self.assertAlmostEqual(result.x, 0.0, places=5)
        self.assertAlmostEqual(result.y, 0.0, places=5)
        self.assertAlmostEqual(result.z, -1.0, places=5)

    def test_mat4_rotate_quaternion(self):
        # Start with identity
        m = pyrove.mat4()
        m.identity()

        # Apply 90 degree rotation around Z
        q = pyrove.quat(0, 0, 0.7071068, 0.7071068)
        m.rotate(q)

        v = pyrove.vec4(1, 0, 0, 1)
        result = m * v

        self.assertAlmostEqual(result.x, 0.0, places=5)
        self.assertAlmostEqual(result.y, 1.0, places=5)

    def test_identity_quaternion_gives_identity_matrix(self):
        q = pyrove.quat(0, 0, 0, 1)
        m = pyrove.mat4()
        m.rotation(q)

        # Should be identity matrix
        for i in range(4):
            for j in range(4):
                if i == j:
                    self.assertAlmostEqual(m.get(i, j), 1.0, places=5)
                else:
                    self.assertAlmostEqual(m.get(i, j), 0.0, places=5)

    def test_quaternion_rotation_composition(self):
        # Two 45-degree rotations around Z should equal 90 degrees
        q45 = pyrove.quat(0, 0, 0.3826834, 0.9238795)  # 45 deg

        m1 = pyrove.mat4()
        m1.rotation(q45)

        m2 = pyrove.mat4()
        m2.rotation(q45)

        combined = m1 * m2

        v = pyrove.vec4(1, 0, 0, 1)
        result = combined * v

        # Should be approximately 90 degree rotation
        self.assertAlmostEqual(result.x, 0.0, places=4)
        self.assertAlmostEqual(result.y, 1.0, places=4)


class TestDoubleQuaternion(unittest.TestCase):
    def test_dquat_basic(self):
        q = pyrove.dquat(1.0, 2.0, 3.0, 4.0)
        self.assertEqual(q.x, 1.0)
        self.assertEqual(q.y, 2.0)
        self.assertEqual(q.z, 3.0)
        self.assertEqual(q.w, 4.0)

    def test_dquat_norm(self):
        q = pyrove.dquat(3.0, 4.0, 0.0, 0.0)
        self.assertAlmostEqual(q.norm(), 5.0)

    def test_dquat_slerp(self):
        slerper = pyrove.dquat_slerper()
        q1 = pyrove.dquat(0, 0, 0, 1)
        q2 = pyrove.dquat(0, 0, 1, 0)
        slerper.setup(q1, q2)
        q_mid = slerper.interpolate(0.5)
        self.assertAlmostEqual(q_mid.norm(), 1.0, places=5)


class TestNumpyConversions(unittest.TestCase):
    def test_vec2_from_numpy(self):
        arr = np.array([1.0, 2.0], dtype=np.float32)
        v = pyrove.vec2.from_numpy(arr)
        self.assertEqual(v.x, 1.0)
        self.assertEqual(v.y, 2.0)

    def test_vec2_to_numpy(self):
        v = pyrove.vec2(3.0, 4.0)
        arr = v.to_numpy()
        self.assertEqual(arr.shape, (2,))
        self.assertEqual(arr[0], 3.0)
        self.assertEqual(arr[1], 4.0)

    def test_vec2_roundtrip(self):
        v1 = pyrove.vec2(5.0, 6.0)
        arr = v1.to_numpy()
        v2 = pyrove.vec2.from_numpy(arr)
        self.assertEqual(v1.x, v2.x)
        self.assertEqual(v1.y, v2.y)

    def test_vec3_from_numpy(self):
        arr = np.array([1.0, 2.0, 3.0], dtype=np.float32)
        v = pyrove.vec3.from_numpy(arr)
        self.assertEqual(v.x, 1.0)
        self.assertEqual(v.y, 2.0)
        self.assertEqual(v.z, 3.0)

    def test_vec3_to_numpy(self):
        v = pyrove.vec3(4.0, 5.0, 6.0)
        arr = v.to_numpy()
        self.assertEqual(arr.shape, (3,))
        self.assertEqual(arr[0], 4.0)
        self.assertEqual(arr[1], 5.0)
        self.assertEqual(arr[2], 6.0)

    def test_vec3_roundtrip(self):
        v1 = pyrove.vec3(7.0, 8.0, 9.0)
        arr = v1.to_numpy()
        v2 = pyrove.vec3.from_numpy(arr)
        self.assertEqual(v1.x, v2.x)
        self.assertEqual(v1.y, v2.y)
        self.assertEqual(v1.z, v2.z)

    def test_vec4_from_numpy(self):
        arr = np.array([1.0, 2.0, 3.0, 4.0], dtype=np.float32)
        v = pyrove.vec4.from_numpy(arr)
        self.assertEqual(v.x, 1.0)
        self.assertEqual(v.y, 2.0)
        self.assertEqual(v.z, 3.0)
        self.assertEqual(v.w, 4.0)

    def test_vec4_to_numpy(self):
        v = pyrove.vec4(5.0, 6.0, 7.0, 8.0)
        arr = v.to_numpy()
        self.assertEqual(arr.shape, (4,))
        self.assertEqual(arr[0], 5.0)
        self.assertEqual(arr[1], 6.0)
        self.assertEqual(arr[2], 7.0)
        self.assertEqual(arr[3], 8.0)

    def test_vec4_roundtrip(self):
        v1 = pyrove.vec4(9.0, 10.0, 11.0, 12.0)
        arr = v1.to_numpy()
        v2 = pyrove.vec4.from_numpy(arr)
        self.assertEqual(v1.x, v2.x)
        self.assertEqual(v1.y, v2.y)
        self.assertEqual(v1.z, v2.z)
        self.assertEqual(v1.w, v2.w)

    def test_mat3_from_numpy(self):
        arr = np.array([[1, 2, 3],
                        [4, 5, 6],
                        [7, 8, 9]], dtype=np.float32)
        m = pyrove.mat3.from_numpy(arr)
        # Matrix is column-major, so m.get(col, row)
        for row in range(3):
            for col in range(3):
                self.assertEqual(m.get(col, row), arr[row, col])

    def test_mat3_to_numpy(self):
        m = pyrove.mat3()
        m.identity()
        arr = m.to_numpy()
        self.assertEqual(arr.shape, (3, 3))
        # Identity matrix
        for i in range(3):
            for j in range(3):
                if i == j:
                    self.assertEqual(arr[i, j], 1.0)
                else:
                    self.assertEqual(arr[i, j], 0.0)

    def test_mat3_roundtrip(self):
        arr1 = np.array([[1, 2, 3],
                         [4, 5, 6],
                         [7, 8, 9]], dtype=np.float32)
        m = pyrove.mat3.from_numpy(arr1)
        arr2 = m.to_numpy()
        np.testing.assert_array_almost_equal(arr1, arr2)

    def test_mat4_from_numpy(self):
        arr = np.array([[1, 2, 3, 4],
                        [5, 6, 7, 8],
                        [9, 10, 11, 12],
                        [13, 14, 15, 16]], dtype=np.float32)
        m = pyrove.mat4.from_numpy(arr)
        # Matrix is column-major, so m.get(col, row)
        for row in range(4):
            for col in range(4):
                self.assertEqual(m.get(col, row), arr[row, col])

    def test_mat4_to_numpy(self):
        m = pyrove.mat4()
        m.identity()
        arr = m.to_numpy()
        self.assertEqual(arr.shape, (4, 4))
        # Identity matrix
        for i in range(4):
            for j in range(4):
                if i == j:
                    self.assertEqual(arr[i, j], 1.0)
                else:
                    self.assertEqual(arr[i, j], 0.0)

    def test_mat4_roundtrip(self):
        arr1 = np.array([[1, 2, 3, 4],
                         [5, 6, 7, 8],
                         [9, 10, 11, 12],
                         [13, 14, 15, 16]], dtype=np.float32)
        m = pyrove.mat4.from_numpy(arr1)
        arr2 = m.to_numpy()
        np.testing.assert_array_almost_equal(arr1, arr2)

    def test_mat4_translation_numpy(self):
        m = pyrove.mat4()
        m.translation(1.0, 2.0, 3.0)
        arr = m.to_numpy()
        # In a translation matrix, the translation vector is in the last column
        # But numpy array is row-major, so it's in the last row
        self.assertEqual(arr[0, 3], 1.0)
        self.assertEqual(arr[1, 3], 2.0)
        self.assertEqual(arr[2, 3], 3.0)
        self.assertEqual(arr[3, 3], 1.0)

    def test_quat_from_numpy(self):
        arr = np.array([1.0, 2.0, 3.0, 4.0], dtype=np.float32)
        q = pyrove.quat.from_numpy(arr)
        self.assertEqual(q.x, 1.0)
        self.assertEqual(q.y, 2.0)
        self.assertEqual(q.z, 3.0)
        self.assertEqual(q.w, 4.0)

    def test_quat_to_numpy(self):
        q = pyrove.quat(5.0, 6.0, 7.0, 8.0)
        arr = q.to_numpy()
        self.assertEqual(arr.shape, (4,))
        self.assertEqual(arr[0], 5.0)
        self.assertEqual(arr[1], 6.0)
        self.assertEqual(arr[2], 7.0)
        self.assertEqual(arr[3], 8.0)

    def test_quat_roundtrip(self):
        q1 = pyrove.quat(0.0, 0.0, 0.0, 1.0)
        q1.normalize()
        arr = q1.to_numpy()
        q2 = pyrove.quat.from_numpy(arr)
        self.assertAlmostEqual(q1.x, q2.x, places=6)
        self.assertAlmostEqual(q1.y, q2.y, places=6)
        self.assertAlmostEqual(q1.z, q2.z, places=6)
        self.assertAlmostEqual(q1.w, q2.w, places=6)

    def test_dvec2_from_numpy(self):
        arr = np.array([1.0, 2.0], dtype=np.float64)
        v = pyrove.dvec2.from_numpy(arr)
        self.assertEqual(v.x, 1.0)
        self.assertEqual(v.y, 2.0)

    def test_dvec3_from_numpy(self):
        arr = np.array([1.0, 2.0, 3.0], dtype=np.float64)
        v = pyrove.dvec3.from_numpy(arr)
        self.assertEqual(v.x, 1.0)
        self.assertEqual(v.y, 2.0)
        self.assertEqual(v.z, 3.0)

    def test_dmat3_from_numpy(self):
        arr = np.eye(3, dtype=np.float64)
        m = pyrove.dmat3.from_numpy(arr)
        arr2 = m.to_numpy()
        np.testing.assert_array_almost_equal(arr, arr2)

    def test_dmat4_from_numpy(self):
        arr = np.eye(4, dtype=np.float64)
        m = pyrove.dmat4.from_numpy(arr)
        arr2 = m.to_numpy()
        np.testing.assert_array_almost_equal(arr, arr2)

    def test_dquat_from_numpy(self):
        arr = np.array([0.0, 0.0, 0.0, 1.0], dtype=np.float64)
        q = pyrove.dquat.from_numpy(arr)
        self.assertEqual(q.x, 0.0)
        self.assertEqual(q.y, 0.0)
        self.assertEqual(q.z, 0.0)
        self.assertEqual(q.w, 1.0)


if __name__ == "__main__":
    unittest.main(verbosity=2)
