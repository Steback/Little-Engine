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
        swapChain = std::make_unique<SwapChain>(device, this->window->getExtent(), surface);



        createPipelines();
        createCmdBuffers();
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

        std::array<vk::ClearValue, 2> clearValues{};
        clearValues[0].color = {clearColor};
        clearValues[1].depthStencil = {{1.0f, 0}};
        vk::CommandBufferBeginInfo beginInfo{};
        commandBuffers[imageIndex].begin(beginInfo);

        vk::RenderPassBeginInfo renderPassBeginInfo(
                swapChain->getRenderPass(),
                swapChain->getFrameBuffer(imageIndex),
                {{0, 0}, swapChain->getExtent()}, // renderArea
                castU32(clearValues.size()),
                clearValues.data()
        );

        commandBuffers[imageIndex].beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);

        graphicsPipeline->bind(commandBuffers[imageIndex]);
    }

    void RenderEngine::endDraw() {
        commandBuffers[imageIndex].endRenderPass();
        commandBuffers[imageIndex].end();

        vk::Result result = swapChain->submitCommandBuffer(commandBuffers[imageIndex], imageIndex);
        VK_HPP_CHECK_RESULT(result, "Failed to present swap chain handle!")
    }

    const std::shared_ptr<Device> &RenderEngine::getDevice() const {
        return device;
    }

    vk::CommandBuffer RenderEngine::getCommandBuffer() {
        return commandBuffers[imageIndex];
    }

    void RenderEngine::createPipelines() {
        graphicsPipeline = std::make_unique<GraphicsPipeline>(
                device->getLogicalDevice(),
                "model.vert.spv","model.frag.spv",
                GraphicsPipeline::defaultConfig(swapChain->getRenderPass(), window->getSize().width, window->getSize().height)
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

} // namespace lve
