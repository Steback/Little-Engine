#include "MeshInterface.hpp"

#include "Mesh/Mesh.hpp"


namespace lve {

    MeshInterface::MeshInterface() = default;

    MeshInterface::MeshInterface(std::shared_ptr<Mesh> mesh) : mesh(std::move(mesh)) {

    }

    std::shared_ptr<Mesh> MeshInterface::getMesh() {
        return mesh;
    }

    void MeshInterface::setMesh(std::shared_ptr<Mesh> mesh_) {
        mesh = std::move(mesh_);
    }

    void MeshInterface::bind(VkCommandBuffer commandBuffer) {
        mesh->bind(commandBuffer);
    }

    void MeshInterface::draw(VkCommandBuffer commandBuffer) {
        mesh->draw(commandBuffer);
    }

} // namespace lv