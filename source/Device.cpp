#include "Device.hpp"

#include "spdlog/spdlog.h"

#include "Window.hpp"
#include "Constants.hpp"
#include "Utilities.hpp"
#ifdef ENGINE_DEBUG
#include "Debug.hpp"
#endif


std::vector<const char*> getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef ENGINE_DEBUG
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    return extensions;
}

Device::Device(std::shared_ptr<Window> window) : window(std::move(window)) {
    std::vector<const char*> layers;
    std::vector<const char*> extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef ENGINE_DEBUG
    std::vector<vk::LayerProperties> instanceLayerProperties = vk::enumerateInstanceLayerProperties();

    layers.push_back("VK_LAYER_KHRONOS_validation");

    if (!checkLayers(layers, instanceLayerProperties))
        spdlog::warn("validation layers requested, but not available!");
#endif

    createInstance(layers);

    surface = this->window->createSurface(instance);

    selectPhysicalDevice(extensions);
    createLogicalDevice(extensions, layers);
}

Device::~Device() = default;

void Device::destroy() {
#ifdef ENGINE_DEBUG
    instance.destroy(debugUtilsMessenger);
#endif

    logicalDevice.destroy();
    instance.destroy(surface);
    instance.destroy();
}

uint32_t Device::getQueueFamilyIndex(vk::QueueFlags queueFlags) const {
    std::vector<vk::QueueFamilyProperties> properties = physicalDevice.getQueueFamilyProperties();

    if (queueFlags & vk::QueueFlagBits::eCompute) {
        for (uint32_t i = 0; i < static_cast<uint32_t>(properties.size()); i++) {
            if ((properties[i].queueFlags & queueFlags) && (!(properties[i].queueFlags & vk::QueueFlagBits::eGraphics)))
                return i;
        }
    }

    if (queueFlags & vk::QueueFlagBits::eTransfer) {
        for (uint32_t i = 0; i < static_cast<uint32_t>(properties.size()); i++) {
            if ((properties[i].queueFlags & queueFlags) && (!(properties[i].queueFlags & vk::QueueFlagBits::eCompute))
                && (!(properties[i].queueFlags & vk::QueueFlagBits::eCompute)))
                return i;
        }
    }

    for (uint32_t i = 0; i < static_cast<uint32_t>(properties.size()); i++) {
        if (properties[i].queueFlags & queueFlags)
            return i;
    }

    THROW_EX("Could not find a matching queue family index")
}

void Device::createInstance(const std::vector<const char*>& layers) {
    vk::ApplicationInfo appInfo("Little Engine", 1, "Little Engine", VK_API_VERSION_1_2);
    std::vector<const char*> extensions = getRequiredExtensions();

    instance = vk::createInstance({vk::InstanceCreateFlags(), &appInfo, layers, extensions});

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
    debugUtilsMessenger = instance.createDebugUtilsMessengerEXT({{}, severityFlags, messageTypeFlags, &debugMessageFunc});
#endif
}

void Device::selectPhysicalDevice(const std::vector<const char*>& extensions) {
    for (auto& device : instance.enumeratePhysicalDevices()) {
        if (checkExtensionsSupport(device, extensions))
            physicalDevice = device; break;
    }

    if (!physicalDevice)
        THROW_EX("Failed to find a suitable GPU!")

    spdlog::info("GPU: {}", physicalDevice.getProperties().deviceName);
}

void Device::createLogicalDevice(const std::vector<const char *> &extensions, const std::vector<const char*>& layers,
                                 vk::QueueFlags requestedQueueTypes) {
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos{};

    const float defaultQueuePriority(0.0f);

    if (requestedQueueTypes & vk::QueueFlagBits::eGraphics) {
        queueFamilyIndices.graphics = getQueueFamilyIndex(vk::QueueFlagBits::eGraphics);

        queueCreateInfos.push_back({{}, queueFamilyIndices.graphics, 1, &defaultQueuePriority});
    } else {
        queueFamilyIndices.graphics = VK_NULL_HANDLE;
    }

    if (requestedQueueTypes & vk::QueueFlagBits::eCompute) {
        queueFamilyIndices.compute = getQueueFamilyIndex(vk::QueueFlagBits::eCompute);

        if (queueFamilyIndices.compute != queueFamilyIndices.graphics)
            queueCreateInfos.push_back({{}, queueFamilyIndices.compute, 1, &defaultQueuePriority});
    } else {
        queueFamilyIndices.compute = queueFamilyIndices.graphics;
    }

    if (requestedQueueTypes & vk::QueueFlagBits::eTransfer) {
        queueFamilyIndices.transfer = getQueueFamilyIndex(vk::QueueFlagBits::eTransfer);

        if ((queueFamilyIndices.transfer != queueFamilyIndices.graphics) && (queueFamilyIndices.transfer != queueFamilyIndices.compute))
            queueCreateInfos.push_back({{}, queueFamilyIndices.transfer, 1, &defaultQueuePriority});
    } else {
        queueFamilyIndices.transfer = queueFamilyIndices.graphics;
    }

    logicalDevice = physicalDevice.createDevice({{}, castU32(queueCreateInfos.size()), queueCreateInfos.data(),
                                                 castU32(layers.size()), layers.data(),
                                                 castU32(extensions.size()), extensions.data(), {}});
}

bool Device::checkLayers(const std::vector<const char*>& layers, const std::vector<vk::LayerProperties>& properties) {
    return std::all_of(layers.begin(), layers.end(), [&properties](const char* name) {
        return std::find_if(properties.begin(), properties.end(), [&name](const vk::LayerProperties& property) {
            return std::strcmp(property.layerName, name) == 0;
        }) != properties.end();
    });
}

bool Device::checkExtensionsSupport(const vk::PhysicalDevice &device, const std::vector<const char *> &extensions) {
    std::vector<vk::ExtensionProperties> properties = device.enumerateDeviceExtensionProperties();

    return std::all_of(extensions.begin(), extensions.end(), [&properties](const char* name){
        return std::find_if(properties.begin(), properties.end(), [&name](const vk::ExtensionProperties& property){
           return std::strcmp(property.extensionName, name) == 0;
        }) != properties.end();
    });
}
