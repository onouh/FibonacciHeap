#!/bin/bash

# Verification script for both GUI applications

echo "==================================================="
echo "Verification Script for Both GUI Applications"
echo "==================================================="
echo ""

# Check if build directory exists
if [ ! -d "build" ]; then
    echo "❌ Build directory not found. Please run: mkdir build && cd build && cmake .. && make"
    exit 1
fi

# Check if executables exist
echo "Checking executables..."
if [ -f "build/bin/FibonacciHeapGUI" ]; then
    echo "✅ FibonacciHeapGUI found"
else
    echo "❌ FibonacciHeapGUI not found"
    exit 1
fi

if [ -f "build/bin/TaskManagerGUI" ]; then
    echo "✅ TaskManagerGUI found"
else
    echo "❌ TaskManagerGUI not found"
    exit 1
fi

echo ""
echo "File sizes:"
ls -lh build/bin/FibonacciHeapGUI build/bin/TaskManagerGUI

echo ""
echo "==================================================="
echo "✅ Both GUI applications verified successfully!"
echo "==================================================="
echo ""
echo "To run the applications:"
echo ""
echo "1. FibonacciHeapGUI (Interactive Visualization):"
echo "   cd build/bin && ./FibonacciHeapGUI"
echo ""
echo "2. TaskManagerGUI (Emergency Care Management):"
echo "   cd build/bin && ./TaskManagerGUI"
echo ""
echo "See RUNNING_BOTH_GUIS.md for detailed instructions."
echo "==================================================="
