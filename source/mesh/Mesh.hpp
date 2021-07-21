#ifndef LITTLEVULKANENGINE_SOURCE_MESH_MESH_HPP
#define LITTLEVULKANENGINE_SOURCE_MESH_MESH_HPP


#include <vector>
#include <memory>

#include "vulkan/vulkan.hpp"

#include "../math/Vector2.hpp"
#include "../math/Vector3.hpp"
#include "../render/Buffer.hpp"


namespace lve {

    class Device;

    class Mesh {
    public:
        struct Vertex {
            vec2 position;
            vec3 color;

            static std::vector<vk::VertexInputBindingDescription> getBindingDescriptions();

            static std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions();
        };

    public:
        Mesh(std::shared_ptr<Device> device, const std::vector<Vertex>& vertices);

        Mesh(const Mesh& mesh) = delete;

        ~Mesh();

        Mesh& operator=(const Mesh& mesh) = delete;

        void destroy();

        void bind(const vk::CommandBuffer& cmdBuffer);

        void draw(const vk::CommandBuffer& cmdBuffer) const;

    private:
        void createVertexBuffer(const std::vector<Vertex>& vertices);

    private:
        std::shared_ptr<Device> device;
        Buffer vertexBuffer;
        uint32_t vertexCount{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_MESH_MESH_HPP
