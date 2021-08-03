#include "Window.hpp"

#include "logs/Logs.hpp"
#include "input/InputSystem.hpp"


static void errorCallback(int error, const char* description) {
    LVE_LOG_ERROR(description);
}

namespace lve {

    Window::Window(std::string name, int width, int height, InputSystem& input)
                : name(std::move(name)), size({width, height}), input(input) {
        glfwSetErrorCallback(errorCallback);

        if (!glfwInit()) LVE_LOG_ERROR_EXIT("Failed to init GLFW");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, this->name.c_str(), nullptr, nullptr);

        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
        glfwSetKeyCallback(window, keyCallback);

        spdlog::info("Window created: {}", this->name);
    }

    Window::~Window() = default;

    void Window::destroy() {
        spdlog::info("Window deleted: {}", name);
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    const std::string &Window::getName() const {
        return name;
    }

    void Window::setName(const std::string &name_) {
        Window::name = name_;
    }

    const Window::Size &Window::getSize() const {
        return size;
    }

    void Window::setSize(const Window::Size &size_) {
        Window::size = size_;
    }

    bool Window::isOpen() {
        return !glfwWindowShouldClose(window);
    }

    void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        LVE_VK_CHECK_RESULT_EXIT(glfwCreateWindowSurface(instance, window, nullptr, surface),
                                 "Failed to create window surface")
    }

    VkExtent2D Window::getExtent() const {
        return {CAST_U32(size.width), CAST_U32(size.height)};
    }

    bool Window::wasWindowResized() const {
        return framebufferResized;
    }

    void Window::resetWindowResizedFlag() {
        framebufferResized = false;
    }

    void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        win->framebufferResized = true;
        win->size = {width, height};
    }

    void Window::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

        if (action == GLFW_PRESS) {
            win->input.setKey(key, MouseState::PRESS);
        } else if (action == GLFW_RELEASE) {
            win->input.setKey(key, MouseState::RELEASE);
        }
    }

} // namespace lv