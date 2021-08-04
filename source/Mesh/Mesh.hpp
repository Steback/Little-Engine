#ifndef LITTLEVULKANENGINE_MESH_HPP
#define LITTLEVULKANENGINE_MESH_HPP


#include <memory>
#include <vector>

#include "vk_mem_alloc.h"

#include "math/Vector3.hpp"
#include "utils/NonCopyable.hpp"
#include "graphics/Buffer.hpp"


namespace lve {

    class Device;
    class Buffer;

    class Mesh : NonCopyable {
    public:
        struct Vertex {
            vec3 position;
            vec3 color;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();

            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        struct Data {
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
        };

    public:
        Mesh(std::shared_ptr<Device> device, const Data& data);

        ~Mesh() override;

        void destroy();

        void bind(VkCommandBuffer commandBuffer);

        void draw(VkCommandBuffer commandBuffer) const;

    private:
        void createVertexBuffer(const std::vector<Vertex>& vertices);

        void createIndexBuffer(const std::vector<uint32_t>& indices);

    private:
        std::shared_ptr<Device> device;
        Buffer vertexBuffer;
        Buffer indexBuffer;
        uint32_t vertexCount{};
        uint32_t indexCount{};
        bool hasIndexBuffer{false};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_MESH_HPP
