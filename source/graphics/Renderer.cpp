#include "Renderer.hpp"
#include "Instance.hpp"
#include "config/Config.hpp"


namespace lve {

    Renderer::Renderer(std::shared_ptr<Window> window, const Config& config) : window(std::move(window)) {
        std::vector<const char *> validationLayers;

#ifdef LVE_DEBUG
        validationLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

        instance = std::make_shared<Instance>(validationLayers, config.getAppName().c_str());
    }

    Renderer::~Renderer() {

    }

} // namespace lv