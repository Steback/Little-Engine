#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_BUFFER_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_BUFFER_HPP


#include "vulkan/vulkan.hpp"


namespace lve {

    class Buffer {
    public:
        Buffer();

        Buffer(vk::Device device, vk::DeviceSize size, vk::BufferUsageFlags usage);

        ~Buffer();

        void destroy() const;

        void allocateMemory(vk::DeviceSize size, uint32_t memoryTypeIndex);

        void bind(vk::DeviceSize offset = 0) const;

        void map(vk::DeviceSize size = VK_WHOLE_SIZE, vk::DeviceSize offset = 0);

        void unmap();

        void setupDescriptor(vk::DeviceSize offset = 0);

        template<typename T>
        void copyTo(T *data) const {
            std::memcpy(mapped, data, size);
        }

        [[nodiscard]] vk::Result flush(vk::DeviceSize offset = 0) const;

    public:
        vk::Device device{};
        vk::Buffer handle{};
        vk::DeviceMemory memory{};
        vk::DescriptorBufferInfo descriptor{};
        vk::DescriptorSet descriptorSet{};
        vk::DeviceSize size{0};
        void* mapped{nullptr};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_RENDER_BUFFER_HPP
