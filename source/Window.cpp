#include "Window.hpp"

#include "spdlog/spdlog.h"


Window::Window() = default;

Window::Window(const char *name, int width, int height) {
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

bool Window::open() {
    return !glfwWindowShouldClose(window);
}
