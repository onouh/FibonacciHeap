#!/bin/bash

# Qt Build Verification Script
# This script verifies that the Fibonacci Heap project can be built and run with Qt

set -e  # Exit on error

echo "======================================"
echo "Qt Build Verification Script"
echo "======================================"
echo ""

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print success
print_success() {
    echo -e "${GREEN}✓${NC} $1"
}

# Function to print error
print_error() {
    echo -e "${RED}✗${NC} $1"
}

# Function to print info
print_info() {
    echo -e "${YELLOW}ℹ${NC} $1"
}

# Check for required tools
echo "1. Checking for required tools..."
MISSING_TOOLS=()

if ! command -v cmake &> /dev/null; then
    MISSING_TOOLS+=("cmake")
fi

if ! command -v make &> /dev/null; then
    MISSING_TOOLS+=("make")
fi

if ! command -v g++ &> /dev/null; then
    MISSING_TOOLS+=("g++")
fi

if [ ${#MISSING_TOOLS[@]} -ne 0 ]; then
    print_error "Missing required tools: ${MISSING_TOOLS[*]}"
    echo ""
    echo "Install with: sudo apt-get install -y build-essential cmake"
    exit 1
fi

print_success "All build tools found (cmake, make, g++)"

# Check for Qt6
echo ""
echo "2. Checking for Qt6..."
if ! ldconfig -p | grep -q "libQt6Widgets.so"; then
    print_error "Qt6 Widgets not found"
    echo ""
    echo "Install with: sudo apt-get install -y qt6-base-dev"
    exit 1
fi

print_success "Qt6 libraries found"

# Check C++ compiler version
echo ""
echo "3. Checking C++ compiler..."
GCC_VERSION=$(g++ -dumpversion)
print_success "GCC version: $GCC_VERSION"

# Clean and create build directory
echo ""
echo "4. Setting up build directory..."
rm -rf build
mkdir -p build
print_success "Build directory created"

# Configure with CMake
echo ""
echo "5. Configuring project with CMake..."
cd build
if cmake .. > /tmp/cmake_output.txt 2>&1; then
    print_success "CMake configuration successful"
else
    print_error "CMake configuration failed"
    cat /tmp/cmake_output.txt
    exit 1
fi

# Build the project
echo ""
echo "6. Building the project..."
if make -j$(nproc) > /tmp/build_output.txt 2>&1; then
    print_success "Build completed successfully"
else
    print_error "Build failed"
    cat /tmp/build_output.txt
    exit 1
fi

# Check executables
echo ""
echo "7. Verifying executables..."

if [ -f "bin/FibonacciHeapGUI" ]; then
    SIZE=$(ls -lh bin/FibonacciHeapGUI | awk '{print $5}')
    print_success "FibonacciHeapGUI built successfully ($SIZE)"
else
    print_error "FibonacciHeapGUI not found"
    exit 1
fi

if [ -f "bin/TaskManagerGUI" ]; then
    SIZE=$(ls -lh bin/TaskManagerGUI | awk '{print $5}')
    print_success "TaskManagerGUI built successfully ($SIZE)"
else
    print_error "TaskManagerGUI not found"
    exit 1
fi

# Check Qt library linkage
echo ""
echo "8. Verifying Qt library linkage..."

if ldd bin/FibonacciHeapGUI | grep -q "libQt6Widgets.so"; then
    print_success "FibonacciHeapGUI properly linked with Qt6"
else
    print_error "FibonacciHeapGUI not properly linked with Qt6"
    exit 1
fi

if ldd bin/TaskManagerGUI | grep -q "libQt6Widgets.so"; then
    print_success "TaskManagerGUI properly linked with Qt6"
else
    print_error "TaskManagerGUI not properly linked with Qt6"
    exit 1
fi

# Summary
echo ""
echo "======================================"
echo -e "${GREEN}✓ VERIFICATION COMPLETE${NC}"
echo "======================================"
echo ""
echo "Both Qt applications built successfully:"
echo "  • FibonacciHeapGUI - Fibonacci Heap Visualization"
echo "  • TaskManagerGUI - Emergency Care Management"
echo ""
echo "Executables location: build/bin/"
echo ""
echo "To run:"
echo "  ./build/bin/FibonacciHeapGUI"
echo "  ./build/bin/TaskManagerGUI"
echo ""
print_info "Note: These are GUI applications that require a display."
print_info "For headless environments, use Xvfb:"
echo "  Xvfb :99 -screen 0 1200x800x24 &"
echo "  export DISPLAY=:99"
echo "  ./build/bin/FibonacciHeapGUI"
echo ""
