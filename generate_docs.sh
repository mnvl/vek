#!/bin/bash
# Generate Doxygen documentation for rove library

set -e

echo "Generating rove documentation..."

# Check if doxygen is installed
if ! command -v doxygen &> /dev/null; then
    echo "Error: Doxygen is not installed."
    echo "Install it with: sudo apt install doxygen graphviz"
    exit 1
fi

# Generate documentation
doxygen Doxyfile

echo ""
echo "Documentation generated successfully!"
echo "Open docs/html/index.html in your browser to view it."
echo ""
echo "To view now, run:"
echo "  xdg-open docs/html/index.html"
