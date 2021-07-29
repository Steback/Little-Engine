#ifndef LITTLEVULKANENGINE_RENDERER_HPP
#define LITTLEVULKANENGINE_RENDERER_HPP


#include <memory>


namespace lve {

    class Window;
    class Config;
    class Instance;

    class Renderer {
    public:
        explicit Renderer(std::shared_ptr<Window> window, const Config& config);

        ~Renderer();

    private:
        std::shared_ptr<Window> window;
        std::shared_ptr<Instance> instance;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_RENDERER_HPP
