cmake_minimum_required(VERSION 3.18)
project(LittleVulkanEngine)

set(CMAKE_CXX_STANDARD 17)

# Binary output
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

# Conan
set(CONAN_DISABLE_CHECK_COMPILER OFF)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS)

# Source code and link
add_subdirectory(source)