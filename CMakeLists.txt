cmake_minimum_required(VERSION 3.16)
project(FibonacciHeapGUI VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

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

# Link SFML libraries
target_link_libraries(FibonacciHeapGUI sfml-graphics sfml-window sfml-system)

# Set output directory
set_target_properties(FibonacciHeapGUI PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Installation rules
install(TARGETS FibonacciHeapGUI DESTINATION bin)
