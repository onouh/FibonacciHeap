cmake_minimum_required(VERSION 3.16)
project(FibonacciHeapGUI VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable automatic MOC, UIC, and RCC for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# ============================================
# Main GUI Application (Fibonacci Heap Visualization)
# ============================================
set(MAIN_SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/AnimationSystem.cpp
    src/TypeSelector.cpp
)

set(MAIN_HEADERS
    include/FibonacciHeap.hpp
    include/MainWindow.h
    include/AnimationSystem.h
    include/TypeSelector.h
    include/HeapInterface.h
    include/HeapWrapper.h
)

# Create main executable
add_executable(FibonacciHeapGUI ${MAIN_SOURCES} ${MAIN_HEADERS})
target_link_libraries(FibonacciHeapGUI Qt6::Widgets)

# ============================================
# TaskManager Application (Emergency Care Management)
# ============================================
set(TASKMANAGER_SOURCES
    application/main.cpp
    application/AppWindow.cpp
    application/TaskManager.cpp
)

set(TASKMANAGER_HEADERS
    application/AppWindow.h
    application/TaskManager.h
    include/FibonacciHeap.hpp
)

# Create TaskManager executable
add_executable(TaskManagerGUI ${TASKMANAGER_SOURCES} ${TASKMANAGER_HEADERS})
target_include_directories(TaskManagerGUI PRIVATE ${CMAKE_SOURCE_DIR}/application)
target_link_libraries(TaskManagerGUI Qt6::Widgets)

# ============================================
# Output directories
# ============================================
set_target_properties(FibonacciHeapGUI TaskManagerGUI PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Installation rules
install(TARGETS FibonacciHeapGUI TaskManagerGUI DESTINATION bin)
