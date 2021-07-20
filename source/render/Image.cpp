#include "Image.hpp"


namespace lve {

    Image::Image() = default;

    Image::Image(vk::Device device, const vk::ImageCreateInfo &createInfo) {
        format = createInfo.format;
        mipLevels = createInfo.mipLevels;
        extent = createInfo.extent;
        handle = device.createImage(createInfo);
    }

    Image::~Image() = default;

    void Image::bind(vk::Device device, uint32_t memoryTypeIndex, vk::DeviceSize size, vk::ImageAspectFlagBits aspectFlags) {
        memory = device.allocateMemory({size, memoryTypeIndex});

        device.bindImageMemory(handle, memory, 0);

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

    void Image::destroy(vk::Device device) {
        device.destroy(view);
        device.destroy(handle);
        device.free(memory);
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

    const vk::DeviceMemory &Image::getMemory() const {
        return memory;
    }

    const vk::Extent3D &Image::getExtent() const {
        return extent;
    }

    uint32_t Image::getMipLevels() const {
        return mipLevels;
    }

} // namespace lve