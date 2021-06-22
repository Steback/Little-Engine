#ifndef LITTLEENGINE_DEVICE_HPP
#define LITTLEENGINE_DEVICE_HPP


#include <memory>

#include "vulkan/vulkan.hpp"


class Window;
class Instance;

class Device {
public:
    struct QueueFamilyIndices {
        uint32_t graphics{};
        uint32_t compute{};
        uint32_t transfer{};
        uint32_t present{};
    };

public:
    explicit Device(const std::shared_ptr<Window>& window, std::shared_ptr<Instance> instance, const std::vector<const char*>& layers = {},
                    vk::QueueFlags requestedQueueTypes = vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer);

    [[nodiscard]] const std::shared_ptr<Instance> &getInstance() const;

    [[nodiscard]] const vk::PhysicalDevice &getPhysicalDevice() const;

    [[nodiscard]] const vk::Device &getLogicalDevice() const;

    [[nodiscard]] const QueueFamilyIndices &getQueueFamilyIndices() const;

    [[nodiscard]] const vk::SurfaceKHR &getSurface() const;

    [[nodiscard]] uint32_t getQueueFamilyIndex(vk::QueueFlags flags, bool presentSupport = false) const;

    void destroy();

private:
    std::shared_ptr<Instance> instance;
    vk::PhysicalDevice physicalDevice;
    vk::Device logicalDevice;
    QueueFamilyIndices queueFamilyIndices;
    vk::SurfaceKHR surface;
};

using QueueFamilyIndices = Device::QueueFamilyIndices;


#endif //LITTLEENGINE_DEVICE_HPP
