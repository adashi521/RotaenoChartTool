cmake_minimum_required(VERSION 4.0.0)

project(RotaenoChartTool)

# Specify the required C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")

# Source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# Add the executable
add_executable(RotaenoChartTool ${SOURCES})

# Include directories
target_include_directories(RotaenoChartTool PRIVATE "${CMAKE_SOURCE_DIR}/include")

# Specify the output directory for the executable
set_target_properties(RotaenoChartTool PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)