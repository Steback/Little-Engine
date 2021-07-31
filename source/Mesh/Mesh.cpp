#include "Mesh.hpp"

#include "graphics/Device.hpp"
#include "graphics/Buffer.hpp"
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

    Mesh::Mesh(VmaAllocator allocator, const std::vector<Vertex> &vertices) : allocator(allocator) {
        createVertexBuffer(vertices);
    }

    Mesh::~Mesh() = default;

    void Mesh::destroy() {
        vertexBuffer->destroy();
    }

    void Mesh::bind(VkCommandBuffer commandBuffer) {
        VkBuffer buffers[] = { vertexBuffer->getBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

    void Mesh::draw(VkCommandBuffer commandBuffer) const {
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }

    void Mesh::createVertexBuffer(const std::vector<Vertex> &vertices) {
        vertexCount = CAST_U32(vertices.size());
        VkDeviceSize size = sizeof(vertices[0]) * vertexCount;
        vertexBuffer = std::make_unique<Buffer>(allocator);
        vertexBuffer->allocateMemory(size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

        vertexBuffer->map();
        vertexBuffer->copyTo(vertices.data());
        vertexBuffer->unmap();
    }

} // namespace lv