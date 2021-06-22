#include "Device.hpp"

#include <utility>

#include "Instance.hpp"
#include "Window.hpp"
#include "Tools.hpp"
#include "Constants.hpp"


Device::Device(const std::shared_ptr<Window>& window, std::shared_ptr<Instance> instance,
               const std::vector<const char*>& layers, vk::QueueFlags requestedQueueTypes)
        : instance(std::move(instance)) {
    std::vector<const char*> reqExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    physicalDevice = this->instance->selectPhysicalDevice(reqExtensions);

    surface = window->createSurface(this->instance->getHandle());

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos{};

    if (requestedQueueTypes & vk::QueueFlagBits::eGraphics) {
        queueFamilyIndices.graphics = getQueueFamilyIndex(vk::QueueFlagBits::eGraphics);
        queueFamilyIndices.present = getQueueFamilyIndex(vk::QueueFlagBits::eGraphics, true);

        if (queueFamilyIndices.graphics == queueFamilyIndices.present) {
            const float queuesPriority[] = {DEFAULT_QUEUE_PRIORITY, DEFAULT_QUEUE_PRIORITY};
            queueCreateInfos.push_back({{}, queueFamilyIndices.graphics, 2, queuesPriority});
        } else {
            queueCreateInfos.push_back({{}, queueFamilyIndices.graphics, 1, &DEFAULT_QUEUE_PRIORITY});
            queueCreateInfos.push_back({{}, queueFamilyIndices.present, 1, &DEFAULT_QUEUE_PRIORITY});
        }
    } else {
        queueFamilyIndices.graphics = VK_NULL_HANDLE;
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
            castU32(layers.size()),
            layers.data(),
            castU32(reqExtensions.size()),
            reqExtensions.data()
    );

    logicalDevice = physicalDevice.createDevice(createInfo);
}

const std::shared_ptr<Instance> &Device::getInstance() const {
    return instance;
}

const vk::PhysicalDevice &Device::getPhysicalDevice() const {
    return physicalDevice;
}

const vk::Device &Device::getLogicalDevice() const {
    return logicalDevice;
}

const QueueFamilyIndices &Device::getQueueFamilyIndices() const {
    return queueFamilyIndices;
}

const vk::SurfaceKHR &Device::getSurface() const {
    return surface;
}

uint32_t Device::getQueueFamilyIndex(vk::QueueFlags flags, bool presentSupport) const {
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
            if (presentSupport)
                if (physicalDevice.getSurfaceSupportKHR(i, surface))
                    return i;

            return i;
        }

    THROW_EX("Could not find a matching queue family index")
}

void Device::destroy() {
    if (surface) instance->destroy(surface);

    logicalDevice.destroy();
}
