#include "Renderer.hpp"
#include "Instance.hpp"
#include "Device.hpp"
#include "config/Config.hpp"
#include "window/Window.hpp"


namespace lve {

    Renderer::Renderer(std::shared_ptr<Window> window, const Config& config) : window(std::move(window)) {
        std::vector<const char *> validationLayers;

#ifdef LVE_DEBUG
        validationLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

        instance = std::make_shared<Instance>(validationLayers, config.getAppName().c_str());
        this->window->createWindowSurface(instance->getHandle(), &surface);

        VkPhysicalDeviceFeatures features{};
        std::vector<const char*> extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        device = std::make_shared<Device>(instance, validationLayers, features, extensions);
    }

    Renderer::~Renderer() {
        vkDestroySurfaceKHR(instance->getHandle(), surface, nullptr);
    }

} // namespace lv