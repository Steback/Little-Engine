#ifndef LITTLEVULKANENGINE_WINDOW_HPP
#define LITTLEVULKANENGINE_WINDOW_HPP


#include <string>

#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"

#include "utils/NonCopyable.hpp"


namespace lve {

    class InputSystem;

    class Window : NonCopyable {
    public:
        struct Size {
            int width;
            int height;
        };

    public:
        Window(std::string  name, int width, int height, InputSystem& input);

        ~Window() override;

        void destroy();

        [[nodiscard]] const std::string &getName() const;

        void setName(const std::string &name_);

        [[nodiscard]] const Size &getSize() const;

        void setSize(const Size &size_);

        bool isOpen();

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

        [[nodiscard]] VkExtent2D getExtent() const;

        [[nodiscard]] bool wasWindowResized() const;

        void resetWindowResizedFlag();

    public:
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    private:
        GLFWwindow* window{};
        std::string name;
        Size size{};
        bool framebufferResized{false};
        InputSystem& input;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_WINDOW_HPP
