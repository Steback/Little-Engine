#include "Image.hpp"

#include "logs/Logs.hpp"


namespace lve {

    Image::Image() = default;

    Image::Image(VkDevice device, VmaAllocator allocator, const VkImageCreateInfo &createInfo, VmaMemoryUsage usage)
            : device(device), allocator(allocator) {
        format = createInfo.format;
        mipLevels = createInfo.mipLevels;
        extent = createInfo.extent;

        VmaAllocationCreateInfo allocInfo{};
        allocInfo.usage = usage;
        vmaCreateImage(this->allocator, &createInfo, &allocInfo, &image, &allocation, nullptr);
    }

    Image::~Image() = default;

    void Image::createView(VkImageAspectFlags aspectFlags) {
        VkImageViewCreateInfo createInfo{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        createInfo.image = image;
        createInfo.format = format;
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.subresourceRange.aspectMask = aspectFlags;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = mipLevels;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        LVE_VK_CHECK_RESULT(vkCreateImageView(device, &createInfo, nullptr, &view),
                            "Failed to create image view");
    }

    void Image::destroy() {
        vkDestroyImageView(device, view, nullptr);
        vmaDestroyImage(allocator, image, allocation);
    }

    const VkImage& Image::getHandle() const {
        return image;
    }

    const VkImageView& Image::getView() const {
        return view;
    }

    const VmaAllocation& Image::getAllocation() const {
        return allocation;
    }

    VkFormat Image::getFormat() const {
        return format;
    }

    const VkExtent3D &Image::getExtent() const {
        return extent;
    }

    uint32_t Image::getMipLevels() const {
        return mipLevels;
    }

} // namespace lv