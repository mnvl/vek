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
| **Vector Addition** | **0.18** | **0.40** | **pyrove** | **2.2x faster** |
| **Vector Subtraction** | **0.10** | **0.36** | **pyrove** | **3.7x faster** |
| **Scalar Multiplication** | **0.11** | **0.67** | **pyrove** | **6.0x faster** |
| **Dot Product** | **0.07** | **0.55** | **pyrove** | **8.2x faster** |
| **Cross Product** | **0.10** | **14.34** | **pyrove** | **144.3x faster** |
| **Vector Length** | **0.06** | **1.09** | **pyrove** | **18.4x faster** |
| **Vector Normalization** | **0.15** | **2.13** | **pyrove** | **14.1x faster** |

### 4x4 Matrix Operations

| Operation | pyrove (µs) | NumPy (µs) | Winner | Speedup |
|-----------|-------------|------------|--------|---------|
| **Matrix Multiplication** | **0.14** | **1.09** | **pyrove** | **8.0x faster** |
| **Matrix-Vector Multiplication** | **0.14** | **0.57** | **pyrove** | **4.1x faster** |
| Matrix Transpose | 0.10 | 0.10 | Tie | ~1.0x |
| **Matrix Inverse** | **0.19** | **4.46** | **pyrove** | **23.9x faster** |

### Temporary Object Overhead

| Operation | pyrove (µs) | NumPy (µs) | Winner | Speedup |
|-----------|-------------|------------|--------|---------|
| **vec3 Creation** | **0.13** | **0.40** | **pyrove** | **3.1x faster** |
| **In-place Addition (v += other)** | **0.12** | **0.32** | **pyrove** | **2.8x faster** |

### Geometric Primitive Operations

| Operation | pyrove (µs) | NumPy (µs) | Winner | Speedup |
|-----------|-------------|------------|--------|---------|
| **Ray-Point Distance** | **0.09** | **3.38** | **pyrove** | **37.3x faster** |
| **Ray-Plane Intersection** | **0.07** | **0.62** | **pyrove** | **8.9x faster** |
| **Triangle Area** | **0.07** | **15.98** | **pyrove** | **218.2x faster** |

### NumPy Conversion Overhead

| Operation | pyrove (µs) | NumPy (µs) | Overhead |
|-----------|-------------|------------|----------|
| vec3.to_numpy() | 1.05 | 0.44 | 2.4x slower |
| vec3.from_numpy() | 0.44 | 0.12 | 3.7x slower |
| mat4.to_numpy() | 1.07 | 1.04 | 1.0x slower |

## Summary Statistics

- **Total benchmarks**: 19 operations tested
- **pyrove wins**: 15 operations (78.9%)
- **NumPy wins**: 4 operations (21.1%)
- **Average speedup**: 26.62x

### Top 3 pyrove Advantages

1. **Triangle Area Calculation**: 218.2x faster than NumPy
2. **Cross Product**: 144.3x faster than NumPy
3. **Ray-Point Distance**: 37.3x faster than NumPy

### Top 3 NumPy Advantages

1. **Array Conversions (from_numpy)**: 3.7x faster (conversion overhead)
2. **Array Conversions (to_numpy)**: 2.4x faster (conversion overhead)
3. **Array Conversions (mat4 to_numpy)**: 1.0x faster (effectively tied)

## Performance Analysis

### When to Use pyrove

pyrove excels at:

1. **Geometric Operations**
   - Triangle calculations: 218.2x faster
   - Cross products: 144.3x faster
   - Ray-point distance: 37.3x faster
   - Ray-plane intersection: 8.9x faster

2. **Matrix Operations**
   - Matrix inverse: 23.9x faster
   - Matrix multiplication: 8.0x faster
   - Matrix-vector multiplication: 4.1x faster

3. **Vector Arithmetic**
   - Dot products: 8.2x faster
   - Vector length: 18.4x faster
   - Normalization: 14.1x faster
   - Scalar multiplication: 6.0x faster
   - Addition/subtraction: 2.2-3.7x faster
   - In-place addition (v += other): 2.8x faster

4. **Use Cases**
   - Real-time graphics rendering
   - Robotics kinematics/dynamics
   - Game physics engines
   - CAD/geometric modeling
   - Ray tracing applications
   - Tight loops with 2D/3D/4D vectors

### When to Use NumPy

NumPy excels at:

1. **Large-Scale Operations**
   - Batch operations on arrays of thousands+ elements
   - Vectorized computations across large datasets

2. **Interoperability**
   - Direct integration with scipy, pandas, sklearn, etc.
   - Converting between pyrove and NumPy has overhead

3. **Use Cases**
   - Scientific computing
   - Data analysis
   - Machine learning (batch operations)
   - Signal processing
   - Large numerical simulations

### Conversion Overhead

Converting between pyrove and NumPy has overhead:
- **to_numpy()**: 2.4x slower than direct NumPy creation
- **from_numpy()**: 3.7x slower than direct pyrove creation

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
area = tri.area()                    # 218x faster than NumPy
normal = tri.get_normal()
centroid = tri.cog()

# Use pyrove for transformations
v1 = pyrove.vec3(1, 2, 3)
v2 = pyrove.vec3(4, 5, 6)
cross = v1.cross(v2)                 # 144x faster than NumPy

# Use pyrove for in-place operations (no temporary objects)
v1 += v2                             # 2.8x faster than NumPy

# Use pyrove for matrix operations
m = pyrove.mat4()
m.translation(1, 2, 3)
m_inv = m.inverse()                  # 24x faster than NumPy
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

- **pyrove** is faster than NumPy for **15 out of 19** benchmarks (78.9%)
- pyrove excels at **all** vector, matrix, and geometric operations on small fixed-size types
- NumPy's only advantage is **conversion overhead** (pyrove ↔ NumPy interop)
- Choose based on your use case:
  - Individual vector/matrix/geometry operations → pyrove
  - Batch numerical operations on large arrays → NumPy
- Minimize conversions between libraries
- Always use optimized builds for production
