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

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
)

# Header files
set(HEADERS
    include/FibonacciHeap.h
    include/FibonacciHeap.tpp
    include/MainWindow.h
)

# Create executable
add_executable(FibonacciHeapGUI ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(FibonacciHeapGUI Qt6::Widgets)

# Set output directory
set_target_properties(FibonacciHeapGUI PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Installation rules
install(TARGETS FibonacciHeapGUI DESTINATION bin)
