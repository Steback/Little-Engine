#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_RENDERENGINE_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_RENDERENGINE_HPP


#include <memory>

#include <vulkan/vulkan.hpp>


namespace lve {

    class Window;
    class Instance;
    class Device;
    class GraphicsPipeline;

    class RenderEngine {
    public:
        explicit RenderEngine(std::shared_ptr<Window> window);

        ~RenderEngine();

        void cleanup();

    private:
        std::shared_ptr<Window> window{};
        std::shared_ptr<Instance> instance{};
        std::shared_ptr<Device> device{};
        std::unique_ptr<GraphicsPipeline> graphicsPipeline{};
        vk::SurfaceKHR surface{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_RENDER_RENDERENGINE_HPP
