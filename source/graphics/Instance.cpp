#include "Instance.hpp"

#include <unordered_set>

#include "GLFW/glfw3.h"

#include "logs/Logs.hpp"


#ifdef LVE_DEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData) {
    spdlog::error(pCallbackData->pMessage);
    LVE_LOG_ERROR(pCallbackData->pMessage);

    return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
        const VkAllocationCallbacks *pAllocator,
        VkDebugUtilsMessengerEXT *pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance,
            "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks *pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance,
            "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;  // Optional
}
#endif


namespace lve {

    Instance::Instance(const std::vector<const char *>& layers, const char* appName) {
        if (!checkLayersSupport(layers)) LVE_LOG_ERROR_EXIT("Validation layers requested, but not available!")

        VkApplicationInfo applicationInfo{VK_STRUCTURE_TYPE_APPLICATION_INFO};
        applicationInfo.pApplicationName = appName;
        applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        applicationInfo.pEngineName = "Little Vulkan Engine";
        applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        applicationInfo.applicationVersion = VK_API_VERSION_1_2;

        VkInstanceCreateInfo createInfo{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
        createInfo.pApplicationInfo = &applicationInfo;
        createInfo.enabledLayerCount = LVE_CASTU32(layers.size());
        createInfo.ppEnabledLayerNames = layers.data();

        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = LVE_CASTU32(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        LVE_VK_CHECK_RESULT_EXIT(vkCreateInstance(&createInfo, nullptr, &instance),
                                 "Failed to create instance!")

        hasGflwRequiredInstanceExtensions();

#ifdef LVE_DEBUG
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        populateDebugMessengerCreateInfo(debugCreateInfo);

        LVE_VK_CHECK_RESULT_EXIT(CreateDebugUtilsMessengerEXT(instance, &debugCreateInfo, nullptr, &debugMessenger),
                                 "failed to set up debug messenger!")
#endif
    }

    Instance::~Instance() {
#ifdef LVE_DEBUG
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
#endif

        vkDestroyInstance(instance, nullptr);
    }

    VkPhysicalDevice Instance::pickPhysicalDevice(const std::vector<const char *> &extensions) {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) LVE_LOG_ERROR_EXIT("Failed to find GPUs with Vulkan support!");

        std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

        for (auto& device : physicalDevices) {
            if (checkExtensionsSupport(device, extensions)) return device;
        }

        LVE_LOG_ERROR_EXIT("Failed to find a suitable GPU!")
    }

    const VkInstance &Instance::getHandle() {
        return instance;
    }

    std::vector<const char *> Instance::getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef LVE_DEBUG
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        return extensions;
    }

    bool Instance::checkExtensionsSupport(const VkPhysicalDevice &device, const std::vector<const char *> &extensions) {
        uint32_t extensionsCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount, nullptr);

        std::vector<VkExtensionProperties> properties(extensionsCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount, properties.data());

        return std::all_of(extensions.begin(), extensions.end(), [&properties](const char* name){
            return std::find_if(properties.begin(), properties.end(), [&name](const VkExtensionProperties& property){
                return std::strcmp(property.extensionName, name) == 0;
            }) != properties.end();
        });    }

    void Instance::hasGflwRequiredInstanceExtensions() {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        std::unordered_set<std::string> available;
        for (const auto &extension : extensions)
            available.insert(extension.extensionName);

        auto requiredExtensions = getRequiredExtensions();
        for (const auto &required : requiredExtensions) {
            if (available.find(required) == available.end())
                LVE_LOG_ERROR_EXIT("Missing required glfw extension")
        }
    }

    bool Instance::checkLayersSupport(const std::vector<const char *>& layers) {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char *layerName : layers) {
            bool layerFound = false;

            for (const auto &layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

} // namespace lv
