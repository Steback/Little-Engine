#ifndef LITTLEVULKANENGINE_WINDOW_HPP
#define LITTLEVULKANENGINE_WINDOW_HPP


#include <string>

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"


namespace lve {

    class Window {
    public:
        struct Size {
            int width;
            int height;

            [[nodiscard]] float aspect() const;
        };

    public:
        Window(int width, int height, const std::string& name);

        Window(const Window&) = delete;

        ~Window();

        Window& operator=(const Window&) = delete;

        void destroy();

        [[nodiscard]] bool shouldClose() const;

        [[nodiscard]] VkSurfaceKHR createWindowSurface(const VkInstance& instance) const;

        GLFWwindow* getHandle();

        [[nodiscard]] Size getSize() const;

        [[nodiscard]] VkExtent2D getExtent() const;

    private:
        GLFWwindow* window{};
        std::string name{};
        Size size{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_WINDOW_HPP
