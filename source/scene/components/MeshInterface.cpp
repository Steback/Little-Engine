#include "MeshInterface.hpp"

#include "../../mesh/Mesh.hpp"


namespace lve {

    MeshInterface::MeshInterface(std::shared_ptr<Mesh> mesh) : mesh(std::move(mesh)) {

    }

    void MeshInterface::bind(vk::CommandBuffer cmdBuffer) const {
        mesh->bind(cmdBuffer);
    }

    void MeshInterface::draw(vk::CommandBuffer cmdBuffer) const {
        mesh->draw(cmdBuffer);
    }

} // namespace lve::component
