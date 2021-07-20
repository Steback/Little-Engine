#include "Buffer.hpp"

namespace lve {

    Buffer::Buffer() = default;

    Buffer::Buffer(vma::Allocator allocator) : allocator(allocator) {

    }

    Buffer::~Buffer() = default;

    void Buffer::destroy() const {
        allocator.destroyBuffer(handle, allocation);
    }

    void Buffer::allocateMemory(vk::DeviceSize size_, vk::BufferUsageFlags usage, vma::MemoryUsage memoryUsage) {
        size = size_;
        vk::BufferCreateInfo bufferCreateInfo({}, size, usage, vk::SharingMode::eExclusive);
        vma::AllocationCreateInfo allocationCreateInfo({}, memoryUsage);

        allocator.createBuffer(&bufferCreateInfo, &allocationCreateInfo, &handle, &allocation, nullptr);
    }

    void Buffer::map() {
        mapped = allocator.mapMemory(allocation);
    }

    void Buffer::unmap() {
        if (mapped) {
            allocator.unmapMemory(allocation);
            mapped = nullptr;
        }
    }

} // namespace lve
