# Build and Run Guide

This guide provides detailed instructions for building and running the Fibonacci Heap GUI application.

## Prerequisites

### Required Software
- **C++ Compiler**: g++ 7.0 or newer (with C++17 support)
- **CMake**: Version 3.16 or newer
- **SFML**: Version 2.5 or newer

### Platform-Specific Setup

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libsfml-dev
```

#### Fedora/RHEL
```bash
sudo dnf install gcc-c++ cmake SFML-devel
```

#### Arch Linux
```bash
sudo pacman -S base-devel cmake sfml
```

#### macOS
```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake sfml
```

#### Windows
1. Install [CMake](https://cmake.org/download/)
2. Install [MinGW-w64](https://www.mingw-w64.org/) or [Visual Studio](https://visualstudio.microsoft.com/)
3. Download and install [SFML](https://www.sfml-dev.org/download.php)
4. Set SFML_ROOT environment variable to your SFML installation path

## Building the Project

### Step 1: Clone or Navigate to Repository
```bash
cd /path/to/FibonacciHeap
```

### Step 2: Create Build Directory
```bash
mkdir -p build
cd build
```

### Step 3: Configure with CMake
```bash
cmake ..
```

If SFML is not found automatically, specify the path:
```bash
cmake .. -DSFML_ROOT=/path/to/sfml
```

### Step 4: Build
```bash
make -j$(nproc)  # Linux/macOS
# OR
cmake --build . --config Release  # Cross-platform
```

The executable will be created in `build/bin/FibonacciHeapGUI`

## Running the Application

### GUI Application
```bash
cd build/bin
./FibonacciHeapGUI
```

### Example Programs

#### Basic Tests
```bash
cd examples
g++ -std=c++17 -I../include basic_test.cpp -o basic_test
./basic_test
```

#### Advanced Examples
```bash
cd examples
g++ -std=c++17 -I../include advanced_example.cpp -o advanced_example
./advanced_example
```

## Using the GUI

### Interface Overview
- **Input Box**: Enter integer values (positive or negative)
- **Insert Button**: Add the entered value to the heap
- **Extract Min Button**: Remove and display the minimum value
- **Reset Button**: Clear the entire heap

### Node Colors
- **Gold**: Minimum node (smallest value in heap)
- **Light Green**: Root nodes in the forest
- **Orange**: Marked nodes (have lost a child)
- **Light Blue**: Regular child nodes

### Tips
- Press Enter after typing a number to quickly insert
- The heap automatically reorganizes after each operation
- Watch how cascading cuts work when extracting minimum
- The status bar at the top shows operation results

## Troubleshooting

### Font Loading Issues
If you see "Warning: Could not load any font", the application will still work but text may not display. To fix:

1. **Linux**: Install DejaVu fonts
   ```bash
   sudo apt-get install fonts-dejavu
   ```

2. **macOS/Windows**: The default system fonts should work automatically

### SFML Not Found
If CMake cannot find SFML:
```bash
# Set SFML directory
export SFML_ROOT=/usr/local
cmake .. -DSFML_ROOT=$SFML_ROOT
```

### Display Issues (Linux Headless)
If running on a server without X11:
```bash
# Install virtual display
sudo apt-get install xvfb

# Run with virtual display
Xvfb :99 -screen 0 1200x800x24 &
export DISPLAY=:99
./FibonacciHeapGUI
```

### Build Errors
1. **"C++17 required"**: Update your compiler
   ```bash
   g++ --version  # Should be 7.0 or higher
   ```

2. **"SFML not found"**: Ensure SFML is properly installed
   ```bash
   pkg-config --modversion sfml-all  # Check SFML version
   ```

## Performance Notes

- The heap can efficiently handle thousands of nodes
- Insert operation: O(1) amortized time
- Extract Min: O(log n) amortized time
- Decrease Key: O(1) amortized time
- The GUI may slow down with very large heaps (>500 nodes) due to rendering

## Additional Resources

- [SFML Documentation](https://www.sfml-dev.org/documentation/)
- [Fibonacci Heap Wikipedia](https://en.wikipedia.org/wiki/Fibonacci_heap)
- [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/)

## Support

For issues or questions:
1. Check the troubleshooting section above
2. Review the README.md for algorithm details
3. Examine the example programs for usage patterns
