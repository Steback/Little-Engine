#ifndef LITTLEVULKANENGINE_MESH_HPP
#define LITTLEVULKANENGINE_MESH_HPP


#include <memory>
#include <vector>

#include "vk_mem_alloc.h"

#include "math/Vector3.hpp"
#include "utils/NonCopyable.hpp"


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

    public:
        Mesh(VmaAllocator allocator, const std::vector<Vertex>& vertices);

        ~Mesh() override;

        void destroy();

        void bind(VkCommandBuffer commandBuffer);

        void draw(VkCommandBuffer commandBuffer) const;

    private:
        void createVertexBuffer(const std::vector<Vertex>& vertices);

    private:
        VmaAllocator allocator;
        std::unique_ptr<Buffer> vertexBuffer;
        uint32_t vertexCount{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_MESH_HPP
