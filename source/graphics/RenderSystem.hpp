#ifndef LITTLEVULKANENGINE_RENDERSYSTEM_HPP
#define LITTLEVULKANENGINE_RENDERSYSTEM_HPP


#include <memory>

#include "vulkan/vulkan.h"

#include "utils/NonCopyable.hpp"
#include "scene/Scene.hpp"


namespace lve {

    class Device;
    class GraphicsPipeline;

    class RenderSystem : NonCopyable {
    public:
        RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass);

        ~RenderSystem() override;

        void destroy();

        void renderEntities(VkCommandBuffer commandBuffer, entt::registry& registry);

    private:
        std::shared_ptr<Device> device;
        std::unique_ptr<GraphicsPipeline> pipeline;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_RENDERSYSTEM_HPP
