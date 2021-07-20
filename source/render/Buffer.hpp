#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_BUFFER_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_BUFFER_HPP


#include "vulkan/vulkan.hpp"
#include "../extras/vk_mem_alloc.hpp"


namespace lve {

    class Buffer {
    public:
        Buffer();

        explicit Buffer(vma::Allocator allocator);

        ~Buffer();

        void destroy() const;

        void allocateMemory(vk::DeviceSize size, vk::BufferUsageFlags usage, vma::MemoryUsage memoryUsage);

        void map();

        void unmap();

        template<typename T>
        void copyTo(T *data) const {
            std::memcpy(mapped, data, size);
        }

    public:
        vma::Allocator allocator;
        vk::Buffer handle{};
        vma::Allocation allocation;
        vk::DeviceSize size{0};
        void* mapped{nullptr};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_RENDER_BUFFER_HPP
