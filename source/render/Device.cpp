#include "Device.hpp"

#include "spdlog/spdlog.h"
#include "GLFW/glfw3.h"

#include "Initializers.hpp"
#include "../BaseApp.hpp"
#include "../config/Config.hpp"
#include "../tools/Tools.hpp"
#include "../tools/Constants.hpp"


inline std::vector<const char*> getRequiredExtensions() {
    uint32_t extensionsCount = 0;
    const char** extensions = glfwGetRequiredInstanceExtensions(&extensionsCount);

    std::vector<const char*> extensionsNames(extensions, extensions + extensionsCount);

#ifdef LVE_DEBUG
    extensionsNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    return extensionsNames;
}

namespace lve {

    Device::Device(std::shared_ptr<Window> window) : window(std::move(window)) {
        createInstance();
        pickPhysicalDevice();
    }

    Device::~Device() = default;

    void Device::destroy() {
        vkDestroyInstance(instance, nullptr);
    }

    VkPhysicalDeviceProperties Device::getPhysicalDeviceProperties(const VkPhysicalDevice &device) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);

        return properties;
    }

    std::vector<VkExtensionProperties> Device::getExtensionsProperties(const VkPhysicalDevice &device) {
        uint32_t extensionsCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount, nullptr);

        std::vector<VkExtensionProperties> properties(extensionsCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount, properties.data());

        return properties;
    }

    bool Device::checkExtensionsSupport(const VkPhysicalDevice &device, const std::vector<const char *> &extensions) {
        std::vector<VkExtensionProperties> properties = getExtensionsProperties(device);

        return std::all_of(extensions.begin(), extensions.end(), [&properties](const char* name){
            return std::find_if(properties.begin(), properties.end(), [&name](const VkExtensionProperties& property){
                return std::strcmp(property.extensionName, name) == 0;
            }) != properties.end();
        });
    }

    void Device::createInstance() {
        VkApplicationInfo appInfo = initializers::applicationInfo(
                BaseApp::config->getAppName().c_str(),
                VK_MAKE_VERSION(0, 0, 1),
                "Little Vulkan Engine",
                VK_MAKE_VERSION(0, 0, 1)
        );

        std::vector<const char*> reqExtensions = getRequiredExtensions();
        std::vector<const char*> reqValidationLayers;

        if (BaseApp::config->reqValidationLayers()) {
            spdlog::info("Validations Layers: {}", "VK_LAYER_KHRONOS_validation");
            reqValidationLayers.push_back("VK_LAYER_KHRONOS_validation");
        }

        VkInstanceCreateInfo createInfo = initializers::instanceCreateInfo(appInfo);
        createInfo.enabledExtensionCount = castU32(reqExtensions.size());
        createInfo.ppEnabledExtensionNames = reqExtensions.data();
        createInfo.enabledLayerCount = castU32(reqValidationLayers.size());
        createInfo.ppEnabledLayerNames = reqValidationLayers.data();

        VK_CHECK_RESULT(vkCreateInstance(&createInfo, nullptr, &instance),
                        "Failed to create vulkan instance");
    }

    void Device::pickPhysicalDevice() {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) THROW_EX("Failed to find GPUs with Vulkan support!");

        std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

        for (auto& device : physicalDevices) {
            if (checkExtensionsSupport(device, {VK_KHR_SWAPCHAIN_EXTENSION_NAME})) {
                physicalDevice = device;

                if (BaseApp::config->reqDeviceInfo()) {
                    VkPhysicalDeviceProperties properties = getPhysicalDeviceProperties(physicalDevice);
                    std::vector<VkExtensionProperties> extensionsProperties = getExtensionsProperties(physicalDevice);

                    spdlog::info("Physical Device: {}", properties.deviceName);

                    spdlog::info("Extensions supported:");
                    for (auto& extension : extensionsProperties)
                        fmt::print("\t{}\n", extension.extensionName);
                }
            }
        }

        if (!physicalDevice) THROW_EX("Failed to find a suitable GPU!");
    }

} // namespace lve