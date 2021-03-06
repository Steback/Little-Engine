#ifndef LITTLEVULKANENGINE_BUFFER_HPP
#define LITTLEVULKANENGINE_BUFFER_HPP


#include <cstring>

#include "vk_mem_alloc.h"

#include "utils/NonCopyable.hpp"


namespace lve {

    class Buffer {
    public:
        Buffer();

        explicit Buffer(VmaAllocator allocator);

        ~Buffer();

        void destroy();

        void allocateMemory(VkDeviceSize size_, VkBufferUsageFlags usageFlags, VmaMemoryUsage memoryUsage);

        void map();

        template<typename T>
        void copyTo(T* data) const;

        void unmap();

        [[nodiscard]] const VkBuffer& getBuffer() const;

        [[nodiscard]] VkDeviceSize getSize() const;

    private:
        VmaAllocator allocator{};
        VkBuffer buffer{};
        VmaAllocation allocation{};
        VkDeviceSize size{};
        void* mapped{nullptr};
    };

    template<typename T>
    void Buffer::copyTo(T *data) const {
        std::memcpy(mapped, data, size);
    }

} // namespace lve


#endif //LITTLEVULKANENGINE_BUFFER_HPP
