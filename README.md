# Little Vulkan Engine
This project is an experimental game engine with Vulkan render. I use it for test ideas and new concepts about game engine development.

## Vulkan Game Engine Tutorial
I start this project following the Brendan Galea's tutorials series [Vulkan Game Engine Tutorial](https://www.youtube.com/c/BrendanGalea/featured) that is a very well tutorial for learn Vulkan and Computer Graphics.

## Third Party
All the dependencies are installed with [Conan](https://conan.io/) package manager with the [conanfile.txt](conanfile.txt)

| Name | Version |
---|---
| [spdlog]() | 1.8.2 | 
| [GLFW](https://www.glfw.org) | 3.3.2 |
| [Vulkan Headers](https://github.com/KhronosGroup/Vulkan-Headers) | 1.2.182 |  
| [Vulkan Loader](https://github.com/KhronosGroup/Vulkan-Loader) | 1.2.182 |  
| [Nlohmann json](https://github.com/nlohmann/json) | 3.9.1 |  
| [CLI11](https://github.com/CLIUtils/CLI11) | 1.9.1 |  
| [Vulkan® Memory Allocator](https://gpuopen.com/vulkan-memory-allocator) - [C++ Bindings](https://github.com/malte-v/VulkanMemoryAllocator-Hpp) | 2.3.0 |  

## Build
### Requirements
* CMake 3.18+
* Python 3+
* Conan 1.35.1+

### Conan
For install Conan and create the profile, there is the [Conan Getting Started](https://docs.conan.io/en/latest/getting_started.html).

### Linux
* GCC 11
```
mkdir build && cd build
conan install .. 
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

### Windows
* MSVC Compiler 16
```
mkdir build && cd build
conan install ..
cmake .. -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 16"
cmake --build . --config Release 
```
