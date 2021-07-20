#include "Buffer.hpp"

namespace lve {

    Buffer::Buffer() = default;

    Buffer::Buffer(vk::Device device, vk::DeviceSize size, vk::BufferUsageFlags usage)
            : device(device), size(size) {
        vk::BufferCreateInfo createInfo({}, size, usage, vk::SharingMode::eExclusive);
        handle = this->device.createBuffer(createInfo);
    }

    Buffer::~Buffer() = default;

    void Buffer::destroy() const {
        if (handle) device.destroy(handle);

        if (memory) device.freeMemory(memory);
    }

    void Buffer::allocateMemory(vk::DeviceSize size_, uint32_t memoryTypeIndex) {
        vk::MemoryAllocateInfo allocateInfo(size_, memoryTypeIndex);
        memory = device.allocateMemory(allocateInfo);
    }

    void Buffer::bind(vk::DeviceSize offset) const {
        device.bindBufferMemory(handle, memory, offset);
    }

    void Buffer::map(vk::DeviceSize size_, vk::DeviceSize offset) {
        if (size_ != VK_WHOLE_SIZE && size != size_)
            size = size_;

        mapped = device.mapMemory(memory, offset, size);
    }

    void Buffer::unmap() {
        if (mapped) {
            device.unmapMemory(memory);
            mapped = nullptr;
        }
    }

    void Buffer::setupDescriptor(vk::DeviceSize offset) {
        descriptor.offset = offset;
        descriptor.buffer = handle;
        descriptor.range = size;
    }

    vk::Result Buffer::flush(vk::DeviceSize offset) const {
        vk::MappedMemoryRange mappedRange{memory, offset, size};

        return device.flushMappedMemoryRanges(1, &mappedRange);
    }

} // namespace lve
