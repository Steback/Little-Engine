#ifndef LITTLEVULKANENGINE_IMAGE_HPP
#define LITTLEVULKANENGINE_IMAGE_HPP


#include "vk_mem_alloc.h"

#include "utils/NonCopyable.hpp"


namespace lve {

    class SwapChain;

    class Image  {
        friend SwapChain;

    public:
        Image();

        Image(VkDevice device, VmaAllocator allocator, const VkImageCreateInfo& createInfo, VmaMemoryUsage usage);

        ~Image();

        void create(VkDevice device_, VmaAllocator allocator_, const VkImageCreateInfo& createInfo, VmaMemoryUsage usage);

        void createView(VkImageAspectFlags aspectFlags);

        void destroy();

        [[nodiscard]] const VkImage& getHandle() const;

        [[nodiscard]] const VkImageView& getView() const;

        [[nodiscard]] const VmaAllocation& getAllocation() const;

        [[nodiscard]] VkFormat getFormat() const;

        [[nodiscard]] const VkExtent3D &getExtent() const;

        [[nodiscard]] uint32_t getMipLevels() const;

    private:
        VkDevice device{};
        VmaAllocator allocator{};
        VkImage image{};
        VkImageView view{};
        VmaAllocation allocation{};
        VkFormat format{};
        VkExtent3D extent{};
        uint32_t mipLevels{1};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_IMAGE_HPP
