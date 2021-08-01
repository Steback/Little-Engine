#ifndef LITTLEVULKANENGINE_DEVICE_HPP
#define LITTLEVULKANENGINE_DEVICE_HPP


#include <memory>
#include <vector>

#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"

#include "utils/NonCopyable.hpp"


namespace lve {

    class Instance;
    class Window;

    class Device : NonCopyable {
    public:
        struct QueueFamilyIndices {
            uint32_t graphics;
            uint32_t present;
            uint32_t compute;
            uint32_t transfer;
        };

    public:
        Device(const std::shared_ptr<Instance>& instance, const std::shared_ptr<Window>& window,
               const std::vector<const char*>& layers, VkPhysicalDeviceFeatures features,
               const std::vector<const char*>& extensions);

        ~Device() override;

        void destroy();

        uint32_t getQueueFamilyIndex(const VkQueueFlags& flags, VkSurfaceKHR surface_ = VK_NULL_HANDLE);

        VkFormat findSupportFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling,
                                   VkFormatFeatureFlags features);

        uint32_t getMemoryType(uint32_t filter, VkMemoryPropertyFlags properties);

        void createCommandPool(VkCommandPool& commandPool, uint32_t queueFamilyIndex);

        void getGraphicsQueue(VkQueue& queue);

        void getPresentQueue(VkQueue& queue);

        void getComputeQueue(VkQueue& queue);

        void getTransferQueue(VkQueue& queue);

        [[nodiscard]] const VkDevice& getDevice() const;

        [[nodiscard]] const VkPhysicalDevice& getPhysicalDevice() const;

        [[nodiscard]] const QueueFamilyIndices &getQueueFamilyIndices() const;

        [[nodiscard]] const VmaAllocator& getAllocator() const;

        [[nodiscard]] const VkSurfaceKHR& getSurface() const;

    private:
        void createLogicalDevice(const std::vector<const char*>& layers, const std::vector<const char*>& extensions, VkPhysicalDeviceFeatures features,
                                 VkQueueFlags queueFlags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT);

        void createAllocator(const std::shared_ptr<Instance>& instance);

    private:
        VkDevice device{};
        VkPhysicalDevice physicalDevice{};
        QueueFamilyIndices queueFamilyIndices{};
        VmaAllocator allocator{};
        VkSurfaceKHR surface{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_DEVICE_HPP
