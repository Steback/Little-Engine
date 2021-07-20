#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_IMAGE_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_IMAGE_HPP


#include "vulkan/vulkan.hpp"
#include "../extras/vk_mem_alloc.hpp"

namespace lve {

    class SwapChain;

    class Image {
        friend SwapChain;

    public:
        Image();

        Image(vma::Allocator allocator, const vk::ImageCreateInfo& createInfo, vma::MemoryUsage memoryUsage);

        ~Image();

        void createView(vk::Device device, vk::ImageAspectFlagBits aspectFlags);

        void destroy(vk::Device device, vma::Allocator allocator);

        [[nodiscard]] const vk::Image &getHandle() const;

        [[nodiscard]] const vk::ImageView &getView() const;

        [[nodiscard]] vk::Format getFormat() const;

        [[nodiscard]] const vk::Extent3D &getExtent() const;

        [[nodiscard]] uint32_t getMipLevels() const;

    private:
        vk::Image handle{};
        vk::ImageView view{};
        vk::Format format{};
        vma::Allocation allocation{};
        vk::DeviceMemory memory{};
        vk::Extent3D extent{};
        uint32_t mipLevels{1};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_RENDER_IMAGE_HPP
