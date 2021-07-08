#include "Instance.hpp"

#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

#include "../BaseApp.hpp"
#include "../tools/Tools.hpp"
#include "../config/Config.hpp"


#ifdef LVE_DEBUG
PFN_vkCreateDebugUtilsMessengerEXT  pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance,
                                                              const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                              const VkAllocationCallbacks *pAllocator,
                                                              VkDebugUtilsMessengerEXT *pMessenger) {
    return pfnVkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
                                                           VkDebugUtilsMessengerEXT messenger,
                                                           VkAllocationCallbacks const *pAllocator) {
    return pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageFunc(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                                VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
                                                void* pUserData) {
    std::string message{};
    message += vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageTypes)) + ":\n";
    message += std::string("\t") + "messageIDName   = <" + pCallbackData->pMessageIdName + ">\n";
    message += std::string("\t") + "messageIdNumber = " + std::to_string(pCallbackData->messageIdNumber) + "\n";
    message += std::string("\t") + "message         = <" + pCallbackData->pMessage + ">\n";

    if (0 < pCallbackData->queueLabelCount) {
        message += std::string("\t") + "Queue Labels:\n";

        for (uint8_t i = 0; i < pCallbackData->queueLabelCount; i++) {
            message += std::string("\t\t") + "labelName = <" + pCallbackData->pQueueLabels[i].pLabelName + ">\n";
        }
    }
    if (0 < pCallbackData->cmdBufLabelCount) {
        message += std::string("\t") + "CommandBuffer Labels:\n";

        for ( uint8_t i = 0; i < pCallbackData->cmdBufLabelCount; i++ ) {
            message += std::string("\t\t") + "labelName = <" + pCallbackData->pCmdBufLabels[i].pLabelName + ">\n";
        }
    }

    if (0 < pCallbackData->objectCount) {
        message += std::string("\t") + "Objects:\n";

        for ( uint8_t i = 0; i < pCallbackData->objectCount; i++ ) {
            message += std::string("\t\t") + "Object " + std::to_string(i) + "\n";
            message += std::string("\t\t\t")
                       + "objectType   = "
                       + vk::to_string( static_cast<vk::ObjectType>( pCallbackData->pObjects[i].objectType ) ) + "\n";
            message += std::string("\t\t\t")
                       + "objectHandle = " + std::to_string(pCallbackData->pObjects[i].objectHandle) + "\n";

            if (pCallbackData->pObjects[i].pObjectName) {
                message += std::string("\t\t\t") + "objectName   = <" + pCallbackData->pObjects[i].pObjectName + ">\n";
            }
        }
    }

    spdlog::error(message);

    return false;
}
#endif

inline std::vector<const char*> getRequiredExtensions() {
    uint32_t extensionsCount = 0;
    const char** extensions = glfwGetRequiredInstanceExtensions(&extensionsCount);

    std::vector<const char*> extensionsNames(extensions, extensions + extensionsCount);

#ifdef LVE_DEBUG
    extensionsNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    return extensionsNames;
}

inline bool checkExtensionsSupport(const vk::PhysicalDevice &device, const std::vector<const char *> &extensions) {
    std::vector<vk::ExtensionProperties> properties = device.enumerateDeviceExtensionProperties();

    return std::all_of(extensions.begin(), extensions.end(), [&properties](const char* name){
        return std::find_if(properties.begin(), properties.end(), [&name](const vk::ExtensionProperties& property){
            return std::strcmp(property.extensionName, name) == 0;
        }) != properties.end();
    });
}

namespace lve {

    Instance::Instance() {
        vk::ApplicationInfo appInfo(
                BaseApp::config->getAppName().c_str(),
                VK_MAKE_VERSION(0, 0, 1),
                "Little Vulkan Engine",
                VK_MAKE_VERSION(0, 0, 1),
                VK_API_VERSION_1_2
        );

        std::vector<const char*> reqExtensions = getRequiredExtensions();
        std::vector<const char*> reqValidationLayers;

#ifdef LVE_DEBUG
        spdlog::info("Validations Layers: {}", "VK_LAYER_KHRONOS_validation");
        reqValidationLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

        vk::InstanceCreateInfo createInfo(
                {},
                &appInfo,
                castU32(reqValidationLayers.size()),
                reqValidationLayers.data(),
                castU32(reqExtensions.size()),
                reqExtensions.data()
        );

        instance = vk::createInstance(createInfo);

#ifdef LVE_DEBUG
        pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
        if (!pfnVkCreateDebugUtilsMessengerEXT)
            THROW_EX("GetInstanceProcAddr: Unable to find pfnVkCreateDebugUtilsMessengerEXT function.");

        pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
        if (!pfnVkDestroyDebugUtilsMessengerEXT)
            THROW_EX("GetInstanceProcAddr: Unable to find pfnVkDestroyDebugUtilsMessengerEXT function.");

        vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo(
                {},
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
                vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
                &debugMessageFunc
        );

        debugMessenger = instance.createDebugUtilsMessengerEXT(debugCreateInfo);
#endif
    }

    Instance::~Instance() = default;

    void Instance::destroy() {
#ifdef LVE_DEBUG
        instance.destroy(debugMessenger);
#endif

        instance.destroy();
    }

    vk::PhysicalDevice Instance::pickPhysicalDevice(const std::vector<const char*> &extensions) {
        std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

        if (physicalDevices.empty()) THROW_EX("Failed to find GPUs with Vulkan support!");

        for (auto& device : physicalDevices) {
            if (checkExtensionsSupport(device, {VK_KHR_SWAPCHAIN_EXTENSION_NAME})) {
                if (BaseApp::config->reqDeviceInfo()) {
                    vk::PhysicalDeviceProperties properties = device.getProperties();
                    std::vector<vk::ExtensionProperties> extensionsProperties = device.enumerateDeviceExtensionProperties();

                    spdlog::info("Physical Device: {}", properties.deviceName);

                    fmt::print("\tExtensions supported:\n");
                    for (auto& extension : extensionsProperties)
                        fmt::print("\t\t{}\n", extension.extensionName);
                }

                return device;
            }
        }

        THROW_EX("Failed to find a suitable GPU!")
    }

    vk::Instance Instance::getHandle() const {
        return instance;
    }

} // namespace lve