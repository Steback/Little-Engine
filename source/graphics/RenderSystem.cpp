#include "RenderSystem.hpp"

#include <utility>

#include "Device.hpp"
#include "pipelines/GraphicsPipeline.hpp"
#include "Camera.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshInterface.hpp"


namespace lve {

    RenderSystem::RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass, const std::string& shadersName,
                               const std::vector<VkPushConstantRange>& pushConstantsRanges)
            : device(std::move(device)) {

        Pipeline::ConfigInfo configInfo;
        GraphicsPipeline::defaultConfigInfo(configInfo, renderPass);
        pipeline = std::make_unique<GraphicsPipeline>(
                this->device->getDevice(),
                shadersName + ".vert.spv", shadersName + ".frag.spv",
                configInfo,
                pushConstantsRanges
        );
    }

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::destroy() {
        pipeline->destroy();
    }

    void RenderSystem::renderEntities(VkCommandBuffer commandBuffer, const Camera& camera, entt::registry & registry,
                                      const std::function<void(VkPipelineLayout, VkCommandBuffer, id_t)>& drawFunction) {
        pipeline->bind(commandBuffer);

        for (auto& entity : registry.view<Transform, MeshInterface>()) {
            drawFunction(pipeline->getLayout(), commandBuffer, entity);
        }
    }

} // namespace lve