#include "RenderEngine.hpp"

#include "spdlog/spdlog.h"

#include "Window.hpp"
#include "Instance.hpp"
#include "Device.hpp"
#include "pipeline/GraphicsPipeline.hpp"


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
        graphicsPipeline = std::make_unique<GraphicsPipeline>("model");
    }

    RenderEngine::~RenderEngine() = default;

    void RenderEngine::cleanup() {
        instance->getHandle().destroy(surface);
        device->destroy();
        instance->destroy();
    }

} // namespace lve
