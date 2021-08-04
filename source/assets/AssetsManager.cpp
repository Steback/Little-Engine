#include "AssetsManager.hpp"

#include "graphics/Device.hpp"


namespace lve {

    AssetsManager::AssetsManager(std::shared_ptr<Device> device) : device(std::move(device)) {

    }

    AssetsManager::~AssetsManager() = default;

    void AssetsManager::cleanup() {
        for (auto& [id, mesh] : meshes) mesh->destroy();
    }

    std::shared_ptr<Mesh> AssetsManager::addMesh(const std::string& id, const Mesh::Data& data) {
        return meshes[id] = std::make_shared<Mesh>(device, data);
    }

    std::shared_ptr<Mesh> AssetsManager::getMesh(const std::string &id) {
        return meshes[id];
    }

} // namespace lv