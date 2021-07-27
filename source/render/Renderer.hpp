#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_RENDERENGINE_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_RENDERENGINE_HPP


#include <memory>
#include <vector>

#include <vulkan/vulkan.hpp>


namespace lve {

    class Window;
    class Instance;
    class Device;
    class SwapChain;

    class Renderer {
    public:
        explicit Renderer(std::shared_ptr<Window> window);

        ~Renderer();

        void cleanup();

        vk::CommandBuffer beginFrame();

        void endFrame();

        [[nodiscard]] const std::shared_ptr<Device> &getDevice() const;

        [[nodiscard]] vk::CommandBuffer getCommandBuffer() const;

        void setupDrawResources();

        [[nodiscard]] vk::RenderPass getRenderPass() const;

        void beginSwapChainRenderPass(vk::CommandBuffer commandBuffer);

        void endSwapChainRenderPass(vk::CommandBuffer commandBuffer) const;

    private:
        void createCmdBuffers();

        void recreateDrawResources();

        void freeCommandBuffers();

    private:
        std::shared_ptr<Window> window{};

        std::shared_ptr<Instance> instance{};
        std::shared_ptr<Device> device{};

        std::unique_ptr<SwapChain> swapChain{};
        vk::SurfaceKHR surface{};

        vk::CommandPool commandPool;
        std::vector<vk::CommandBuffer> commandBuffers;

        uint32_t imageIndex{};
        uint32_t currentFrameIndex{0};
        bool isFrameStarted{false};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_RENDER_RENDERENGINE_HPP
