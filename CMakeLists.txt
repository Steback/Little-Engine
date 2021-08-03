cmake_minimum_required(VERSION 3.18)
project(LittleVulkanEngine)

set(CMAKE_CXX_STANDARD 17)

# Binary output
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Conan
set(CONAN_DISABLE_CHECK_COMPILER OFF)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS)

# Compile Shaders
set(COMPILE_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/tools/compileShaders.py ${CMAKE_CURRENT_SOURCE_DIR})
execute_process(COMMAND python ${COMPILE_SCRIPT})

# Source code and link
include_directories(${PROJECT_SOURCE_DIR}/source)
add_subdirectory(source)

# Samples
add_subdirectory(${PROJECT_SOURCE_DIR}/samples)