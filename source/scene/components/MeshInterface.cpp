#include "MeshInterface.hpp"

#include "../../mesh/Mesh.hpp"


namespace lve {

    MeshInterface::MeshInterface(std::shared_ptr<Mesh> mesh) : mesh(std::move(mesh)) {

    }

    void MeshInterface::draw(vk::CommandBuffer cmdBuffer) {
        mesh->draw(cmdBuffer);
    }

} // namespace lve::component
