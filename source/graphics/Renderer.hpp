#ifndef LITTLEVULKANENGINE_RENDERER_HPP
#define LITTLEVULKANENGINE_RENDERER_HPP


#include <memory>


namespace lve {

    class Window;

    class Renderer {
    public:
        explicit Renderer(std::shared_ptr<Window> window);

        ~Renderer();

    private:
        std::shared_ptr<Window> window;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_RENDERER_HPP
