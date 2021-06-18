#ifndef LITTLEENGINE_DEVICE_HPP
#define LITTLEENGINE_DEVICE_HPP


#include <string>
#include <vector>
#include <memory>

#include "vulkan/vulkan.hpp"


class Window;

class Device {
public:
    struct QueueFamilyIndices {
        uint32_t graphics{};
        uint32_t compute{};
        uint32_t transfer{};
    };

public:
    explicit Device(std::shared_ptr<Window> window);

    ~Device();

    void destroy();

    [[nodiscard]] uint32_t getQueueFamilyIndex(vk::QueueFlags queueFlags) const;

    vk::Device& getLogicalDevice();

private:
    void createInstance(const std::vector<const char*>& layers);

    void selectPhysicalDevice(const std::vector<const char*>& extensions);

    void createLogicalDevice(const std::vector<const char*>& extensions, const std::vector<const char*>& layers,
                             vk::QueueFlags requestedQueueTypes = vk::QueueFlagBits::eGraphics |
                                     vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer);

    static bool checkLayers(const std::vector<const char*>& layers, const std::vector<vk::LayerProperties>& properties);

    static bool checkExtensionsSupport(const vk::PhysicalDevice& device, const std::vector<const char*>& extensions);

private:
    std::shared_ptr<Window> window;
    vk::Instance instance;
    vk::DebugUtilsMessengerEXT debugUtilsMessenger;
    vk::PhysicalDevice physicalDevice;
    vk::Device logicalDevice;
    QueueFamilyIndices queueFamilyIndices;
    vk::SurfaceKHR surface;
};


#endif //LITTLEENGINE_DEVICE_HPP
