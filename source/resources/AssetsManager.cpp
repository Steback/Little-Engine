#include "AssetsManager.hpp"


namespace lve {

    AssetsManager::AssetsManager(std::shared_ptr<Device> device) : device(std::move(device)) {

    }

    void AssetsManager::cleanup() {
        for (auto& [id, mesh] : meshes)
            mesh->destroy();
    }

    std::shared_ptr<Mesh> AssetsManager::addMesh(uint32_t id, const std::vector<Mesh::Vertex> &vertices) {
        meshes[id] = std::make_shared<Mesh>(device, vertices);
        return meshes[id];
    }

    std::shared_ptr<Mesh> AssetsManager::getMesh(uint32_t id) {
        return meshes[id];
    }

    void AssetsManager::bindMeshes(vk::CommandBuffer commandBuffer) {
        for (auto& [id, mesh] : meshes)
            mesh->bind(commandBuffer);
    }

} // namespace lve
