#ifndef LITTLEVULKANENGINE_WINDOW_HPP
#define LITTLEVULKANENGINE_WINDOW_HPP


#include <string>

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

        [[nodiscard]] const std::string &getName() const;

        void setName(const std::string &name_);

        [[nodiscard]] const Size &getSize() const;

        void setSize(const Size &size_);

        bool isOpen();

    private:
        GLFWwindow* window{};
        std::string name;
        Size size{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_WINDOW_HPP
