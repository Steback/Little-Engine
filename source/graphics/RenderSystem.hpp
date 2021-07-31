#ifndef LITTLEVULKANENGINE_RENDERSYSTEM_HPP
#define LITTLEVULKANENGINE_RENDERSYSTEM_HPP


#include <memory>

#include "vulkan/vulkan.h"

#include "utils/NonCopyable.hpp"
#include "math/Vector3.hpp"
#include "math/Matrix4.hpp"


namespace lve {

    class Device;
    class GraphicsPipeline;
    class Mesh;

    class RenderSystem : NonCopyable {
    public:
        RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass);

        ~RenderSystem() override;

        void destroy();

        void renderEntities(VkCommandBuffer commandBuffer, Mesh* mesh);

    private:
        [[nodiscard]] mat4 worldTransform() const;

    private:
        std::shared_ptr<Device> device;
        std::unique_ptr<GraphicsPipeline> pipeline;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_RENDERSYSTEM_HPP
