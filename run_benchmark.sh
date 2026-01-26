#!/bin/bash
# Wrapper script to run the pyrove vs NumPy benchmark

# Find the correct Python interpreter
if [ -f ".venv/bin/python3.14" ]; then
    PYTHON=".venv/bin/python3.14"
elif [ -f ".venv/bin/python3" ]; then
    PYTHON=".venv/bin/python3"
else
    PYTHON="python3"
fi

echo "Using Python: $PYTHON"
$PYTHON --version

# Run the benchmark
$PYTHON benchmark.py "$@"
