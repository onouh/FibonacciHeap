# Running Both GUI Applications

This repository contains **two complete GUI applications** that demonstrate the Fibonacci Heap data structure:

## 1. FibonacciHeapGUI - Interactive Visualization

The main visualization application with animated heap operations.

### Features
- Visual representation of Fibonacci Heap structure
- Interactive node selection
- Animated operations (Insert, Extract Min, Decrease Key, etc.)
- Animation speed controls
- Real-time heap visualization

### How to Run
```bash
cd build/bin
./FibonacciHeapGUI
```

### Screenshot
![Fibonacci Heap Visualization](https://github.com/user-attachments/assets/38915855-76d1-40fc-ae8c-50c442d6bddd)

## 2. TaskManagerGUI - Emergency Care Management System

A practical application using Fibonacci Heap for priority queue management in a medical emergency care setting.

### Features
- Priority-based patient queue management
- Four urgency levels: CRITICAL, URGENT, MODERATE, MINOR
- Add new patients with priority levels
- View most urgent patient
- Treat next patient (extract min operation)
- Update patient priority (decrease key operation)
- Complete/remove specific patients
- Real-time clock display
- Color-coded priority visualization

### How to Run
```bash
cd build/bin
./TaskManagerGUI
```

### Screenshot
![Emergency Care Management](https://github.com/user-attachments/assets/b53e3352-3375-47e5-b4a3-c0249f6064bb)

## Building Both Applications

### Prerequisites
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake qt6-base-dev
```

### Build Steps
```bash
# From repository root
mkdir -p build
cd build
cmake ..
make -j4
```

This will create both executables in `build/bin/`:
- `FibonacciHeapGUI` - Main visualization GUI
- `TaskManagerGUI` - Emergency care management GUI

## Key Differences

| Feature | FibonacciHeapGUI | TaskManagerGUI |
|---------|------------------|----------------|
| Purpose | Educational visualization | Practical application |
| Data Type | Integers | String (patient names) |
| Focus | Algorithm visualization | Priority queue management |
| Interface | Canvas-based drawing | List-based task view |
| User Interaction | Node clicking, animations | Form inputs, buttons |
| Use Case | Learning data structures | Emergency room triage |

## Architecture

Both applications share the same core **FibonacciHeap** template implementation (`include/FibonacciHeap.hpp`), demonstrating the reusability and versatility of the data structure for different use cases.

### Shared Components
- `include/FibonacciHeap.hpp` - Template heap implementation
- `include/FibonacciHeap.tpp` - Template implementation details
- Qt 6 framework for GUI

### Application-Specific Components

**FibonacciHeapGUI:**
- `src/MainWindow.cpp` - Visualization window
- `src/AnimationSystem.cpp` - Animation framework
- `include/MainWindow.h` - Main window interface

**TaskManagerGUI:**
- `application/AppWindow.cpp` - Management interface
- `application/TaskManager.cpp` - Queue management logic
- `application/TaskManager.h` - Priority queue wrapper

## Testing

Both applications have been verified to:
- ✅ Compile without errors or warnings
- ✅ Run successfully with Qt6
- ✅ Handle all Fibonacci Heap operations correctly
- ✅ Provide intuitive user interfaces
- ✅ Display proper visual feedback

## Use Cases

### FibonacciHeapGUI
- Computer science education
- Algorithm visualization
- Understanding heap operations
- Demonstrating time complexity

### TaskManagerGUI
- Hospital emergency room triage
- Task priority management
- Job scheduling systems
- Any priority queue application

## Notes

- Both applications use the same underlying Fibonacci Heap implementation
- The heap efficiently maintains O(1) insert and O(log n) extract-min operations
- Priority levels in TaskManager use integer keys (1=highest priority)
- The applications demonstrate both educational and practical uses of the data structure
