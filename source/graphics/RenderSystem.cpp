#include "RenderSystem.hpp"

#include <cmath>

#include "fmt/format.h"

#include "Device.hpp"
#include "pipelines/GraphicsPipeline.hpp"
#include "Camera.hpp"
#include "math/Common.hpp"
#include "math/Matrix4.hpp"
#include "math/Vector3.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshInterface.hpp"


namespace lve {

    struct SimplePushConstantData {
        mat4 transform{1.f};
        alignas(16) vec3 color{};
    };

    RenderSystem::RenderSystem(std::shared_ptr<Device> device, VkRenderPass renderPass)
            : device(std::move(device)) {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        Pipeline::ConfigInfo configInfo;
        GraphicsPipeline::defaultConfigInfo(configInfo, renderPass);
        pipeline = std::make_unique<GraphicsPipeline>(
                this->device->getDevice(),
                "model.vert.spv", "model.frag.spv",
                configInfo,
                std::vector<VkPushConstantRange>{pushConstantRange}
        );
    }

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::destroy() {
        pipeline->destroy();
    }

    void RenderSystem::renderEntities(VkCommandBuffer commandBuffer, entt::registry& registry, const Camera& camera) {
        pipeline->bind(commandBuffer);

        for (auto& entity : registry.view<Transform, MeshInterface>()) {
            auto& transform = registry.get<Transform>(entity);
            auto& meshInterface = registry.get<MeshInterface>(entity);
            auto viewProjection = camera.getProjection() * camera.getView();

            SimplePushConstantData push{};
            push.color = {0.2f, 0.0f, 0.02f};
            push.transform = viewProjection * transform.worldTransform();

            vkCmdPushConstants(
                    commandBuffer,
                    pipeline->getLayout(),
                    VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                    0,
                    sizeof(SimplePushConstantData),
                    &push);

            meshInterface.bind(commandBuffer);
            meshInterface.draw(commandBuffer);
        }
    }

} // namespace lve