#ifndef LITTLEENGINE_WINDOW_HPP
#define LITTLEENGINE_WINDOW_HPP


#include <string>

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "vulkan/vulkan.hpp"


class Window {
public:
    Window();

    Window(const Window &) = delete;

    Window(const char* name, int width, int height);

    Window &operator=(const Window &) = delete;

    ~Window();

    void destroy();

    bool isOpen();

    [[nodiscard]] int getWidth() const;

    [[nodiscard]] int getHeight() const;

    vk::SurfaceKHR createSurface(const vk::Instance& instance);

private:
    GLFWwindow* window{};
    std::string name{};
    const int width{}, height{};
};


#endif //LITTLEENGINE_WINDOW_HPP
