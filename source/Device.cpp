#include "Device.hpp"

#include "spdlog/spdlog.h"

#include "Window.hpp"
#include "Constants.hpp"


#ifdef ENGINE_DEBUG
#include "Debug.hpp"
#endif


Device::Device(std::shared_ptr<Window> window) : window(std::move(window)) {
    createInstance();
    selectPhysicalDevice();
}

Device::~Device() = default;

void Device::destroy() {
#ifdef ENGINE_DEBUG
    instance.destroy(debugUtilsMessenger);
#endif

    instance.destroy();
}

bool Device::checkLayers(const std::vector<const char*>& layers, const std::vector<vk::LayerProperties>& properties) {
    return std::all_of(layers.begin(), layers.end(), [&properties](const char* name) {
        return std::find_if(properties.begin(), properties.end(), [&name](const vk::LayerProperties& property) {
            return strcmp(property.layerName, name) == 0;
        }) != properties.end();
    });
}

void Device::createInstance() {
    vk::ApplicationInfo appInfo("Little Engine", 1, "Little Engine", VK_API_VERSION_1_2);
    std::vector<vk::LayerProperties> instanceLayerProperties = vk::enumerateInstanceLayerProperties();
    std::vector<const char*> instanceExtensionsNames;
    std::vector<const char*> instanceLayers;

#ifdef ENGINE_DEBUG
    instanceLayers.push_back("VK_LAYER_KHRONOS_validation");

    if (!checkLayers(instanceLayers, instanceLayerProperties))
        spdlog::warn("validation layers requested, but not available!");

    instanceExtensionsNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    vk::InstanceCreateInfo instanceCreateInfo(vk::InstanceCreateFlags(), &appInfo, instanceLayers, instanceExtensionsNames);
    instance = vk::createInstance(instanceCreateInfo);

#ifdef ENGINE_DEBUG
    pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
    if (!pfnVkCreateDebugUtilsMessengerEXT)
        THROW_EX("GetInstanceProcAddr: Unable to find pfnVkCreateDebugUtilsMessengerEXT function.")

    pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
    if (!pfnVkDestroyDebugUtilsMessengerEXT)
        THROW_EX("GetInstanceProcAddr: Unable to find pfnVkDestroyDebugUtilsMessengerEXT function.")

    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                                        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
    vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                                                        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                                                        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
    debugUtilsMessenger = instance.createDebugUtilsMessengerEXT(
            vk::DebugUtilsMessengerCreateInfoEXT({}, severityFlags, messageTypeFlags, &debugMessageFunc));
#endif
}

void Device::selectPhysicalDevice() {
    physicalDevice = instance.enumeratePhysicalDevices().front();

    if (!physicalDevice)
        THROW_EX("Failed to find a suitable GPU!")

    spdlog::info("GPU: {}", physicalDevice.getProperties().deviceName);
}


