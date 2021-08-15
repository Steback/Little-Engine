#ifndef LITTLEVULKANENGINE_RENDERSYSTEM_HPP
#define LITTLEVULKANENGINE_RENDERSYSTEM_HPP


#include <memory>
#include <functional>

#include "vulkan/vulkan.h"

#include "utils/NonCopyable.hpp"
#include "scene/Scene.hpp"


namespace lve {

    class Device;
    class GraphicsPipeline;
    class Camera;

    class RenderSystem : NonCopyable {
    public:
        RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass, const std::string& shadersName,
                     const std::vector<VkPushConstantRange>& pushConstantsRanges);

        ~RenderSystem() override;

        void destroy();

        void renderEntities(VkCommandBuffer commandBuffer, const Camera& camera, entt::registry & registry,
                            const std::function<void(VkPipelineLayout, VkCommandBuffer, id_t)>& drawFunction);

    private:
        std::shared_ptr<Device> device;
        std::unique_ptr<GraphicsPipeline> pipeline;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_RENDERSYSTEM_HPP
