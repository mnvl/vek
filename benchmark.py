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

Benchmark Results (Release build with -O3 -march=native -flto):
    pyrove faster for:
    - Triangle area: 218.2x faster (0.07 µs vs 15.98 µs)
    - Cross products: 144.3x faster (0.10 µs vs 14.34 µs)
    - Ray-point distance: 37.3x faster (0.09 µs vs 3.38 µs)
    - Matrix inverse: 23.9x faster (0.19 µs vs 4.46 µs)
    - Vector normalization: 14.1x faster (0.15 µs vs 2.13 µs)
    - In-place addition (v += other): 2.8x faster (0.12 µs vs 0.32 µs)

    NumPy faster for:
    - Array conversions: 2.4-3.7x faster (significant overhead)
    - Matrix transpose: ~1.0x (effectively tied)

    Overall: pyrove wins 15/19 benchmarks (78.9%)

See BENCHMARKS.md for detailed analysis.
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


def benchmark_object_creation(iterations=100000):
    """Benchmark object creation overhead."""
    print("\n" + "="*80)
    print("Object Creation")
    print("="*80)

    results = []

    # vec3 creation
    def pyrove_vec3_create():
        v = pyrove.vec3(1.0, 2.0, 3.0)

    def numpy_vec3_create():
        v = np.array([1.0, 2.0, 3.0], dtype=np.float32)

    results.append(BenchmarkResult(
        "vec3 Creation",
        benchmark(pyrove_vec3_create, iterations),
        benchmark(numpy_vec3_create, iterations),
        iterations
    ))

    # mat4 creation
    def pyrove_mat4_create():
        m = pyrove.mat4()

    def numpy_mat4_create():
        m = np.zeros((4, 4), dtype=np.float32)

    results.append(BenchmarkResult(
        "mat4 Creation (zero matrix)",
        benchmark(pyrove_mat4_create, iterations),
        benchmark(numpy_mat4_create, iterations),
        iterations
    ))

    # mat4 identity creation
    def pyrove_mat4_identity():
        m = pyrove.mat4()
        m.identity()

    def numpy_mat4_identity():
        m = np.eye(4, dtype=np.float32)

    results.append(BenchmarkResult(
        "mat4 Identity Creation",
        benchmark(pyrove_mat4_identity, iterations),
        benchmark(numpy_mat4_identity, iterations),
        iterations
    ))

    for result in results:
        print(result)

    return results


def benchmark_vec3_operations(iterations=100000):
    """Benchmark 3D vector operations."""
    print("\n" + "="*80)
    print("3D Vector Operations (Pure Arithmetic)")
    print("="*80)

    results = []

    # Pre-create objects
    pv1 = pyrove.vec3(1.0, 2.0, 3.0)
    pv2 = pyrove.vec3(4.0, 5.0, 6.0)
    nv1 = np.array([1.0, 2.0, 3.0], dtype=np.float32)
    nv2 = np.array([4.0, 5.0, 6.0], dtype=np.float32)

    # Vector addition
    def pyrove_add():
        v3 = pv1 + pv2

    def numpy_add():
        v3 = nv1 + nv2

    results.append(BenchmarkResult(
        "Vector Addition (vec3 + vec3)",
        benchmark(pyrove_add, iterations),
        benchmark(numpy_add, iterations),
        iterations
    ))

    # Vector subtraction
    def pyrove_sub():
        v3 = pv1 - pv2

    def numpy_sub():
        v3 = nv1 - nv2

    results.append(BenchmarkResult(
        "Vector Subtraction (vec3 - vec3)",
        benchmark(pyrove_sub, iterations),
        benchmark(numpy_sub, iterations),
        iterations
    ))

    # Scalar multiplication
    pv = pyrove.vec3(1.0, 2.0, 3.0)
    nv = np.array([1.0, 2.0, 3.0], dtype=np.float32)

    def pyrove_scale():
        v2 = pv * 2.5

    def numpy_scale():
        v2 = nv * 2.5

    results.append(BenchmarkResult(
        "Scalar Multiplication (vec3 * scalar)",
        benchmark(pyrove_scale, iterations),
        benchmark(numpy_scale, iterations),
        iterations
    ))

    # Dot product
    def pyrove_dot():
        d = pv1.dot(pv2)

    def numpy_dot():
        d = np.dot(nv1, nv2)

    results.append(BenchmarkResult(
        "Dot Product",
        benchmark(pyrove_dot, iterations),
        benchmark(numpy_dot, iterations),
        iterations
    ))

    # Cross product
    def pyrove_cross():
        c = pv1.cross(pv2)

    def numpy_cross():
        c = np.cross(nv1, nv2)

    results.append(BenchmarkResult(
        "Cross Product",
        benchmark(pyrove_cross, iterations),
        benchmark(numpy_cross, iterations),
        iterations
    ))

    # Vector length
    pvl = pyrove.vec3(1.0, 2.0, 3.0)
    nvl = np.array([1.0, 2.0, 3.0], dtype=np.float32)

    def pyrove_length():
        l = pvl.length()

    def numpy_length():
        l = np.linalg.norm(nvl)

    results.append(BenchmarkResult(
        "Vector Length (magnitude)",
        benchmark(pyrove_length, iterations),
        benchmark(numpy_length, iterations),
        iterations
    ))

    # Normalization (mutable operation, create fresh each time)
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
    print("4x4 Matrix Operations (Pure Arithmetic)")
    print("="*80)

    results = []

    # Pre-create matrices
    pm1 = pyrove.mat4()
    pm1.identity()
    pm2 = pyrove.mat4()
    pm2.identity()
    nm1 = np.eye(4, dtype=np.float32)
    nm2 = np.eye(4, dtype=np.float32)

    # Matrix multiplication
    def pyrove_matmul():
        m3 = pm1 * pm2

    def numpy_matmul():
        m3 = np.matmul(nm1, nm2)

    results.append(BenchmarkResult(
        "Matrix Multiplication (mat4 * mat4)",
        benchmark(pyrove_matmul, iterations),
        benchmark(numpy_matmul, iterations),
        iterations
    ))

    # Matrix-vector multiplication
    pv = pyrove.vec3(1.0, 2.0, 3.0)
    nv = np.array([1.0, 2.0, 3.0, 1.0], dtype=np.float32)

    def pyrove_matvec():
        v2 = pm1 * pv

    def numpy_matvec():
        v2 = np.dot(nm1, nv)

    results.append(BenchmarkResult(
        "Matrix-Vector Multiplication",
        benchmark(pyrove_matvec, iterations),
        benchmark(numpy_matvec, iterations),
        iterations
    ))

    # Matrix transpose
    def pyrove_transpose():
        mt = pm1.transpose()

    def numpy_transpose():
        mt = nm1.T

    results.append(BenchmarkResult(
        "Matrix Transpose",
        benchmark(pyrove_transpose, iterations),
        benchmark(numpy_transpose, iterations),
        iterations
    ))

    # Matrix inverse (with translation for non-trivial matrix)
    ptm = pyrove.mat4()
    ptm.translation(1.0, 2.0, 3.0)
    ntm = np.eye(4, dtype=np.float32)
    ntm[0, 3] = 1.0
    ntm[1, 3] = 2.0
    ntm[2, 3] = 3.0

    def pyrove_inverse():
        mi = ptm.inverse()

    def numpy_inverse():
        mi = np.linalg.inv(ntm)

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


def benchmark_temporary_objects(iterations=100000):
    """Benchmark temporary object creation overhead.

    This section isolates the cost of creating new vector objects vs
    modifying existing ones in place. pyrove's += operator creates a
    temporary (no __iadd__), while NumPy's += is truly in-place.
    """
    print("\n" + "="*80)
    print("Temporary Object Overhead (Creation vs In-Place)")
    print("="*80)

    results = []

    # --- Test 1: Vector creation ---
    def pyrove_vec3_create():
        v = pyrove.vec3(1.0, 2.0, 3.0)

    def numpy_vec3_create():
        v = np.array([1.0, 2.0, 3.0], dtype=np.float32)

    results.append(BenchmarkResult(
        "vec3 Creation",
        benchmark(pyrove_vec3_create, iterations),
        benchmark(numpy_vec3_create, iterations),
        iterations
    ))

    # --- Test 2: In-place addition (+=) ---
    # For NumPy, += is truly in-place (no allocation).
    # For pyrove, += falls back to __add__ + rebind (creates temporary).
    pv_acc = pyrove.vec3(0.0, 0.0, 0.0)
    pv_delta = pyrove.vec3(1.0, 2.0, 3.0)
    nv_acc = np.array([0.0, 0.0, 0.0], dtype=np.float32)
    nv_delta = np.array([1.0, 2.0, 3.0], dtype=np.float32)

    def pyrove_iadd():
        nonlocal pv_acc
        pv_acc += pv_delta

    def numpy_iadd():
        nonlocal nv_acc
        nv_acc += nv_delta

    results.append(BenchmarkResult(
        "In-place Addition (v += other)",
        benchmark(pyrove_iadd, iterations),
        benchmark(numpy_iadd, iterations),
        iterations
    ))

    for result in results:
        print(result)

    return results


def benchmark_geometric_operations(iterations=50000):
    """Benchmark geometric primitive operations."""
    print("\n" + "="*80)
    print("Geometric Primitive Operations (Pure Computation)")
    print("="*80)

    results = []

    # Pre-create geometric objects
    pray = pyrove.ray3(pyrove.vec3(0, 0, 0), pyrove.vec3(1, 0, 0))
    ppoint = pyrove.vec3(5, 3, 0)

    # NumPy equivalents (pre-created)
    norigin = np.array([0, 0, 0], dtype=np.float32)
    ndirection = np.array([1, 0, 0], dtype=np.float32)
    npoint = np.array([5, 3, 0], dtype=np.float32)

    # Ray-point distance
    def pyrove_ray_distance():
        dist = pray.distance(ppoint)

    def numpy_ray_distance():
        # Manual implementation
        v = npoint - norigin
        t = np.dot(v, ndirection)
        closest = norigin + t * ndirection
        dist = np.linalg.norm(npoint - closest)

    results.append(BenchmarkResult(
        "Ray-Point Distance",
        benchmark(pyrove_ray_distance, iterations),
        benchmark(numpy_ray_distance, iterations),
        iterations
    ))

    # Plane intersection test
    pplane = pyrove.plane(pyrove.vec3(0, 0, 0), pyrove.vec3(0, 1, 0))
    pray2 = pyrove.ray3(pyrove.vec3(0, 10, 0), pyrove.vec3(0, -1, 0))

    # NumPy equivalents
    nplane_n = np.array([0, 1, 0], dtype=np.float32)
    nplane_d = 0.0
    nray_o = np.array([0, 10, 0], dtype=np.float32)
    nray_d = np.array([0, -1, 0], dtype=np.float32)

    def pyrove_plane_test():
        intersects = pplane.test_intersection(pray2)

    def numpy_plane_test():
        # Manual implementation
        denom = np.dot(nplane_n, nray_d)
        intersects = abs(denom) > 1e-6

    results.append(BenchmarkResult(
        "Ray-Plane Intersection Test",
        benchmark(pyrove_plane_test, iterations),
        benchmark(numpy_plane_test, iterations),
        iterations
    ))

    # Triangle area calculation
    ptri = pyrove.triangle3(
        pyrove.vec3(0, 0, 0),
        pyrove.vec3(1, 0, 0),
        pyrove.vec3(0, 1, 0)
    )

    # NumPy equivalents
    na = np.array([0, 0, 0], dtype=np.float32)
    nb = np.array([1, 0, 0], dtype=np.float32)
    nc = np.array([0, 1, 0], dtype=np.float32)
    nab = nb - na
    nac = nc - na

    def pyrove_triangle_area():
        area = ptri.area()

    def numpy_triangle_area():
        # Using cross product
        cross = np.cross(nab, nac)
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
    all_results.extend(benchmark_temporary_objects(iterations=100000))
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
