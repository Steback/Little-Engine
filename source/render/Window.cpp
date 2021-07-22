#include "Window.hpp"

#include "spdlog/spdlog.h"

#include "../tools/Tools.hpp"
#include "../logger/Logger.hpp"


namespace lve {

    float Window::Size::aspect() const {
        return static_cast<float>(width) / static_cast<float>(height);
    }

    Window::Window(int width, int height, const std::string &name) : size{width, height}, name{name} {
        if (!glfwInit())
            EXIT_ERROR(fmt::format("Failed to create handle {}", name));

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        handle = glfwCreateWindow(size.width, size.height, this->name.c_str(), nullptr, nullptr);

        glfwSetWindowUserPointer(handle, this);
        glfwSetFramebufferSizeCallback(handle, framebufferResizeCallback);

        spdlog::info("Open handle {}", this->name);
    }

    Window::~Window() = default;

    void Window::destroy() {
        if (handle) {
            spdlog::info("Destroy handle {}", this->name);

            glfwDestroyWindow(handle);
        }

        glfwTerminate();
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(handle);
    }

    VkSurfaceKHR Window::createWindowSurface(const VkInstance& instance) const {
        VkSurfaceKHR surface;

        VK_CHECK_RESULT(glfwCreateWindowSurface(instance, handle, nullptr, &surface),
                        "Failed to create handle surface!");

        return surface;
    }

    GLFWwindow *Window::getHandle() {
        return handle;
    }

    Window::Size Window::getSize() const {
        return size;
    }

    VkExtent2D Window::getExtent() const {
        return {castU32(size.width), castU32(size.height)};
    }

    bool Window::wasWindowResized() const {
        return framebufferResized;
    }

    void Window::resetWindowResizeFlag() {
        framebufferResized = false;
    }

    void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto* win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        win->framebufferResized = true;
        win->size = {width, height};
    }

} // namespace lve
