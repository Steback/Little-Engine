#include "Instance.hpp"

#include "spdlog/spdlog.h"
#include "GLFW/glfw3.h"

#include "Utilities.hpp"


#ifdef ENGINE_DEBUG
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
    auto* instance = reinterpret_cast<Instance*>(pUserData);
    std::unordered_map<uint32_t, std::string>& debugMessages = instance->debugMessages;

    if (debugMessages.find(pCallbackData->messageIdNumber) == debugMessages.end()) {
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

        debugMessages[pCallbackData->messageIdNumber] = pCallbackData->pMessage;

        spdlog::error(message);
    }

    return false;
}
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

Instance::Instance(const vk::ApplicationInfo& appInfo, const std::vector<const char*>& reqLayers) {
    std::vector<const char*> reqExtensions = getRequiredExtensions();
    vk::InstanceCreateInfo createInfo(
            {},
            &appInfo,
            castU32(reqLayers.size()),
            reqLayers.data(),
            castU32(reqExtensions.size()),
            reqExtensions.data()
    );

    instance = vk::createInstance(createInfo);

#ifdef ENGINE_DEBUG
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
            &debugMessageFunc,
            this
    );

    debugMessenger = instance.createDebugUtilsMessengerEXT(debugCreateInfo);
#endif
}

Instance::~Instance() = default;

void Instance::destroy() {
#ifdef ENGINE_DEBUG
    instance.destroy(debugMessenger);
#endif
    instance.destroy();
}

const vk::Instance &Instance::getHandle() const {
    return instance;
}

void Instance::destroy(const vk::SurfaceKHR &surface) {
    instance.destroy(surface);
}
