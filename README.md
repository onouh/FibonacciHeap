# Fibonacci Heap with GUI Visualization

This repository contains a complete C++ implementation of a Fibonacci Heap data structure with a graphical user interface for visualization.

## Features

### Backend (Fibonacci Heap)
- **Templated implementation** supporting any comparable type
- **Core operations**:
  - `insert(key)`: Insert a new node - O(1) amortized
  - `getMin()`: Return the minimum value - O(1)
  - `extractMin()`: Remove and return minimum node - O(log n) amortized
  - `decreaseKey(node, newKey)`: Decrease node value with cascading cuts - O(1) amortized
  - `deleteNode(node)`: Delete a specific node - O(log n) amortized
  - `merge(otherHeap)`: Merge two heaps - O(1)
- **Proper memory management** with no memory leaks
- **Cascading cut logic** for maintaining heap properties

### Frontend (GUI)
- Built with **SFML** (Simple and Fast Multimedia Library)
- **Interactive controls**:
  - Input field for entering values
  - Insert button to add values to the heap
  - Extract Min button to remove the minimum value
  - Reset button to clear the heap
- **Visual representation**:
  - Nodes displayed as circles with values
  - Color coding:
    - Gold: Minimum node
    - Light green: Root nodes
    - Orange: Marked nodes (for cascading cuts)
    - Light blue: Regular nodes
  - Lines showing parent-child relationships
  - Horizontal layout of root list (forest structure)

## Project Structure

```
FibonacciHeap/
├── include/
│   ├── FibonacciHeap.h      # Heap class declaration
│   ├── FibonacciHeap.tpp    # Heap template implementation
│   └── MainWindow.h         # GUI window class
├── src/
│   ├── main.cpp             # Application entry point
│   └── MainWindow.cpp       # GUI implementation
├── CMakeLists.txt           # Build configuration
└── README.md                # This file
```

## Requirements

- **C++17** or newer
- **CMake** 3.16 or newer
- **SFML** 2.5 or newer
- **g++** or compatible C++ compiler

## Building the Project

### Install Dependencies (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libsfml-dev
```

### Build Instructions

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
make

# Run the application
./bin/FibonacciHeapGUI
```

## Usage

1. **Insert values**: Type a number in the input box and click "Insert" (or press Enter)
2. **View the heap**: The visualization updates automatically after each operation
3. **Extract minimum**: Click "Extract Min" to remove the smallest value
4. **Reset**: Click "Reset" to clear the heap and start over

## Algorithm Details

### Fibonacci Heap Properties
- Collection of min-heap-ordered trees
- Root nodes stored in a circular doubly-linked list
- Each node contains:
  - Key value
  - Degree (number of children)
  - Marked flag (for cascading cuts)
  - Parent, child, left, and right pointers

### Cascading Cuts
When a node loses a child (during `decreaseKey`):
- If unmarked: mark the node
- If marked: cut the node and move to root list, then recursively check parent

### Consolidation
After `extractMin`:
- Trees of same degree are merged
- Maintains the logarithmic bound on tree heights
- Ensures O(log n) trees in the root list

## Testing

A test program is included to verify the heap operations:

```bash
# Compile test
g++ -std=c++17 -I./include demo_test.cpp -o demo_test

# Run test
./demo_test
```

The test program validates:
- Insert and extract operations
- Heap property maintenance
- Merge functionality
- Decrease key with cascading cuts

## License

This project is part of a Data Structures Course final project.

## Screenshots

![Fibonacci Heap GUI](https://github.com/user-attachments/assets/4d61005d-5060-4418-a49c-2df242a8259a)

*The GUI showing the initial state with controls for inserting values, extracting minimum, and resetting the heap.* 
