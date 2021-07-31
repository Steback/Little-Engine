#ifndef LITTLEVULKANENGINE_RENDERER_HPP
#define LITTLEVULKANENGINE_RENDERER_HPP


#include <memory>
#include <vector>

#include "vulkan/vulkan.h"

#include "utils/NonCopyable.hpp"


namespace lve {

    class Window;
    class Config;
    class Instance;
    class Device;
    class SwapChain;

    class Renderer : NonCopyable {
    public:
        explicit Renderer(std::shared_ptr<Window> window, const Config& config);

        ~Renderer() override;

        void cleanup();

        void setupDrawResources();

        [[nodiscard]] VkRenderPass getRenderPass() const;

        [[nodiscard]] bool isFrameInProgress() const;

        [[nodiscard]] VkCommandBuffer getCommandBuffer() const;

        [[nodiscard]] int getFrameIndex() const;

        VkCommandBuffer beginFrame();

        void endFrame();

        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);

        void endSwapChainRenderPass(VkCommandBuffer commandBuffer) const;

        void waitDeviceIde();

    private:
        void createCommandBuffers();

        void freeCommandBuffers();

        void recreateSwapChain();

    private:
        std::shared_ptr<Window> window;

        std::shared_ptr<Instance> instance;
        std::shared_ptr<Device> device;

        std::unique_ptr<SwapChain> swapChain;

        VkCommandPool commandPool{};
        std::vector<VkCommandBuffer> commandBuffers;

        VkSurfaceKHR surface{};

        uint32_t imageIndex{};
        int currentFrameIndex{0};
        bool isFrameStarted{false};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_RENDERER_HPP
