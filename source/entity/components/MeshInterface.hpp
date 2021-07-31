#ifndef LITTLEVULKANENGINE_MESHINTERFACE_HPP
#define LITTLEVULKANENGINE_MESHINTERFACE_HPP


#include <memory>

#include "vulkan/vulkan.h"


namespace lve {

    class Mesh;

    class MeshInterface {
    public:
        MeshInterface();

        explicit MeshInterface(std::shared_ptr<Mesh> mesh);

        std::shared_ptr<Mesh> getMesh();

        void setMesh(std::shared_ptr<Mesh> mesh_);

        void bind(VkCommandBuffer commandBuffer);

        void draw(VkCommandBuffer commandBuffer);

    public:
        const static uint32_t type = 0x02;

    private:
        std::shared_ptr<Mesh> mesh{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_MESHINTERFACE_HPP
