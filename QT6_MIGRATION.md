# Qt 6 Migration Guide

## Overview
This document describes the migration from SFML to Qt 6 Widgets for the Fibonacci Heap GUI visualization.

## Changes Made

### 1. Build System (CMakeLists.txt)
- **Removed**: SFML dependency (`find_package(SFML 2.5 ...)`)
- **Added**: Qt 6 dependency (`find_package(Qt6 REQUIRED COMPONENTS Widgets)`)
- **Added**: Qt automation flags for MOC, UIC, and RCC
- **Updated**: Link against `Qt6::Widgets` instead of SFML libraries

### 2. GUI Framework

#### MainWindow.h
**Before (SFML)**:
- Used `sf::RenderWindow` for the main window
- Custom event handling with `sf::Event`
- Manual font loading and text rendering
- Manual button drawing with `sf::RectangleShape`

**After (Qt 6)**:
- Inherits from `QMainWindow` for main window
- Created separate `HeapCanvas` class inheriting from `QWidget` for visualization
- Uses Qt's native widgets: `QPushButton`, `QLineEdit`, `QLabel`
- Uses Qt's signal-slot mechanism for event handling
- Qt handles all widget rendering automatically

#### MainWindow.cpp
**Before (SFML)**:
- Manual event loop with `window.pollEvent()`
- Custom rendering logic for all UI elements
- Manual button click detection
- Custom text input handling

**After (Qt 6)**:
- Qt event loop with `app.exec()`
- Uses Qt layouts (`QVBoxLayout`, `QHBoxLayout`) for UI organization
- Automatic button event handling with `clicked` signals
- Native text input with `QLineEdit` widget
- Custom painting only for heap visualization in `HeapCanvas::paintEvent()`

#### main.cpp
**Before (SFML)**:
```cpp
MainWindow window;
window.run();  // Custom event loop
```

**After (Qt 6)**:
```cpp
QApplication app(argc, argv);
MainWindow window;
window.show();
return app.exec();  // Qt event loop
```

### 3. Visualization Canvas

Created a dedicated `HeapCanvas` class that:
- Inherits from `QWidget`
- Overrides `paintEvent()` for custom drawing
- Uses `QPainter` for rendering nodes and connections
- Maintains reference to the `FibonacciHeap<int>` instance

### 4. Drawing API Changes

| Operation | SFML | Qt 6 |
|-----------|------|------|
| Circle | `sf::CircleShape` | `QPainter::drawEllipse()` |
| Line | `sf::Vertex[]` | `QPainter::drawLine()` |
| Text | `sf::Text` | `QPainter::drawText()` |
| Color | `sf::Color` | `QColor` |
| Position | `sf::Vector2f` | `QPointF` |

## Building with Qt 6

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt-get install qt6-base-dev

# Fedora
sudo dnf install qt6-qtbase-devel

# macOS
brew install qt@6

# Windows
# Download Qt installer from https://www.qt.io/download
```

### Build Commands
```bash
mkdir build
cd build
cmake ..
make
./bin/FibonacciHeapGUI
```

## Features Preserved

All original functionality has been maintained:
- ✅ Insert values into heap
- ✅ Extract minimum value
- ✅ Reset heap
- ✅ Real-time visualization
- ✅ Color-coded nodes (minimum, marked, root, regular)
- ✅ Parent-child relationship lines
- ✅ Forest structure (horizontal root layout)

## Benefits of Qt 6 Migration

1. **Native Look and Feel**: Qt provides platform-native UI components
2. **Better Input Handling**: Built-in text field validation and event handling
3. **Easier Maintenance**: Less custom code for UI elements
4. **Better Documentation**: Qt has extensive documentation and community support
5. **More Features**: Easy to extend with Qt's rich widget library
6. **Cross-Platform**: Better cross-platform support out of the box

## Code Quality

The implementation follows Qt best practices:
- Uses Qt's object hierarchy for automatic memory management
- Implements proper signal-slot connections
- Separates visualization logic (HeapCanvas) from UI logic (MainWindow)
- Uses Qt layouts for responsive design

## Testing

The implementation has been tested with:
- Basic insert/extract operations
- Empty heap handling
- Multiple nodes visualization
- UI responsiveness

## Future Enhancements

With Qt 6, it's now easier to add:
- Menu bar with File, Edit, View options
- Tool bar with quick actions
- Status bar with detailed information
- Dialogs for decrease-key and delete operations
- Export to image functionality
- Animation during operations
- Undo/Redo functionality
- Zoom and pan controls
