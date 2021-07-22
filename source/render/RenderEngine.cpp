#include "RenderEngine.hpp"

#include <array>

#include "spdlog/spdlog.h"

#include "Window.hpp"
#include "Instance.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"
#include "pipeline/GraphicsPipeline.hpp"
#include "../tools/Tools.hpp"
#include "../mesh/Mesh.hpp"
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

        std::vector<Mesh::Vertex> vertices{
                { {0.0f, -0.5f}, {1.0f, 0.0f, 0.0f} },
                { {0.5f, 0.5f}, {0.0f, 1.0f, 0.0f} },
                { {-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f} }
        };

        model = std::make_unique<Mesh>(device, vertices);

        createPipelines();
        createCmdBuffers();
    }

    RenderEngine::~RenderEngine() = default;

    void RenderEngine::cleanup() {
        device->getLogicalDevice().waitIdle();

        model->destroy();

        device->getLogicalDevice().destroy(graphicsCmdPool);
        graphicsPipeline->destroy();
        swapChain->cleanup(device->getAllocator());
        instance->getHandle().destroy(surface);
        device->destroy();
        instance->destroy();
    }

    void RenderEngine::draw() {
        vk::Result result = swapChain->acquireNextImage(&imageIndex);

        result = swapChain->submitCommandBuffer(graphicsCmdBuffers[imageIndex], imageIndex);
        VK_HPP_CHECK_RESULT(result, "Failed to present swap chain handle!")
    }

    void RenderEngine::beginDraw(const std::array<float, 4>& clearColor) {

    }

    void RenderEngine::endDraw() {

    }

    const std::shared_ptr<Device> &RenderEngine::getDevice() const {
        return device;
    }

    void RenderEngine::createPipelines() {
        graphicsPipeline = std::make_unique<GraphicsPipeline>(
                device->getLogicalDevice(),
                "model.vert.spv","model.frag.spv",
                GraphicsPipeline::defaultConfig(swapChain->getRenderPass(), window->getSize().width, window->getSize().height)
        );
    }

    void RenderEngine::createCmdBuffers() {
        graphicsCmdPool = device->createCommandPool(device->getQueueFamilyIndices().graphics);

        graphicsCmdBuffers.resize(swapChain->imageCount());
        vk::CommandBufferAllocateInfo allocateInfo(
                graphicsCmdPool,
                vk::CommandBufferLevel::ePrimary,
                castU32(graphicsCmdBuffers.size())
        );

        graphicsCmdBuffers = device->getLogicalDevice().allocateCommandBuffers(allocateInfo);

        std::array<vk::ClearValue, 2> clearValues{};
        clearValues[0].color = {std::array<float, 4>({{0.1f, 0.1f, 0.1f, 1.0f}})};
        clearValues[1].depthStencil = {{1.0f, 0}};

        for (size_t i = 0; i < graphicsCmdBuffers.size(); ++i) {
            vk::CommandBufferBeginInfo beginInfo{};
            graphicsCmdBuffers[i].begin(beginInfo);

            vk::RenderPassBeginInfo renderPassBeginInfo(
                    swapChain->getRenderPass(),
                    swapChain->getFrameBuffer(i),
                    {{0, 0}, swapChain->getExtent()}, // renderArea
                    castU32(clearValues.size()),
                    clearValues.data()
            );

            graphicsCmdBuffers[i].beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);

            graphicsPipeline->bind(graphicsCmdBuffers[i]);
            model->bind(graphicsCmdBuffers[i]);
            model->draw(graphicsCmdBuffers[i]);

            graphicsCmdBuffers[i].endRenderPass();
            graphicsCmdBuffers[i].end();
        }
    }

} // namespace lve
