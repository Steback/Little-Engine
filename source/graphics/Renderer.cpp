#include "Renderer.hpp"
#include "Instance.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"
#include "config/Config.hpp"
#include "window/Window.hpp"


namespace lve {

    Renderer::Renderer(std::shared_ptr<Window> window, const Config& config) : window(std::move(window)) {
        std::vector<const char *> validationLayers;

#ifdef LVE_DEBUG
        validationLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

        instance = std::make_shared<Instance>(validationLayers, config.getAppName().c_str());

        VkPhysicalDeviceFeatures features{};
        std::vector<const char*> extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        device = std::make_shared<Device>(instance, this->window, validationLayers, features, extensions);
        logicalDevice = device->getDevice();
        device->createCommandPool(commandPool, device->getQueueFamilyIndices().graphics);
    }

    Renderer::~Renderer() = default;

    void Renderer::cleanup() {
        freeCommandBuffers();
        vkDestroyCommandPool(logicalDevice, commandPool, nullptr);

        swapChain->destroy();
        device->destroy();
        vkDestroySurfaceKHR(instance->getHandle(), device->getSurface(), nullptr);
        instance->destroy();
    }

    void Renderer::setupDrawResources() {
        swapChain = std::make_unique<SwapChain>(device, window->getExtent());
        createCommandBuffers();
    }

    VkRenderPass Renderer::getRenderPass() const {
        return swapChain->getRenderPass();
    }

    bool Renderer::isFrameInProgress() const {
        return isFrameStarted;
    }

    VkCommandBuffer Renderer::getCommandBuffer() const {
        assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
        return commandBuffers[currentFrameIndex];
    }

    int Renderer::getFrameIndex() const {
        assert(isFrameStarted && "Cannot get frame index when frame not in progress");
        return currentFrameIndex;
    }

    VkCommandBuffer Renderer::beginFrame() {
        assert(!isFrameStarted && "Can't call beginFrame while already in progress");

        auto result = swapChain->acquireNextImage(&imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            LVE_THROW_EX("Failed to acquire swap chain image!");

        isFrameStarted = true;

        auto commandBuffer = getCommandBuffer();
        VkCommandBufferBeginInfo beginInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};

        LVE_VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &beginInfo),
                            "Failed to begin recording command buffer!");

        return commandBuffer;
    }

    void Renderer::endFrame() {
        assert(isFrameStarted && "Can't call endFrame while frame is not in progress");

        auto commandBuffer = getCommandBuffer();

        LVE_VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer), "Failed to record command buffer!");

        auto result = swapChain->submitCommandBuffers(commandBuffer, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window->wasWindowResized()) {
            window->resetWindowResizedFlag();
            recreateSwapChain();
            isFrameStarted = false;
            currentFrameIndex = (currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
            return;
        } else if (result != VK_SUCCESS) {
            LVE_THROW_EX("Failed to present swap chain image!");
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void Renderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
        assert(commandBuffer == getCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

        VkRenderPassBeginInfo renderPassInfo{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
        renderPassInfo.renderPass = swapChain->getRenderPass();
        renderPassInfo.framebuffer = swapChain->getFrameBuffer(imageIndex);
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain->getExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = CAST_FLOAT(swapChain->getExtent().width);
        viewport.height = CAST_FLOAT(swapChain->getExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, swapChain->getExtent()};

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) const {
        assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
        assert(commandBuffer == getCommandBuffer() && "Can't end render pass on command buffer from a different frame");

        vkCmdEndRenderPass(commandBuffer);
    }

    float Renderer::getAspectRatio() const {
        return swapChain->extentAspectRatio();
    }

    void Renderer::createCommandBuffers() {
        commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = CAST_U32(commandBuffers.size());

        LVE_VK_CHECK_RESULT(vkAllocateCommandBuffers(device->getDevice(), &allocInfo, commandBuffers.data()),
                            "Failed to allocate command buffers!");
    }

    void Renderer::freeCommandBuffers() {
        vkFreeCommandBuffers(
                logicalDevice,
                commandPool,
                CAST_U32(commandBuffers.size()),
                commandBuffers.data());
        commandBuffers.clear();
    }

    void Renderer::recreateSwapChain() {
        auto extent = window->getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = window->getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(logicalDevice);

        std::shared_ptr<SwapChain> oldSwapChain = std::move(swapChain);
        swapChain = std::make_unique<SwapChain>(device, extent, oldSwapChain);

        if (!oldSwapChain->compareFormats(*swapChain)) {
            LVE_THROW_EX("Swap chain image(or depth) format has changed!");
        }

        oldSwapChain->destroy();
    }

    void Renderer::waitDeviceIde() {
        vkDeviceWaitIdle(logicalDevice);
    }

    std::shared_ptr<Device> Renderer::getDevice() {
        return device;
    }

} // namespace lv