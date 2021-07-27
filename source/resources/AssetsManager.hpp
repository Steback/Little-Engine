#ifndef LITTLEVULKANENGINE_ASSETSMANAGER_HPP
#define LITTLEVULKANENGINE_ASSETSMANAGER_HPP


#include <unordered_map>
#include <memory>
#include <cstdint>
#include <vector>

#include "vulkan/vulkan.hpp"

#include "../mesh/Mesh.hpp"


namespace lve {

    class Device;

    class AssetsManager {
    public:
        explicit AssetsManager(std::shared_ptr<Device> device);

        void cleanup();

        std::shared_ptr<Mesh> addMesh(uint32_t id, const std::vector<Mesh::Vertex>& vertices);

        std::shared_ptr<Mesh> getMesh(uint32_t id);

        void bindMeshes(vk::CommandBuffer commandBuffer);

    private:
        std::shared_ptr<Device> device;
        std::unordered_map<uint32_t, std::shared_ptr<Mesh>> meshes;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_ASSETSMANAGER_HPP
