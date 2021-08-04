#ifndef LITTLEVULKANENGINE_ASSETSMANAGER_HPP
#define LITTLEVULKANENGINE_ASSETSMANAGER_HPP


#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "utils/NonCopyable.hpp"
#include "Mesh/Mesh.hpp"


namespace lve {

    class Device;

    class AssetsManager : NonCopyable {
    public:
        explicit AssetsManager(std::shared_ptr<Device> device);

        ~AssetsManager() override;

        void cleanup();

        std::shared_ptr<Mesh> addMesh(const std::string& id, const Mesh::Data& data);

        std::shared_ptr<Mesh> getMesh(const std::string& id);

    private:
        std::shared_ptr<Device> device;
        // TODO: Change mesh id
        std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_ASSETSMANAGER_HPP
