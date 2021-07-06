#include "Window.hpp"

#include "Tools.hpp"


namespace lve {

    float Window::Size::aspect() const {
        return static_cast<float>(width) / static_cast<float>(height);
    }

    Window::Window(int width, int height, const char *name) : size{width, height}, name{name} {
        if (!glfwInit())
            THROW_EX(fmt::format("Failed to create window {}", name));

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(size.width, size.height, this->name, nullptr, nullptr);
    }

    Window::~Window() {
        destroy();
    }

    void Window::destroy() {
        if (window) glfwDestroyWindow(window);

        glfwTerminate();
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(window);
    }

    GLFWwindow *Window::getHandle() {
        return window;
    }

    Window::Size Window::getSize() const {
        return size;
    }

} // namespace lve
