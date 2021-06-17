#include "Window.hpp"

#include "spdlog/spdlog.h"

#include "Constants.hpp"


Window::Window() = default;

Window::Window(const char *name, int width, int height) : name(name), width(width), height(height) {
    if (!glfwInit())
        spdlog::error("Failed to init GLFW");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(width, height, name, nullptr, nullptr);
}

Window::~Window() {
    if (window) destroy();
}

void Window::destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::isOpen() {
    return !glfwWindowShouldClose(window);
}

int Window::getWidth() const {
    return width;
}

int Window::getHeight() const {
    return height;
}

vk::SurfaceKHR Window::createSurface(const vk::Instance &instance) {
    VkSurfaceKHR surface;

    if (glfwCreateWindowSurface(instance, window, nullptr, &surface))
       THROW_EX(fmt::format("Failed to create surface to {} window", name))

    return vk::SurfaceKHR(surface);
}
