#!/usr/bin/env python3
"""
Performance benchmark comparing pyrove with NumPy.

This script benchmarks common vector and matrix operations to compare
the performance of pyrove (C++ with Python bindings) against NumPy.

Usage:
    python3 benchmark.py
    ./run_benchmark.sh

Interpreting Results:
    - µs (microseconds) = time per operation (lower is better)
    - Speedup > 1.0 means pyrove is faster
    - Speedup < 1.0 means NumPy is faster

Performance Tips:
    - Use pyrove for tight loops with 2D/3D/4D vectors and 3x3/4x4 matrices
    - Use pyrove for geometric operations (rays, planes, triangles)
    - Use NumPy for large arrays and vectorized batch operations
    - Minimize conversions between pyrove and NumPy (has overhead)
    - pyrove excels at graphics/robotics computations with small fixed-size data
    - NumPy excels at large-scale numerical/scientific computing
"""

import time
import sys
import os
import numpy as np

# Add build directory to path to find pyrove
script_dir = os.path.dirname(os.path.abspath(__file__))
build_dir = os.path.join(script_dir, 'build')
if os.path.exists(build_dir):
    sys.path.insert(0, build_dir)

try:
    import pyrove
except ImportError:
    print("Error: pyrove module not found. Build the project with -DROVE_BUILD_PYTHON=ON")
    print(f"Looked in: {build_dir}")
    sys.exit(1)


class BenchmarkResult:
    """Store and format benchmark results."""

    def __init__(self, name, pyrove_time, numpy_time, iterations):
        self.name = name
        self.pyrove_time = pyrove_time
        self.numpy_time = numpy_time
        self.iterations = iterations
        self.speedup = numpy_time / pyrove_time if pyrove_time > 0 else 0

    def __str__(self):
        pyrove_us = self.pyrove_time * 1e6 / self.iterations
        numpy_us = self.numpy_time * 1e6 / self.iterations
        speedup_str = f"{self.speedup:.2f}x" if self.speedup >= 1 else f"{1/self.speedup:.2f}x slower"

        return (f"{self.name:40s} | "
                f"pyrove: {pyrove_us:8.2f} µs | "
                f"numpy: {numpy_us:8.2f} µs | "
                f"{speedup_str:15s}")


def benchmark(func, iterations=100000):
    """Benchmark a function by running it multiple times."""
    start = time.perf_counter()
    for _ in range(iterations):
        func()
    end = time.perf_counter()
    return end - start


def benchmark_vec3_operations(iterations=100000):
    """Benchmark 3D vector operations."""
    print("\n" + "="*80)
    print("3D Vector Operations")
    print("="*80)

    results = []

    # Vector addition
    def pyrove_add():
        v1 = pyrove.vec3(1.0, 2.0, 3.0)
        v2 = pyrove.vec3(4.0, 5.0, 6.0)
        v3 = v1 + v2

    def numpy_add():
        v1 = np.array([1.0, 2.0, 3.0], dtype=np.float32)
        v2 = np.array([4.0, 5.0, 6.0], dtype=np.float32)
        v3 = v1 + v2

    results.append(BenchmarkResult(
        "Vector Addition (vec3 + vec3)",
        benchmark(pyrove_add, iterations),
        benchmark(numpy_add, iterations),
        iterations
    ))

    # Vector subtraction
    def pyrove_sub():
        v1 = pyrove.vec3(1.0, 2.0, 3.0)
        v2 = pyrove.vec3(4.0, 5.0, 6.0)
        v3 = v1 - v2

    def numpy_sub():
        v1 = np.array([1.0, 2.0, 3.0], dtype=np.float32)
        v2 = np.array([4.0, 5.0, 6.0], dtype=np.float32)
        v3 = v1 - v2

    results.append(BenchmarkResult(
        "Vector Subtraction (vec3 - vec3)",
        benchmark(pyrove_sub, iterations),
        benchmark(numpy_sub, iterations),
        iterations
    ))

    # Scalar multiplication
    def pyrove_scale():
        v = pyrove.vec3(1.0, 2.0, 3.0)
        v2 = v * 2.5

    def numpy_scale():
        v = np.array([1.0, 2.0, 3.0], dtype=np.float32)
        v2 = v * 2.5

    results.append(BenchmarkResult(
        "Scalar Multiplication (vec3 * scalar)",
        benchmark(pyrove_scale, iterations),
        benchmark(numpy_scale, iterations),
        iterations
    ))

    # Dot product
    def pyrove_dot():
        v1 = pyrove.vec3(1.0, 2.0, 3.0)
        v2 = pyrove.vec3(4.0, 5.0, 6.0)
        d = v1.dot(v2)

    def numpy_dot():
        v1 = np.array([1.0, 2.0, 3.0], dtype=np.float32)
        v2 = np.array([4.0, 5.0, 6.0], dtype=np.float32)
        d = np.dot(v1, v2)

    results.append(BenchmarkResult(
        "Dot Product",
        benchmark(pyrove_dot, iterations),
        benchmark(numpy_dot, iterations),
        iterations
    ))

    # Cross product
    def pyrove_cross():
        v1 = pyrove.vec3(1.0, 2.0, 3.0)
        v2 = pyrove.vec3(4.0, 5.0, 6.0)
        c = v1.cross(v2)

    def numpy_cross():
        v1 = np.array([1.0, 2.0, 3.0], dtype=np.float32)
        v2 = np.array([4.0, 5.0, 6.0], dtype=np.float32)
        c = np.cross(v1, v2)

    results.append(BenchmarkResult(
        "Cross Product",
        benchmark(pyrove_cross, iterations),
        benchmark(numpy_cross, iterations),
        iterations
    ))

    # Vector length
    def pyrove_length():
        v = pyrove.vec3(1.0, 2.0, 3.0)
        l = v.length()

    def numpy_length():
        v = np.array([1.0, 2.0, 3.0], dtype=np.float32)
        l = np.linalg.norm(v)

    results.append(BenchmarkResult(
        "Vector Length (magnitude)",
        benchmark(pyrove_length, iterations),
        benchmark(numpy_length, iterations),
        iterations
    ))

    # Normalization
    def pyrove_normalize():
        v = pyrove.vec3(1.0, 2.0, 3.0)
        v.normalize()

    def numpy_normalize():
        v = np.array([1.0, 2.0, 3.0], dtype=np.float32)
        v = v / np.linalg.norm(v)

    results.append(BenchmarkResult(
        "Vector Normalization",
        benchmark(pyrove_normalize, iterations),
        benchmark(numpy_normalize, iterations),
        iterations
    ))

    for result in results:
        print(result)

    return results


def benchmark_mat4_operations(iterations=50000):
    """Benchmark 4x4 matrix operations."""
    print("\n" + "="*80)
    print("4x4 Matrix Operations")
    print("="*80)

    results = []

    # Matrix creation
    def pyrove_create():
        m = pyrove.mat4()
        m.identity()

    def numpy_create():
        m = np.eye(4, dtype=np.float32)

    results.append(BenchmarkResult(
        "Matrix Creation (identity)",
        benchmark(pyrove_create, iterations),
        benchmark(numpy_create, iterations),
        iterations
    ))

    # Matrix multiplication
    def pyrove_matmul():
        m1 = pyrove.mat4()
        m1.identity()
        m2 = pyrove.mat4()
        m2.identity()
        m3 = m1 * m2

    def numpy_matmul():
        m1 = np.eye(4, dtype=np.float32)
        m2 = np.eye(4, dtype=np.float32)
        m3 = np.matmul(m1, m2)

    results.append(BenchmarkResult(
        "Matrix Multiplication (mat4 * mat4)",
        benchmark(pyrove_matmul, iterations),
        benchmark(numpy_matmul, iterations),
        iterations
    ))

    # Matrix-vector multiplication
    def pyrove_matvec():
        m = pyrove.mat4()
        m.identity()
        v = pyrove.vec3(1.0, 2.0, 3.0)
        v2 = m * v

    def numpy_matvec():
        m = np.eye(4, dtype=np.float32)
        v = np.array([1.0, 2.0, 3.0, 1.0], dtype=np.float32)
        v2 = np.dot(m, v)

    results.append(BenchmarkResult(
        "Matrix-Vector Multiplication",
        benchmark(pyrove_matvec, iterations),
        benchmark(numpy_matvec, iterations),
        iterations
    ))

    # Translation matrix
    def pyrove_translate():
        m = pyrove.mat4()
        m.translation(1.0, 2.0, 3.0)

    def numpy_translate():
        m = np.eye(4, dtype=np.float32)
        m[0, 3] = 1.0
        m[1, 3] = 2.0
        m[2, 3] = 3.0

    results.append(BenchmarkResult(
        "Translation Matrix Creation",
        benchmark(pyrove_translate, iterations),
        benchmark(numpy_translate, iterations),
        iterations
    ))

    # Matrix transpose
    def pyrove_transpose():
        m = pyrove.mat4()
        m.identity()
        mt = m.transpose()

    def numpy_transpose():
        m = np.eye(4, dtype=np.float32)
        mt = m.T

    results.append(BenchmarkResult(
        "Matrix Transpose",
        benchmark(pyrove_transpose, iterations),
        benchmark(numpy_transpose, iterations),
        iterations
    ))

    # Matrix inverse
    def pyrove_inverse():
        m = pyrove.mat4()
        m.translation(1.0, 2.0, 3.0)
        mi = m.inverse()

    def numpy_inverse():
        m = np.eye(4, dtype=np.float32)
        m[0, 3] = 1.0
        m[1, 3] = 2.0
        m[2, 3] = 3.0
        mi = np.linalg.inv(m)

    results.append(BenchmarkResult(
        "Matrix Inverse",
        benchmark(pyrove_inverse, iterations),
        benchmark(numpy_inverse, iterations),
        iterations
    ))

    for result in results:
        print(result)

    return results


def benchmark_conversion_overhead(iterations=50000):
    """Benchmark NumPy conversion overhead."""
    print("\n" + "="*80)
    print("NumPy Conversion Overhead")
    print("="*80)

    results = []

    # vec3 to numpy
    def pyrove_to_numpy():
        v = pyrove.vec3(1.0, 2.0, 3.0)
        arr = v.to_numpy()

    def numpy_create():
        arr = np.array([1.0, 2.0, 3.0], dtype=np.float32)

    results.append(BenchmarkResult(
        "vec3.to_numpy() vs np.array()",
        benchmark(pyrove_to_numpy, iterations),
        benchmark(numpy_create, iterations),
        iterations
    ))

    # numpy to vec3
    arr = np.array([1.0, 2.0, 3.0], dtype=np.float32)

    def numpy_to_pyrove():
        v = pyrove.vec3.from_numpy(arr)

    def pyrove_create():
        v = pyrove.vec3(1.0, 2.0, 3.0)

    results.append(BenchmarkResult(
        "vec3.from_numpy() vs vec3() constructor",
        benchmark(numpy_to_pyrove, iterations),
        benchmark(pyrove_create, iterations),
        iterations
    ))

    # mat4 to numpy
    def mat_to_numpy():
        m = pyrove.mat4()
        m.identity()
        arr = m.to_numpy()

    def numpy_mat_create():
        arr = np.eye(4, dtype=np.float32)

    results.append(BenchmarkResult(
        "mat4.to_numpy() vs np.eye()",
        benchmark(mat_to_numpy, iterations),
        benchmark(numpy_mat_create, iterations),
        iterations
    ))

    for result in results:
        print(result)

    return results


def benchmark_geometric_operations(iterations=50000):
    """Benchmark geometric primitive operations."""
    print("\n" + "="*80)
    print("Geometric Primitive Operations")
    print("="*80)

    results = []

    # Ray-point distance (no direct NumPy equivalent)
    def pyrove_ray_distance():
        ray = pyrove.ray3(pyrove.vec3(0, 0, 0), pyrove.vec3(1, 0, 0))
        point = pyrove.vec3(5, 3, 0)
        dist = ray.distance(point)

    def numpy_ray_distance():
        # Manual implementation
        origin = np.array([0, 0, 0], dtype=np.float32)
        direction = np.array([1, 0, 0], dtype=np.float32)
        point = np.array([5, 3, 0], dtype=np.float32)
        v = point - origin
        t = np.dot(v, direction)
        closest = origin + t * direction
        dist = np.linalg.norm(point - closest)

    results.append(BenchmarkResult(
        "Ray-Point Distance",
        benchmark(pyrove_ray_distance, iterations),
        benchmark(numpy_ray_distance, iterations),
        iterations
    ))

    # Plane intersection test
    def pyrove_plane_test():
        plane = pyrove.plane(pyrove.vec3(0, 0, 0), pyrove.vec3(0, 1, 0))
        ray = pyrove.ray3(pyrove.vec3(0, 10, 0), pyrove.vec3(0, -1, 0))
        intersects = plane.test_intersection(ray)

    def numpy_plane_test():
        # Manual implementation
        plane_n = np.array([0, 1, 0], dtype=np.float32)
        plane_d = 0.0
        ray_o = np.array([0, 10, 0], dtype=np.float32)
        ray_d = np.array([0, -1, 0], dtype=np.float32)
        denom = np.dot(plane_n, ray_d)
        intersects = abs(denom) > 1e-6

    results.append(BenchmarkResult(
        "Ray-Plane Intersection Test",
        benchmark(pyrove_plane_test, iterations),
        benchmark(numpy_plane_test, iterations),
        iterations
    ))

    # Triangle area calculation
    def pyrove_triangle_area():
        tri = pyrove.triangle3(
            pyrove.vec3(0, 0, 0),
            pyrove.vec3(1, 0, 0),
            pyrove.vec3(0, 1, 0)
        )
        area = tri.area()

    def numpy_triangle_area():
        # Using cross product
        a = np.array([0, 0, 0], dtype=np.float32)
        b = np.array([1, 0, 0], dtype=np.float32)
        c = np.array([0, 1, 0], dtype=np.float32)
        ab = b - a
        ac = c - a
        cross = np.cross(ab, ac)
        area = 0.5 * np.linalg.norm(cross)

    results.append(BenchmarkResult(
        "Triangle Area Calculation",
        benchmark(pyrove_triangle_area, iterations),
        benchmark(numpy_triangle_area, iterations),
        iterations
    ))

    for result in results:
        print(result)

    return results


def print_summary(all_results):
    """Print summary statistics."""
    print("\n" + "="*80)
    print("Summary")
    print("="*80)

    pyrove_wins = sum(1 for r in all_results if r.speedup >= 1.0)
    numpy_wins = len(all_results) - pyrove_wins

    avg_speedup = sum(r.speedup for r in all_results) / len(all_results)

    print(f"Total benchmarks: {len(all_results)}")
    print(f"pyrove faster:    {pyrove_wins} ({pyrove_wins/len(all_results)*100:.1f}%)")
    print(f"NumPy faster:     {numpy_wins} ({numpy_wins/len(all_results)*100:.1f}%)")
    print(f"Average speedup:  {avg_speedup:.2f}x")

    print("\nFastest pyrove operations:")
    sorted_results = sorted(all_results, key=lambda r: r.speedup, reverse=True)
    for result in sorted_results[:3]:
        print(f"  {result.name:40s} - {result.speedup:.2f}x faster")

    print("\nFastest NumPy operations:")
    sorted_results = sorted(all_results, key=lambda r: r.speedup)
    for result in sorted_results[:3]:
        print(f"  {result.name:40s} - {1/result.speedup:.2f}x faster")


def main():
    print("="*80)
    print("pyrove vs NumPy Performance Benchmark")
    print("="*80)
    print("\nBenchmarking common vector and matrix operations...")
    print("Lower time per operation is better.")
    print()

    all_results = []

    # Run benchmarks
    all_results.extend(benchmark_vec3_operations(iterations=100000))
    all_results.extend(benchmark_mat4_operations(iterations=50000))
    all_results.extend(benchmark_conversion_overhead(iterations=50000))
    all_results.extend(benchmark_geometric_operations(iterations=50000))

    # Print summary
    print_summary(all_results)

    print("\n" + "="*80)
    print("Benchmark complete!")
    print("="*80)
    print("\nNotes:")
    print("- pyrove operations are often faster for small, fixed-size vectors/matrices")
    print("- NumPy may be faster for large arrays and vectorized operations")
    print("- Conversion between pyrove and NumPy has overhead")
    print("- pyrove is optimized for graphics/robotics use cases")
    print("- Use pyrove for tight loops with small vectors/matrices")
    print("- Use NumPy for large-scale numerical computations")


if __name__ == "__main__":
    main()
