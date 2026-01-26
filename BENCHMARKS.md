# Performance Benchmarks

This document contains detailed performance benchmark results comparing pyrove with NumPy.

## Test Environment

- **Build**: Release build with `-O3 -march=native -mtune=native -flto`
- **Python**: 3.14.2
- **NumPy**: 2.4.1
- **Platform**: macOS (ARM64)
- **Compiler**: Clang with full optimizations

## Benchmark Results

All times are in microseconds (µs) per operation. Lower is better.

### 3D Vector Operations

| Operation | pyrove (µs) | NumPy (µs) | Winner | Speedup |
|-----------|-------------|------------|--------|---------|
| Vector Addition | 2.77 | 0.91 | NumPy | 3.0x faster |
| Vector Subtraction | 2.69 | 0.91 | NumPy | 3.0x faster |
| Scalar Multiplication | 1.81 | 0.82 | NumPy | 2.2x faster |
| Dot Product | 1.76 | 1.08 | NumPy | 1.6x faster |
| **Cross Product** | **2.70** | **11.98** | **pyrove** | **4.4x faster** |
| **Vector Length** | **0.86** | **1.16** | **pyrove** | **1.4x faster** |
| **Vector Normalization** | **0.85** | **1.54** | **pyrove** | **1.8x faster** |

### 4x4 Matrix Operations

| Operation | pyrove (µs) | NumPy (µs) | Winner | Speedup |
|-----------|-------------|------------|--------|---------|
| **Matrix Creation (identity)** | **0.74** | **0.84** | **pyrove** | **1.1x faster** |
| Matrix Multiplication | 2.84 | 2.35 | NumPy | 1.2x faster |
| Matrix-Vector Multiplication | 3.01 | 1.63 | NumPy | 1.8x faster |
| **Translation Matrix** | **0.86** | **0.97** | **pyrove** | **1.1x faster** |
| Matrix Transpose | 1.68 | 0.92 | NumPy | 1.8x faster |
| **Matrix Inverse** | **1.94** | **4.29** | **pyrove** | **2.2x faster** |

### Geometric Primitive Operations

| Operation | pyrove (µs) | NumPy (µs) | Winner | Speedup |
|-----------|-------------|------------|--------|---------|
| Ray-Point Distance | 3.63 | 3.51 | Tie | ~1.0x |
| Ray-Plane Intersection | 4.58 | 1.51 | NumPy | 3.0x faster |
| **Triangle Area** | **3.66** | **14.08** | **pyrove** | **3.9x faster** |

### NumPy Conversion Overhead

| Operation | pyrove (µs) | NumPy (µs) | Overhead |
|-----------|-------------|------------|----------|
| vec3.to_numpy() | 2.08 | 0.34 | 6.1x slower |
| vec3.from_numpy() | 1.31 | 0.75 | 1.7x slower |
| mat4.to_numpy() | 2.12 | 0.84 | 2.5x slower |

## Summary Statistics

- **Total benchmarks**: 19 operations tested
- **pyrove wins**: 7 operations (36.8%)
- **NumPy wins**: 12 operations (63.2%)
- **Average speedup**: 1.16x (slightly slower overall)

### Top 3 pyrove Advantages

1. **Cross Product**: 4.43x faster than NumPy
2. **Triangle Area Calculation**: 3.85x faster than NumPy
3. **Matrix Inverse**: 2.21x faster than NumPy

### Top 3 NumPy Advantages

1. **Array Conversions**: 6.13x faster (to_numpy overhead)
2. **Ray-Plane Intersection**: 3.04x faster
3. **Vector Addition**: 3.03x faster

## Performance Analysis

### When to Use pyrove

pyrove excels at:

1. **Geometric Operations**
   - Cross products: 4.4x faster
   - Triangle calculations: 3.9x faster
   - Specialized geometric primitives (rays, planes, triangles)

2. **Matrix Operations**
   - Matrix inverse: 2.2x faster
   - Matrix creation: 1.1x faster

3. **Vector Utilities**
   - Normalization: 1.8x faster
   - Length calculation: 1.4x faster

4. **Use Cases**
   - Real-time graphics rendering
   - Robotics kinematics/dynamics
   - Game physics engines
   - CAD/geometric modeling
   - Ray tracing applications
   - Tight loops with 2D/3D/4D vectors

### When to Use NumPy

NumPy excels at:

1. **Basic Vector Arithmetic**
   - Addition/subtraction: 3x faster
   - Scalar multiplication: 2.2x faster
   - Dot products: 1.6x faster

2. **Large-Scale Operations**
   - Batch operations on arrays
   - Matrix-vector multiplications
   - Vectorized computations

3. **Use Cases**
   - Scientific computing
   - Data analysis
   - Machine learning (batch operations)
   - Signal processing
   - Large numerical simulations

### Conversion Overhead

Converting between pyrove and NumPy has significant overhead:
- **to_numpy()**: 6x slower than direct NumPy creation
- **from_numpy()**: 1.7x slower than direct pyrove creation

**Best Practice**: Minimize conversions. Choose one library for your hot path.

## Optimization Impact

The optimized build (Release with `-O3 -march=native -flto`) provides:
- **8x smaller binary** (527KB vs 4.4MB debug)
- **Estimated 5-10x faster** than debug builds
- **Better cache utilization** from smaller code size

## Recommendations

### For Graphics/Robotics Applications

```python
# Use pyrove for geometric calculations
tri = pyrove.triangle3(v1, v2, v3)
area = tri.area()                    # 3.9x faster than NumPy
normal = tri.get_normal()
centroid = tri.cog()

# Use pyrove for transformations
v1 = pyrove.vec3(1, 2, 3)
v2 = pyrove.vec3(4, 5, 6)
cross = v1.cross(v2)                 # 4.4x faster than NumPy

# Use pyrove for matrix operations
m = pyrove.mat4()
m.translation(1, 2, 3)
m_inv = m.inverse()                  # 2.2x faster than NumPy
```

### For Numerical Computing

```python
# Use NumPy for batch operations
points = np.random.rand(10000, 3)
transformed = points + offset         # Much faster with NumPy
normalized = points / np.linalg.norm(points, axis=1, keepdims=True)
```

### Mixed Usage Pattern

```python
# Process in NumPy, extract for pyrove when needed
import numpy as np
import pyrove

# Batch operations in NumPy
vertices = np.random.rand(1000, 3).astype(np.float32)
centered = vertices - vertices.mean(axis=0)

# Extract individual vectors for geometric operations
for i in range(0, len(centered), 3):
    # Create pyrove triangle for specialized calculations
    v1 = pyrove.vec3(*centered[i])
    v2 = pyrove.vec3(*centered[i+1])
    v3 = pyrove.vec3(*centered[i+2])

    tri = pyrove.triangle3(v1, v2, v3)
    area = tri.area()                 # Faster than NumPy cross product
    normal = tri.get_normal()
```

## Running the Benchmark

To reproduce these results:

```bash
# Build optimized version
./build_release.sh

# Run benchmark
./run_benchmark.sh

# Or manually
PYTHONPATH=build-release .venv/bin/python3.14 benchmark.py
```

## Conclusion

- **pyrove** is optimized for **graphics and robotics** with small fixed-size types
- **NumPy** is optimized for **large-scale numerical computing**
- Choose based on your use case:
  - Geometric operations → pyrove
  - Batch numerical operations → NumPy
- Minimize conversions between libraries
- Always use optimized builds for production

## Future Improvements

Potential areas for pyrove optimization:
1. SIMD vectorization for basic arithmetic
2. Template specialization for common operations
3. Inline more Python binding code
4. Profile-guided optimization (PGO)

Expected gains: 1.5-2x additional speedup possible
