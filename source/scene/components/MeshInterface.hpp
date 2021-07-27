#ifndef LITTLEVULKANENGINE_COMPONENT_MESH_INTERFACE_HPP
#define LITTLEVULKANENGINE_COMPONENT_MESH_INTERFACE_HPP


#include <memory>

#include "vulkan/vulkan.hpp"


namespace lve {

    class Mesh;

    class MeshInterface {
    public:
        explicit MeshInterface(std::shared_ptr<Mesh> mesh);

        void draw(vk::CommandBuffer cmdBuffer);

    private:
        std::shared_ptr<Mesh> mesh;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_COMPONENT_MESH_INTERFACE_HPP
