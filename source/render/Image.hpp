#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_IMAGE_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_IMAGE_HPP


#include "vulkan/vulkan.hpp"


namespace lve {

    class SwapChain;

    class Image {
        friend SwapChain;

    public:
        Image();

        Image(vk::Device device, const vk::ImageCreateInfo& createInfo);

        ~Image();

        void bind(vk::Device device, uint32_t memoryTypeIndex, vk::DeviceSize size, vk::ImageAspectFlagBits aspectFlags);

        void destroy(vk::Device device);

        [[nodiscard]] const vk::Image &getHandle() const;

        [[nodiscard]] const vk::ImageView &getView() const;

        [[nodiscard]] vk::Format getFormat() const;

        [[nodiscard]] const vk::DeviceMemory &getMemory() const;

        [[nodiscard]] const vk::Extent3D &getExtent() const;

        [[nodiscard]] uint32_t getMipLevels() const;

    private:
        vk::Image image{};
        vk::ImageView view{};
        vk::Format format{};
        vk::DeviceMemory memory{};
        vk::Extent3D extent{};
        uint32_t mipLevels{1};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_RENDER_IMAGE_HPP
