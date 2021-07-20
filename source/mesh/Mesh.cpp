#include "Mesh.hpp"

#include "../tools/Tools.hpp"
#include "../render/Device.hpp"


namespace lve {

    std::vector<vk::VertexInputBindingDescription> Mesh::Vertex::getBindingDescriptions() {
        return {
                {0, sizeof(Vertex), vk::VertexInputRate::eVertex}
        };
    }

    std::vector<vk::VertexInputAttributeDescription> Mesh::Vertex::getAttributeDescriptions() {
        return {
                {0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, position)}
        };
    }

    Mesh::Mesh(std::shared_ptr<Device> device, const std::vector<Vertex> &vertices) : device(std::move(device)) {
        createVertexBuffer(vertices);
    }

    Mesh::~Mesh() = default;

    void Mesh::destroy() {
        vertexBuffer.destroy();
    }

    void Mesh::bind(const vk::CommandBuffer& cmdBuffer) {
        vk::Buffer buffers[] = {vertexBuffer.handle};
        vk::DeviceSize  offsets[] = {0};
        cmdBuffer.bindVertexBuffers(0, 1, buffers, offsets);
    }

    void Mesh::draw(const vk::CommandBuffer &cmdBuffer) const {
        cmdBuffer.draw(vertexCount, 1, 0, 0);
    }

    void Mesh::createVertexBuffer(const std::vector<Vertex> &vertices) {
        vertexCount = castU32(vertices.size());
        vk::DeviceSize size = sizeof(vertices[0]) * vertexCount;
        vertexBuffer = device->createBuffer(
                size,
                vk::BufferUsageFlagBits::eVertexBuffer,
                vma::MemoryUsage::eCpuToGpu
        );

        vertexBuffer.map();
        vertexBuffer.copyTo(vertices.data());
        vertexBuffer.unmap();
    }


} // namespace lve
