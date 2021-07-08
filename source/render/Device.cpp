#include "Device.hpp"

#include "spdlog/spdlog.h"

#include "../BaseApp.hpp"
#include "../config/Config.hpp"
#include "../tools/Tools.hpp"
#include "../tools/Constants.hpp"

#include "Instance.hpp"


namespace lve {

    Device::Device(const std::shared_ptr<Instance>& instance, const std::vector<const char*>& reqLayers, vk::SurfaceKHR* surface) {
        std::vector<const char*> extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        physicalDevice = instance->pickPhysicalDevice(extensions);

        if (BaseApp::config->reqDeviceInfo()) {
            vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();
            std::vector<vk::ExtensionProperties> extensionsProperties = physicalDevice.enumerateDeviceExtensionProperties();

            spdlog::info("Physical Device: {}", properties.deviceName);

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

        THROW_EX("Could not find a matching queue family index")
    }

    const vk::PhysicalDevice &Device::getPhysicalDevice() const {
        return physicalDevice;
    }

    const vk::Device &Device::getLogicalDevice() const {
        return logicalDevice;
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

} // namespace lve