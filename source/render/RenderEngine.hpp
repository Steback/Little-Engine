#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_RENDERENGINE_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_RENDERENGINE_HPP


#include <memory>
#include <vector>

#include <vulkan/vulkan.hpp>


namespace lve {

    class Window;
    class Instance;
    class Device;
    class GraphicsPipeline;
    class SwapChain;

    class RenderEngine {
    public:
        explicit RenderEngine(std::shared_ptr<Window> window);

        ~RenderEngine();

        void cleanup();

        void draw();

    private:
        void createPipelines();

        void createCmdBuffers();

    private:
        std::shared_ptr<Window> window{};
        std::shared_ptr<Instance> instance{};
        std::shared_ptr<Device> device{};
        std::unique_ptr<GraphicsPipeline> graphicsPipeline{};
        std::unique_ptr<SwapChain> swapChain{};
        vk::SurfaceKHR surface{};
        uint32_t indexImage{};
        vk::CommandPool graphicsCmdPool;
        std::vector<vk::CommandBuffer> graphicsCmdBuffers;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_RENDER_RENDERENGINE_HPP
