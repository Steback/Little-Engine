#include "Renderer.hpp"

#include <array>

#include "spdlog/spdlog.h"

#include "Window.hpp"
#include "Instance.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"
#include "../tools/Tools.hpp"
#include "../fileManager/FileManager.hpp"


namespace lve {

    Renderer::Renderer(std::shared_ptr<Window> window) : window(std::move(window)) {
        std::vector<const char*> reqValidationLayers;

#ifdef LVE_DEBUG
        spdlog::info("Validations Layers: {}", "VK_LAYER_KHRONOS_validation");
        reqValidationLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

        instance = std::make_shared<Instance>(reqValidationLayers);
        surface = vk::SurfaceKHR(this->window->createWindowSurface(instance->getHandle()));
        device = std::make_shared<Device>(instance, reqValidationLayers, &surface);
        commandPool = device->createCommandPool(device->getQueueFamilyIndices().graphics);
    }

    Renderer::~Renderer() = default;

    void Renderer::cleanup() {
        freeCommandBuffers();
        device->getLogicalDevice().destroy(commandPool);
        swapChain->cleanup(device->getAllocator());
        instance->getHandle().destroy(surface);
        device->destroy();
        instance->destroy();
    }

    vk::CommandBuffer Renderer::beginFrame() {
        assert(!isFrameStarted && "Can't call beginFrame while already in progress");
        vk::Result result = swapChain->acquireNextImage(&imageIndex);

        if (result == vk::Result::eErrorOutOfDateKHR) {
            recreateDrawResources();
            return nullptr;
        }

        if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
            THROW_EX("Failed to acquire swap chain image!")

        isFrameStarted = true;
        auto commandBuffer = getCommandBuffer();

        vk::CommandBufferBeginInfo beginInfo{};
        commandBuffer.begin(beginInfo);

        return commandBuffer;
    }

    void Renderer::endFrame() {
        assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
        auto commandBuffer = getCommandBuffer();

        commandBuffer.end();

        vk::Result result = swapChain->submitCommandBuffer(commandBuffer, imageIndex);

        if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || window->wasWindowResized()) {
            window->resetWindowResizeFlag();
            recreateDrawResources();
        } else if (result != vk::Result::eSuccess) {
            THROW_EX("Failed to present swap chain handle!")
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    const std::shared_ptr<Device> &Renderer::getDevice() const {
        return device;
    }

    vk::CommandBuffer Renderer::getCommandBuffer() const {
        return commandBuffers[currentFrameIndex];
    }

    void Renderer::setupDrawResources() {
        swapChain = std::make_unique<SwapChain>(device, this->window->getExtent(), surface);
        createCmdBuffers();
    }

    vk::RenderPass Renderer::getRenderPass() const {
        return swapChain->getRenderPass();
    }

    void Renderer::beginSwapChainRenderPass(vk::CommandBuffer commandBuffer) {
        std::array<vk::ClearValue, 2> clearValues{};
        clearValues[0].color = {std::array<float, 4>{{0.01f, 0.01f, 0.01f, 1.0f}}};
        clearValues[1].depthStencil = {{1.0f, 0}};

        vk::RenderPassBeginInfo renderPassBeginInfo(
                swapChain->getRenderPass(),
                swapChain->getFrameBuffer(imageIndex),
                {{0, 0}, swapChain->getExtent()}, // renderArea
                clearValues
        );

        commandBuffer.beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);

        vk::Viewport viewport(
                0.0f, 0.0,
                static_cast<float>(swapChain->getExtent().width), static_cast<float>(swapChain->getExtent().height),
                0.0f, 1.0f
        );
        vk::Rect2D scissor({0, 0}, swapChain->getExtent());

        commandBuffer.setViewport(0, 1, &viewport);
        commandBuffer.setScissor(0, 1, &scissor);
    }

    void Renderer::endSwapChainRenderPass(vk::CommandBuffer commandBuffer) const {
        assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
        assert(commandBuffer == getCommandBuffer() && "Can't end render pass on command buffer from a different frame");
        commandBuffer.endRenderPass();
    }

    void Renderer::createCmdBuffers() {
        commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        vk::CommandBufferAllocateInfo allocateInfo(
                commandPool,
                vk::CommandBufferLevel::ePrimary,
                castU32(commandBuffers.size())
        );

        commandBuffers = device->getLogicalDevice().allocateCommandBuffers(allocateInfo);
    }

    void Renderer::recreateDrawResources() {
        auto extent = window->getExtent();

        while (extent.width == 0 || extent.height == 0) {
            extent = window->getExtent();
            glfwWaitEvents();
        }

        device->getLogicalDevice().waitIdle();

        std::shared_ptr<SwapChain> oldSwapChain = std::move(swapChain);
        swapChain = std::make_unique<SwapChain>(device, extent, surface, oldSwapChain);

        if (!oldSwapChain->compareFormats(*swapChain)) {
            THROW_EX("Swap chain image(or depth) format has changed!")
        }

        oldSwapChain->cleanup(device->getAllocator());
    }

    void Renderer::freeCommandBuffers() {
        device->getLogicalDevice().free(commandPool, commandBuffers);
        commandBuffers.clear();
    }

} // namespace lve
