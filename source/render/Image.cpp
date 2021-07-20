#include "Image.hpp"


namespace lve {

    Image::Image() = default;

    Image::Image(vma::Allocator allocator, const vk::ImageCreateInfo &createInfo, vma::MemoryUsage memoryUsage) {
        format = createInfo.format;
        mipLevels = createInfo.mipLevels;
        extent = createInfo.extent;

        vma::AllocationCreateInfo allocationCreateInfo({}, memoryUsage);
        allocator.createImage(&createInfo, &allocationCreateInfo, &handle, &allocation, nullptr);
    }

    Image::~Image() = default;

    void Image::createView(vk::Device device, vk::ImageAspectFlagBits aspectFlags) {
        vk::ImageViewCreateInfo viewCreateInfo(
                {}, // flags
                handle,
                vk::ImageViewType::e2D,
                format,
                {}, // Components
                {aspectFlags, 0, mipLevels, 0, 1} // subresourceRange
        );

        view = device.createImageView(viewCreateInfo);
    }

    void Image::destroy(vk::Device device, vma::Allocator allocator) {
        device.destroy(view);
        allocator.destroyImage(handle, allocation);
    }

    const vk::Image &Image::getHandle() const {
        return handle;
    }

    const vk::ImageView &Image::getView() const {
        return view;
    }

    vk::Format Image::getFormat() const {
        return format;
    }

    const vk::Extent3D &Image::getExtent() const {
        return extent;
    }

    uint32_t Image::getMipLevels() const {
        return mipLevels;
    }

} // namespace lve