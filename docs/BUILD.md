# Building the Documentation

This guide explains how to build the rove documentation.

## Prerequisites

Install the required Python packages:

```bash
pip install -r requirements.txt
```

This installs:
- Sphinx (documentation generator)
- sphinx-rtd-theme (Read the Docs theme)
- breathe (bridge between Sphinx and Doxygen for C++ docs)
- numpy (for NumPy integration examples)

## Building HTML Documentation

From the `docs/` directory:

```bash
cd docs
make html
```

Or use the Sphinx build command directly:

```bash
sphinx-build -b html . _build/html
```

## Viewing the Documentation

After building, open the documentation in your browser:

```bash
# On macOS
open _build/html/index.html

# On Linux
xdg-open _build/html/index.html

# Or manually navigate to:
# docs/_build/html/index.html
```

## Other Build Targets

```bash
# Clean build artifacts
make clean

# Build PDF (requires LaTeX)
make latexpdf

# Build man pages
make man

# See all available targets
make help
```

## Troubleshooting

### Missing Dependencies

If you get an error about missing modules:

```bash
pip install -r requirements.txt
```

### Using a Virtual Environment

It's recommended to use a virtual environment:

```bash
# Create virtual environment
python3 -m venv .venv

# Activate it
source .venv/bin/activate  # On macOS/Linux
# or
.venv\Scripts\activate  # On Windows

# Install requirements
pip install -r requirements.txt

# Build docs
make html
```

### Cleaning Build Artifacts

If you encounter build issues, try cleaning first:

```bash
make clean
make html
```

## Documentation Structure

The documentation is organized as:

- `index.rst` - Main documentation index
- `getting_started.rst` - Getting started guide
- `api/` - Python API reference documentation
  - `vectors.rst`, `matrices.rst`, `quaternions.rst`
  - `ray.rst`, `line.rst`, `plane.rst`, `triangle.rst`
  - `capsule.rst`, `frustum.rst`
  - `aabb.rst`, `obb.rst`
- `geometric_primitives.rst` - Comprehensive guide for geometric classes
- `cpp_api/` - C++ API reference (auto-generated from Doxygen)
- `conf.py` - Sphinx configuration

## Live Rebuild (Optional)

For automatic rebuilding during development:

```bash
pip install sphinx-autobuild
sphinx-autobuild . _build/html
```

Then open http://127.0.0.1:8000 in your browser. The page will auto-refresh when you save changes.
