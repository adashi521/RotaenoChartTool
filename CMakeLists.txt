cmake_minimum_required(VERSION 3.0.0)

project(RotaenoChartTool)

# Specify the required C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add the executable
add_executable(RotaenoChartTool ${SOURCES})

# Specify the output directory for the executable
set_target_properties(RotaenoChartTool PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
