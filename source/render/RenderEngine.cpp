#include "RenderEngine.hpp"

#include "Window.hpp"
#include "Instance.hpp"
#include "Device.hpp"
#include "pipeline/GraphicsPipeline.hpp"


namespace lve {

    RenderEngine::RenderEngine(std::shared_ptr<Window> window) : window(std::move(window)) {
        instance = std::make_shared<Instance>();
        device = std::make_shared<Device>(instance);
        graphicsPipeline = std::make_unique<GraphicsPipeline>("model");
    }

    RenderEngine::~RenderEngine() = default;

    void RenderEngine::cleanup() {
        device->destroy();
        instance->destroy();
    }

} // namespace lve
