#include "Device.hpp"

#include "Instance.hpp"
#include "logs/Logs.hpp"


const float DEFAULT_QUEUE_PRIORITY = 1.0f;


namespace lve {

    Device::Device(const std::shared_ptr<Instance> &instance, const std::vector<const char *> &layers,
                   VkPhysicalDeviceFeatures features, const std::vector<const char *>& extensions,
                   VkSurfaceKHR *surface) {
        physicalDevice = instance->pickPhysicalDevice(extensions);

        createLogicalDevice(layers, extensions, features, surface);
        createAllocator(instance);
    }

    Device::~Device() = default;

    void Device::destroy() {
        vmaDestroyAllocator(allocator);
        vkDestroyDevice(device, nullptr);
    }

    uint32_t Device::getQueueFamilyIndex(const VkQueueFlags &flags, VkSurfaceKHR *surface) {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> properties(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, properties.data());

        if (flags & VK_QUEUE_COMPUTE_BIT) {
            for (uint32_t i = 0; i < CAST_U32(properties.size()); ++i) {
                if ((properties[i].queueFlags & flags) && (!(properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)))
                    return i;
            }
        }

        if (flags & VK_QUEUE_TRANSFER_BIT) {
            for (uint32_t i = 0; i < CAST_U32(properties.size()); ++i) {
                if ((properties[i].queueFlags & flags) && (!(properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)))
                    return i;
            }
        }

        for (uint32_t i = 0; i < CAST_U32(properties.size()); ++i) {
            if (properties[i].queueFlags & flags) {
                if (surface) {
                    VkBool32 supported = false;
                    vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, *surface, &supported);

                    if (supported) return i;
                }

                return i;
            }
        }

        LVE_LOG_ERROR_EXIT("Could not find a matching queue family index")
    }

    VkFormat Device::findSupportFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling,
                                       VkFormatFeatureFlags features) {
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            } else if (
                    tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }

        LVE_THROW_EX("Failed to find supported format!");
    }

    uint32_t Device::getMemoryType(uint32_t filter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
            if ((filter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        }

        LVE_THROW_EX("Failed to find suitable memory type");
    }

    void Device::createCommandPool(VkCommandPool& commandPool, uint32_t queueFamilyIndex) {
        VkCommandPoolCreateInfo createInfo{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
        createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

        if (queueFamilyIndex == queueFamilyIndices.compute) {
            createInfo.queueFamilyIndex = queueFamilyIndices.compute;
            vkCreateCommandPool(device, &createInfo, nullptr, &commandPool);
        } else if (queueFamilyIndex == queueFamilyIndices.transfer) {
            createInfo.queueFamilyIndex = queueFamilyIndices.transfer;
            vkCreateCommandPool(device, &createInfo, nullptr, &commandPool);
        } else {
            createInfo.queueFamilyIndex = queueFamilyIndices.graphics;
            vkCreateCommandPool(device, &createInfo, nullptr, &commandPool);
        }
    }

    void Device::getGraphicsQueue(VkQueue& queue) {
        vkGetDeviceQueue(device, queueFamilyIndices.graphics, 0, &queue);
    }

    void Device::getPresentQueue(VkQueue &queue) {
        if (queueFamilyIndices.graphics != queueFamilyIndices.present) {
            vkGetDeviceQueue(device, queueFamilyIndices.present, 0, &queue);
        } else {
            vkGetDeviceQueue(device, queueFamilyIndices.graphics, 1, &queue);
        }
    }

    void Device::getComputeQueue(VkQueue &queue) {
        vkGetDeviceQueue(device, queueFamilyIndices.compute, 0, &queue);
    }

    void Device::getTransferQueue(VkQueue &queue) {
        vkGetDeviceQueue(device, queueFamilyIndices.transfer, 0, &queue);
    }

    const VkDevice& Device::getLogicalDevice() const {
        return device;
    }

    const VkPhysicalDevice& Device::getPhysicalDevice() const {
        return physicalDevice;
    }

    const Device::QueueFamilyIndices &Device::getQueueFamilyIndices() const {
        return queueFamilyIndices;
    }

    const VmaAllocator& Device::getAllocator() const {
        return allocator;
    }

    void
    Device::createLogicalDevice(const std::vector<const char *> &layers, const std::vector<const char *>& extensions,
                                VkPhysicalDeviceFeatures features, VkSurfaceKHR *surface, VkQueueFlags queueFlags) {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

        if (queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queueFamilyIndices.graphics = getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
            queueFamilyIndices.present = getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT, surface);

            if (queueFamilyIndices.graphics == queueFamilyIndices.present) {
                const float queuesPriority[] = {DEFAULT_QUEUE_PRIORITY, DEFAULT_QUEUE_PRIORITY};
                VkDeviceQueueCreateInfo createInfo{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
                createInfo.queueFamilyIndex = queueFamilyIndices.graphics;
                createInfo.queueCount = 2;
                createInfo.pQueuePriorities = queuesPriority;

                queueCreateInfos.push_back(createInfo);
            } else {
                VkDeviceQueueCreateInfo createInfoGraphics{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
                createInfoGraphics.queueFamilyIndex = queueFamilyIndices.graphics;
                createInfoGraphics.queueCount = 1;
                createInfoGraphics.pQueuePriorities = &DEFAULT_QUEUE_PRIORITY;

                queueCreateInfos.push_back(createInfoGraphics);

                VkDeviceQueueCreateInfo createInfoPresent{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
                createInfoPresent.queueFamilyIndex = queueFamilyIndices.present;
                createInfoPresent.queueCount = 1;
                createInfoPresent.pQueuePriorities = &DEFAULT_QUEUE_PRIORITY;

                queueCreateInfos.push_back(createInfoPresent);
            }
        } else {
            queueFamilyIndices.graphics = 0;
        }

        if (queueFlags & VK_QUEUE_COMPUTE_BIT) {
            queueFamilyIndices.compute = getQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);

            VkDeviceQueueCreateInfo createInfo{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
            createInfo.queueFamilyIndex = queueFamilyIndices.compute;
            createInfo.queueCount = 1;
            createInfo.pQueuePriorities = &DEFAULT_QUEUE_PRIORITY;

            queueCreateInfos.push_back(createInfo);
        } else {
            queueFamilyIndices.compute = queueFamilyIndices.graphics;
        }

        if (queueFlags & VK_QUEUE_TRANSFER_BIT) {
            queueFamilyIndices.transfer = getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT);

            VkDeviceQueueCreateInfo createInfo{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
            createInfo.queueFamilyIndex = queueFamilyIndices.transfer;
            createInfo.queueCount = 1;
            createInfo.pQueuePriorities = &DEFAULT_QUEUE_PRIORITY;

            queueCreateInfos.push_back(createInfo);
        } else {
            queueFamilyIndices.transfer = queueFamilyIndices.graphics;
        }

        VkDeviceCreateInfo createInfo{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
        createInfo.enabledLayerCount = CAST_U32(layers.size());
        createInfo.ppEnabledLayerNames = layers.data();
        createInfo.enabledExtensionCount = CAST_U32(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
        createInfo.queueCreateInfoCount = CAST_U32(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &features;

        LVE_VK_CHECK_RESULT_EXIT(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device),
                                 "Failed to create logical device!")
    }

    void Device::createAllocator(const std::shared_ptr<Instance> &instance) {
        VmaAllocatorCreateInfo createInfo{};
        // TODO: Set Vulkan version 1.0 to VMA because vkGetImageMemoryRequirements2KHR is NULL
        createInfo.vulkanApiVersion = VK_API_VERSION_1_0;
        createInfo.device = device;
        createInfo.physicalDevice = physicalDevice;
        createInfo.instance = instance->getHandle();

        LVE_VK_CHECK_RESULT_EXIT(vmaCreateAllocator(&createInfo, &allocator),
                                 "Failed to create memory Allocator")
    }

} // namespace lv