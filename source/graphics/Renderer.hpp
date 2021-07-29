#ifndef LITTLEVULKANENGINE_RENDERER_HPP
#define LITTLEVULKANENGINE_RENDERER_HPP


#include <memory>

#include "utils/NonCopyable.hpp"


namespace lve {

    class Window;
    class Config;
    class Instance;

    class Renderer : NonCopyable {
    public:
        explicit Renderer(std::shared_ptr<Window> window, const Config& config);

        ~Renderer() override;

    private:
        std::shared_ptr<Window> window;
        std::shared_ptr<Instance> instance;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_RENDERER_HPP
