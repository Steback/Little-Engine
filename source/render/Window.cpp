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
            EXIT_ERROR(fmt::format("Failed to create window {}", name));

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(size.width, size.height, this->name.c_str(), nullptr, nullptr);

        spdlog::info("Open window {}", this->name);
    }

    Window::~Window() = default;

    void Window::destroy() {
        if (window) {
            spdlog::info("Destroy window {}", this->name);

            glfwDestroyWindow(window);
        }

        glfwTerminate();
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(window);
    }

    VkSurfaceKHR Window::createWindowSurface(const VkInstance& instance) const {
        VkSurfaceKHR surface;

        VK_CHECK_RESULT(glfwCreateWindowSurface(instance, window, nullptr, &surface),
                        "Failed to create window surface!");

        return surface;
    }

    GLFWwindow *Window::getHandle() {
        return window;
    }

    Window::Size Window::getSize() const {
        return size;
    }

    VkExtent2D Window::getExtent() const {
        return {castU32(size.width), castU32(size.height)};
    }

} // namespace lve
