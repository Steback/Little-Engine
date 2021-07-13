#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_DEVICE_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_DEVICE_HPP


#include <vector>

#include "vulkan/vulkan.hpp"


namespace lve {

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
        explicit Device(const std::shared_ptr<Instance>& instance, const std::vector<const char*>& reqLayers = {},
                        vk::SurfaceKHR* surface = nullptr);

        Device(const Device&) = delete;

        ~Device();

        Device& operator=(const Device&) = delete;

        void destroy();

        [[nodiscard]] QueueFamilyIndices getQueueFamilyIndices() const;

        [[nodiscard]] uint32_t getQueueFamilyIndex(vk::QueueFlags flags, vk::SurfaceKHR* surface = nullptr) const;

        [[nodiscard]] const vk::PhysicalDevice &getPhysicalDevice() const;

        [[nodiscard]] const vk::Device &getLogicalDevice() const;

        vk::Format findSupportFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlagBits features);

        vk::Queue getGraphicsQueue();

        vk::Queue getPresentQueue();

        uint32_t getMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const;

    private:
        void createLogicalDevice(const std::vector<const char*>& reqExtensions, const std::vector<const char*>& reqLayers,
                                 vk::SurfaceKHR* surface = nullptr,
                                 vk::QueueFlags requestedQueueTypes = vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer);

        vk::PhysicalDevice physicalDevice{};
        vk::Device logicalDevice{};
        QueueFamilyIndices queueFamilyIndices;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_RENDER_DEVICE_HPP
