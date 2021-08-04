#include "Mesh.hpp"

#include <utility>

#include "graphics/Device.hpp"
#include "utils/Macros.hpp"


namespace lve {

    std::vector<VkVertexInputBindingDescription> Mesh::Vertex::getBindingDescriptions() {
        return {
            { 0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
        };
    }

    std::vector<VkVertexInputAttributeDescription> Mesh::Vertex::getAttributeDescriptions() {
        return {
            { 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position) },
            { 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color) }
        };
    }

    Mesh::Mesh(std::shared_ptr<Device> device, const std::vector<Vertex> &vertices) : device(std::move(device)) {
        createVertexBuffer(vertices);
    }

    Mesh::~Mesh() = default;

    void Mesh::destroy() {
        vertexBuffer.destroy();
    }

    void Mesh::bind(VkCommandBuffer commandBuffer) {
        VkBuffer buffers[] = { vertexBuffer.getBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

    void Mesh::draw(VkCommandBuffer commandBuffer) const {
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }

    void Mesh::createVertexBuffer(const std::vector<Vertex> &vertices) {
        vertexCount = CAST_U32(vertices.size());
        VkDeviceSize size = sizeof(vertices[0]) * vertexCount;

        Buffer stagingBuffer(device->getAllocator());
        stagingBuffer.allocateMemory(size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
        stagingBuffer.map();
        stagingBuffer.copyTo(vertices.data());
        stagingBuffer.unmap();

        vertexBuffer = Buffer(device->getAllocator());
        vertexBuffer.allocateMemory(size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY);

        device->copyBuffer(stagingBuffer, vertexBuffer, size);

        stagingBuffer.destroy();
    }

} // namespace lv