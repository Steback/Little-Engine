#include "Device.hpp"

#include "spdlog/spdlog.h"

#include "Buffer.hpp"
#include "Instance.hpp"
#include "../BaseApp.hpp"
#include "../config/Config.hpp"
#include "../tools/Tools.hpp"
#include "../logger/Logger.hpp"


namespace lve {

    Device::Device(const std::shared_ptr<Instance>& instance, const std::vector<const char*>& reqLayers, vk::SurfaceKHR* surface) {
        std::vector<const char*> extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        physicalDevice = instance->pickPhysicalDevice(extensions);

        vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();
        spdlog::info("Physical Device: {}", properties.deviceName);

        if (BaseApp::config->reqDeviceInfo()) {
            std::vector<vk::ExtensionProperties> extensionsProperties = physicalDevice.enumerateDeviceExtensionProperties();

            fmt::print("\tExtensions supported:\n");
            for (auto& extension : extensionsProperties)
                fmt::print("\t\t{}\n", extension.extensionName);
        }

        createLogicalDevice(extensions, reqLayers, surface);
    }

    Device::~Device() = default;

    void Device::destroy() {
        logicalDevice.destroy();
    }

    Device::QueueFamilyIndices Device::getQueueFamilyIndices() const {
        return queueFamilyIndices;
    }

    uint32_t Device::getQueueFamilyIndex(vk::QueueFlags flags, vk::SurfaceKHR* surface) const {
        std::vector<vk::QueueFamilyProperties> properties = physicalDevice.getQueueFamilyProperties();

        if (flags & vk::QueueFlagBits::eCompute)
            for (uint32_t i = 0; i < static_cast<uint32_t>(properties.size()); i++)
                if ((properties[i].queueFlags & flags) && (!(properties[i].queueFlags & vk::QueueFlagBits::eGraphics)))
                    return i;

        if (flags & vk::QueueFlagBits::eTransfer)
            for (uint32_t i = 0; i < static_cast<uint32_t>(properties.size()); i++)
                if ((properties[i].queueFlags & flags) && (!(properties[i].queueFlags & vk::QueueFlagBits::eCompute))
                    && (!(properties[i].queueFlags & vk::QueueFlagBits::eCompute)))
                    return i;

        for (uint32_t i = 0; i < static_cast<uint32_t>(properties.size()); i++)
            if (properties[i].queueFlags & flags) {
                if (surface)
                    if (physicalDevice.getSurfaceSupportKHR(i, *surface))
                        return i;

                return i;
            }

        EXIT_ERROR("Could not find a matching queue family index")
    }

    const vk::PhysicalDevice &Device::getPhysicalDevice() const {
        return physicalDevice;
    }

    const vk::Device &Device::getLogicalDevice() const {
        return logicalDevice;
    }

    vk::Format Device::findSupportFormat(const std::vector<vk::Format> &candidates, vk::ImageTiling tiling,
                                         vk::FormatFeatureFlagBits features) {
        for (auto& format : candidates) {
            vk::FormatProperties properties = physicalDevice.getFormatProperties(format);

            if (tiling == vk::ImageTiling::eLinear && (properties.linearTilingFeatures & features) == features) {
                return format;
            } else if (tiling == vk::ImageTiling::eOptimal && (properties.optimalTilingFeatures & features) == features) {
                return format;
            }
        }

        THROW_EX("Failed to find supported format!");
    }

    vk::Queue Device::getGraphicsQueue() {
        return logicalDevice.getQueue(queueFamilyIndices.graphics, 0);
    }

    vk::Queue Device::getPresentQueue() {
        if (queueFamilyIndices.graphics != queueFamilyIndices.present) {
            return logicalDevice.getQueue(queueFamilyIndices.present, 0);
        } else {
            return logicalDevice.getQueue(queueFamilyIndices.present, 1);
        }
    }

    void Device::createLogicalDevice(const std::vector<const char*>& reqExtensions, const std::vector<const char*>& reqLayers, vk::SurfaceKHR* surface,
                                     vk::QueueFlags requestedQueueTypes) {
        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos{};

        if (requestedQueueTypes & vk::QueueFlagBits::eGraphics) {
            queueFamilyIndices.graphics = getQueueFamilyIndex(vk::QueueFlagBits::eGraphics);
            queueFamilyIndices.present = getQueueFamilyIndex(vk::QueueFlagBits::eGraphics, surface);

            if (queueFamilyIndices.graphics == queueFamilyIndices.present) {
                const float queuesPriority[] = {DEFAULT_QUEUE_PRIORITY, DEFAULT_QUEUE_PRIORITY};
                queueCreateInfos.push_back({{}, queueFamilyIndices.graphics, 2, queuesPriority});
            } else {
                queueCreateInfos.push_back({{}, queueFamilyIndices.graphics, 1, &DEFAULT_QUEUE_PRIORITY});
                queueCreateInfos.push_back({{}, queueFamilyIndices.present, 1, &DEFAULT_QUEUE_PRIORITY});
            }
        } else {
            queueFamilyIndices.graphics = 0;
        }

        if (requestedQueueTypes & vk::QueueFlagBits::eCompute) {
            queueFamilyIndices.compute = getQueueFamilyIndex(vk::QueueFlagBits::eCompute);

            if (queueFamilyIndices.compute != queueFamilyIndices.graphics) {
                queueCreateInfos.push_back({{}, queueFamilyIndices.compute, 1, &DEFAULT_QUEUE_PRIORITY});
            }
        } else {
            queueFamilyIndices.compute = queueFamilyIndices.graphics;
        }

        if (requestedQueueTypes & vk::QueueFlagBits::eTransfer) {
            queueFamilyIndices.transfer = getQueueFamilyIndex(vk::QueueFlagBits::eTransfer);

            if ((queueFamilyIndices.transfer != queueFamilyIndices.graphics)
                && (queueFamilyIndices.transfer != queueFamilyIndices.compute)) {
                queueCreateInfos.push_back({{}, queueFamilyIndices.transfer, 1, &DEFAULT_QUEUE_PRIORITY});
            }
        } else {
            queueFamilyIndices.transfer = queueFamilyIndices.graphics;
        }

        vk::DeviceCreateInfo createInfo(
                {},
                castU32(queueCreateInfos.size()),
                queueCreateInfos.data(),
                castU32(reqLayers.size()),
                reqLayers.data(),
                castU32(reqExtensions.size()),
                reqExtensions.data()
        );

        logicalDevice = physicalDevice.createDevice(createInfo);
    }

    uint32_t Device::getMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const {
        vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        }

        EXIT_ERROR("Failed to find suitable memory type");
    }

    vk::CommandPool Device::createCommandPool(uint32_t queueFamilyIndex) {
        vk::CommandPoolCreateInfo createInfo(
                vk::CommandPoolCreateFlagBits::eTransient | vk::CommandPoolCreateFlagBits::eResetCommandBuffer // flags
        );

        if (queueFamilyIndex == queueFamilyIndices.compute) {
            createInfo.queueFamilyIndex = queueFamilyIndices.compute;
            return logicalDevice.createCommandPool(createInfo);
        } else if (queueFamilyIndex == queueFamilyIndices.transfer) {
            createInfo.queueFamilyIndex = queueFamilyIndices.transfer;
            return logicalDevice.createCommandPool(createInfo);
        } else {
            createInfo.queueFamilyIndex = queueFamilyIndices.graphics;
            return logicalDevice.createCommandPool(createInfo);
        }
    }

    Buffer Device::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties) {
        Buffer buffer(logicalDevice, size, usage);

        vk::MemoryRequirements requirements = logicalDevice.getBufferMemoryRequirements(buffer.buffer);
        buffer.allocateMemory(requirements.size, getMemoryType(requirements.memoryTypeBits, properties));
        buffer.bind();

        return buffer;
    }

} // namespace lve