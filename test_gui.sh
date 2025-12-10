#!/bin/bash

# Start Xvfb on display :99
Xvfb :99 -screen 0 1200x800x24 &
XVFB_PID=$!
export DISPLAY=:99

# Wait for Xvfb to start
sleep 2

# Run the application in background
cd /home/runner/work/FibonacciHeap/FibonacciHeap/build/bin
./FibonacciHeapGUI &
APP_PID=$!

# Wait for the window to open
sleep 3

# Take initial screenshot
import -window root /tmp/fibonacci_heap_initial.png

# Give it some more time
sleep 2

# Kill the application
kill $APP_PID 2>/dev/null || true

# Kill Xvfb
kill $XVFB_PID 2>/dev/null || true

echo "Screenshot saved to /tmp/fibonacci_heap_initial.png"
