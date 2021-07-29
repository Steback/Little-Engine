#ifndef LITTLEVULKANENGINE_DEVICE_HPP
#define LITTLEVULKANENGINE_DEVICE_HPP


#include <memory>
#include <vector>

#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"

#include "utils/NonCopyable.hpp"


namespace lve {

    class Instance;

    class Device : NonCopyable {
    public:
        struct QueueFamilyIndices {
            uint32_t graphics;
            uint32_t present;
            uint32_t compute;
            uint32_t transfer;
        };

    public:
        Device(const std::shared_ptr<Instance>& instance, const std::vector<const char*>& layers, VkPhysicalDeviceFeatures features,
               const std::vector<const char*>& extensions, VkSurfaceKHR* surface = nullptr);

        ~Device() override;

        void destroy();

        uint32_t getQueueFamilyIndex(const VkQueueFlags& flags, VkSurfaceKHR* surface = nullptr);

        VkFormat findSupportFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling,
                                   VkFormatFeatureFlags features);

        uint32_t getMemoryType(uint32_t filter, VkMemoryPropertyFlags properties);

        void createCommandPool(VkCommandPool& commandPool, uint32_t queueFamilyIndex);

        void getGraphicsQueue(VkQueue& queue);

        void getPresentQueue(VkQueue& queue);

        void getComputeQueue(VkQueue& queue);

        void getTransferQueue(VkQueue& queue);

        [[nodiscard]] const VkDevice& getLogicalDevice() const;

        [[nodiscard]] const VkPhysicalDevice& getPhysicalDevice() const;

        [[nodiscard]] const QueueFamilyIndices &getQueueFamilyIndices() const;

        [[nodiscard]] const VmaAllocator& getAllocator() const;

    private:
        void createLogicalDevice(const std::vector<const char*>& layers, const std::vector<const char*>& extensions, VkPhysicalDeviceFeatures features,
                                 VkSurfaceKHR* surface = nullptr, VkQueueFlags queueFlags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT);

        void createAllocator(const std::shared_ptr<Instance>& instance);

    private:
        VkDevice logicalDevice{};
        VkPhysicalDevice physicalDevice{};
        QueueFamilyIndices queueFamilyIndices{};
        VmaAllocator allocator{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_DEVICE_HPP
