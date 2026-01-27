# Fibonacci Heap with Two Interactive GUI Applications

This repository contains a complete C++ implementation of a Fibonacci Heap data structure with **two fully functional GUI applications** for visualization and practical use.

## 🎯 Two Complete Applications

### 1. FibonacciHeapGUI - Interactive Visualization
**Fully interactive, animated GUI** for visualizing Fibonacci Heap operations and understanding the algorithm.

### 2. TaskManagerGUI - Emergency Care Management
**Practical application** using the Fibonacci Heap for priority-based patient queue management in emergency care settings.

> **See [RUNNING_BOTH_GUIS.md](RUNNING_BOTH_GUIS.md) for detailed instructions on running both applications.**

## ✨ Key Features

### Backend (Fibonacci Heap)
- **Templated implementation** supporting any comparable type
- **All standard operations**:
  - `insert(key)`: Insert a new node - O(1) amortized
  - `getMin()`: Return the minimum value - O(1)
  - `extractMin()`: Remove and return minimum node - O(log n) amortized
  - `decreaseKey(node, newKey)`: Decrease node value with cascading cuts - O(1) amortized
  - `deleteNode(node)`: Delete a specific node - O(log n) amortized
  - `merge(otherHeap)`: Merge two heaps - O(1)
- **Proper memory management** with no memory leaks
- **Cascading cut logic** for maintaining heap properties

### Frontend (Enhanced GUI)
- Built with **Qt 6** (Widgets module)
- **Interactive controls** for all operations:
  - **Insert**: Input field with instant visualization
  - **Find Min**: Highlights minimum node in yellow
  - **Extract Min**: **Animated** removal and consolidation
  - **Decrease Key**: **Animated** cuts and cascading cuts with node selection
  - **Delete Node**: Click-to-select node deletion
  - **Union**: Merge with another heap
  - **Reset**: Clear the entire heap
  
- **Animation System**:
  - Smooth transitions between states
  - Speed control (1-3: Slow, Normal, Fast)
  - Pause/Resume controls
  - Real-time visual feedback
  
- **Enhanced Visualization**:
  - **Larger nodes** (30px radius) for better visibility
  - **Interactive node selection** (click any node)
  - **Pointer visualization**: Dashed lines show sibling connections
  - **Arrow-based** parent-child relationships
  - **Color coding**:
    - 🟡 Gold: Minimum node
    - 🟡 Yellow: Highlighted node (Find Min)
    - 🟢 Light green: Root nodes
    - 🟠 Orange: Marked nodes (lost a child)
    - 🔵 Light blue: Regular child nodes
    - 🔴 Red border: Selected node
  - **Tooltips**: Hover over nodes to see details (key, value, degree, marked status)
  - **Smart layout**: Non-overlapping nodes with adequate spacing
  
- **User Interface**:
  - Organized control panel with grouped operations
  - Real-time status bar with operation feedback
  - Info bar showing heap size and minimum value
  - Visual legend for color meanings
  - Professional styling with color-coded buttons

## 🎯 What's New in Enhanced Version

1. **Full Interactivity**: Click to select nodes for Decrease Key and Delete operations
2. **Smooth Animations**: Watch Extract Min and Decrease Key operations step-by-step
3. **Animation Controls**: Adjust speed and pause/resume animations
4. **Enhanced Visualization**: See pointer relationships and better node layout
5. **All Operations**: Complete set of Fibonacci Heap operations accessible via GUI
6. **Professional UI**: Polished interface with intuitive controls and feedback

## Project Structure

```
FibonacciHeap/
├── include/
│   ├── FibonacciHeap.hpp      # Heap class declaration and implementation
│   ├── MainWindow.h           # Enhanced GUI window class
│   ├── AnimationSystem.h      # Animation framework
│   ├── TypeSelector.h         # Type selection dialog
│   └── Vector.hpp             # Custom vector implementation
├── src/
│   ├── main.cpp               # Application entry point
│   ├── MainWindow.cpp         # Enhanced GUI implementation
│   ├── AnimationSystem.cpp    # Animation system implementation
│   └── TypeSelector.cpp       # Type selector implementation
├── CMakeLists.txt             # Build configuration
├── FEATURES.md                # Detailed feature documentation
├── README.md                  # This file
└── test_enhanced_gui.sh       # Feature validation test script
```

## Requirements

- **C++17** or newer
- **CMake** 3.16 or newer
- **Qt 6** (Widgets module)
- **g++** or compatible C++ compiler

## Building the Project

### Install Dependencies (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake qt6-base-dev
```

### Build Instructions

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build both applications
make -j4

# Run the visualization GUI
./bin/FibonacciHeapGUI

# Or run the task manager GUI
./bin/TaskManagerGUI
```

Both executables will be created in `build/bin/`:
- **FibonacciHeapGUI** - Interactive visualization
- **TaskManagerGUI** - Emergency care management

### Testing the Enhanced Features

```bash
# Run the feature validation test
./test_enhanced_gui.sh
```

This test script validates:
- ✓ All GUI controls are present
- ✓ Animation system is integrated
- ✓ Interactive features are implemented
- ✓ Enhanced visualization is active

## Usage

### Running the Application

1. **Insert values**: Type an integer in the input box and click "Insert" (or press Enter)
2. **Find Min**: Click "Find Min" to highlight the minimum node in yellow
3. **Extract Min**: Click "Extract Min" to watch an animated removal and consolidation
4. **Decrease Key**: 
   - Click on any node to select it (red border appears)
   - Enter a new key value (must be less than current)
   - Click "Decrease Key" to watch animated cuts
5. **Delete Node**:
   - Click on any node to select it
   - Click "Delete Selected Node" to remove it
6. **Union**: Click "Union with New Heap" to merge with a new heap
7. **Reset**: Click "Reset Heap" to clear and start over

### Animation Controls

- **Speed Slider**: Adjust animation speed (1=Slow, 2=Normal, 3=Fast)
- **Pause/Resume**: Control animation playback during operations

### Visual Guide

- **Hover** over nodes to see tooltips with detailed information
- **Click** nodes to select them for operations
- **Watch** for color changes indicating:
  - Yellow highlighting during Find Min
  - Orange marking during cascading cuts
  - Red selection ring when node is selected
- **Follow** the dashed lines showing sibling connections
- **Observe** solid arrows showing parent-child relationships

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

### FibonacciHeapGUI - Interactive Visualization
![WhatsApp Image 2026-01-27 at 10 30 07 AM](https://github.com/user-attachments/assets/418605fd-a57c-4b32-abf6-1799de829a8f)

![Fibonacci Heap GUI](https://github.com/user-attachments/assets/38915855-76d1-40fc-ae8c-50c442d6bddd)

*The visualization GUI showing the heap structure with interactive controls for all operations, animation controls, and real-time visual feedback.*

### TaskManagerGUI - Emergency Care Management
![WhatsApp Image 2026-01-27 at 10 29 37 AM](https://github.com/user-attachments/assets/3049fc0d-db7f-44ab-ac53-34bada8239f3)

![WhatsApp Image 2026-01-27 at 10 29 20 AM](https://github.com/user-attachments/assets/f9a7fdda-f192-4cd8-8c35-35450209a095)

*The emergency care management application showing priority-based patient queue with color-coded urgency levels and

Helpful Source:
  
  [Fibonacci Heap](https://www.youtube.com/watch?v=6JxvKfSV9Ns&list=PPSV)
 
