cmake_minimum_required(VERSION 3.16)

project(MyEmergencyTriage VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Quick)

# Standard Qt project setup
qt_standard_project_setup(REQUIRES 6.8)

# Add the main executable target
qt_add_executable(appMyEmergencyTriage
    main.cpp
)

# Add the Qt QML module directly with the QML file
qt_add_qml_module(appMyEmergencyTriage
    URI MyEmergencyTriage  # QML Module URI
    QML_FILES
        Main.qml  # Add your QML file directly
    SOURCES
        TriageBridge.hpp TriageBridge.cpp
        FibonacciHeap.hpp FibonacciHeap.tpp
        Vector.hpp
        TaskManager.cpp
)

# MacOS/iOS bundle settings
set_target_properties(appMyEmergencyTriage PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MyEmergencyTriage
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# Link required libraries
target_link_libraries(appMyEmergencyTriage
    PRIVATE Qt6::Quick
)

# Installation settings
include(GNUInstallDirs)
install(TARGETS appMyEmergencyTriage
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
