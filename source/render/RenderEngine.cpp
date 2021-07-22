#include "RenderEngine.hpp"

#include <array>

#include "spdlog/spdlog.h"

#include "Window.hpp"
#include "Instance.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"
#include "pipeline/GraphicsPipeline.hpp"
#include "../tools/Tools.hpp"
#include "../fileManager/FileManager.hpp"


namespace lve {

    RenderEngine::RenderEngine(std::shared_ptr<Window> window) : window(std::move(window)) {
        std::vector<const char*> reqValidationLayers;

#ifdef LVE_DEBUG
        spdlog::info("Validations Layers: {}", "VK_LAYER_KHRONOS_validation");
        reqValidationLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

        instance = std::make_shared<Instance>(reqValidationLayers);
        surface = vk::SurfaceKHR(this->window->createWindowSurface(instance->getHandle()));
        device = std::make_shared<Device>(instance, reqValidationLayers, &surface);
    }

    RenderEngine::~RenderEngine() = default;

    void RenderEngine::cleanup() {
        device->getLogicalDevice().destroy(commandPool);
        graphicsPipeline->destroy();
        swapChain->cleanup(device->getAllocator());
        instance->getHandle().destroy(surface);
        device->destroy();
        instance->destroy();
    }

    void RenderEngine::beginDraw(const std::array<float, 4>& clearColor) {
        vk::Result result = swapChain->acquireNextImage(&imageIndex);

        if (result == vk::Result::eErrorOutOfDateKHR) {
            recreateDrawResources();
            return;
        }

        if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
            THROW_EX("Failed to acquire swap chain image!")

        std::array<vk::ClearValue, 2> clearValues{};
        clearValues[0].color = {clearColor};
        clearValues[1].depthStencil = {{1.0f, 0}};
        vk::CommandBufferBeginInfo beginInfo{};
        commandBuffers[imageIndex].begin(beginInfo);

        vk::RenderPassBeginInfo renderPassBeginInfo(
                swapChain->getRenderPass(),
                swapChain->getFrameBuffer(imageIndex),
                {{0, 0}, swapChain->getExtent()}, // renderArea
                clearValues
        );

        commandBuffers[imageIndex].beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);

        vk::Viewport viewport(
                0.0f, 0.0,
                static_cast<float>(swapChain->getExtent().width), static_cast<float>(swapChain->getExtent().height),
                0.0f, 1.0f
        );
        vk::Rect2D scissor({0, 0}, swapChain->getExtent());

        commandBuffers[imageIndex].setViewport(0, 1, &viewport);
        commandBuffers[imageIndex].setScissor(0, 1, &scissor);


        graphicsPipeline->bind(commandBuffers[imageIndex]);
    }

    void RenderEngine::endDraw() {
        commandBuffers[imageIndex].endRenderPass();
        commandBuffers[imageIndex].end();

        vk::Result result = swapChain->submitCommandBuffer(commandBuffers[imageIndex], imageIndex);

        if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || window->wasWindowResized()) {
            window->resetWindowResizeFlag();
            recreateDrawResources();
            return;
        }

        VK_HPP_CHECK_RESULT(result, "Failed to present swap chain handle!")
    }

    const std::shared_ptr<Device> &RenderEngine::getDevice() const {
        return device;
    }

    vk::CommandBuffer RenderEngine::getCommandBuffer() {
        return commandBuffers[imageIndex];
    }

    void RenderEngine::setupDrawResources() {
        swapChain = std::make_unique<SwapChain>(device, this->window->getExtent(), surface);
        createPipelines();
        createCmdBuffers();
    }

    void RenderEngine::createPipelines() {
        graphicsPipeline = std::make_unique<GraphicsPipeline>(
                device->getLogicalDevice(),
                "model.vert.spv","model.frag.spv",
                GraphicsPipeline::defaultConfig(swapChain->getRenderPass(),
                                                {vk::DynamicState::eViewport, vk::DynamicState::eScissor})
        );
    }

    void RenderEngine::createCmdBuffers() {
        commandPool = device->createCommandPool(device->getQueueFamilyIndices().graphics);

        commandBuffers.resize(swapChain->imageCount());
        vk::CommandBufferAllocateInfo allocateInfo(
                commandPool,
                vk::CommandBufferLevel::ePrimary,
                castU32(commandBuffers.size())
        );

        commandBuffers = device->getLogicalDevice().allocateCommandBuffers(allocateInfo);
    }

    void RenderEngine::recreateDrawResources() {
        auto extent = window->getExtent();

        while (extent.width == 0 || extent.height == 0) {
            extent = window->getExtent();
            glfwWaitEvents();
        }

        device->getLogicalDevice().waitIdle();

        graphicsPipeline->destroy();

        swapChain = std::make_unique<SwapChain>(device, extent, surface, std::move(swapChain));
        if (swapChain->imageCount() != commandBuffers.size()) {
            freeCommandBuffers();
            createCmdBuffers();
        }

        createPipelines();
    }

    void RenderEngine::freeCommandBuffers() {
        device->getLogicalDevice().free(commandPool, commandBuffers);
    }

} // namespace lve
