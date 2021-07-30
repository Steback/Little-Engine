#ifndef LITTLEVULKANENGINE_WINDOW_HPP
#define LITTLEVULKANENGINE_WINDOW_HPP


#include <string>

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "utils/NonCopyable.hpp"


namespace lve {

    class Window : NonCopyable {
    public:
        struct Size {
            int width;
            int height;
        };

    public:
        Window(std::string  name, int width, int height);

        ~Window() override;

        void destroy();

        [[nodiscard]] const std::string &getName() const;

        void setName(const std::string &name_);

        [[nodiscard]] const Size &getSize() const;

        void setSize(const Size &size_);

        bool isOpen();

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

        [[nodiscard]] VkExtent2D getExtent() const;

    private:
        GLFWwindow* window{};
        std::string name;
        Size size{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_WINDOW_HPP
