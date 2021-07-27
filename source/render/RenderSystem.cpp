#include "RenderSystem.hpp"

#include <utility>


#include "Device.hpp"
#include "pipeline/GraphicsPipeline.hpp"
#include "../scene/components/Transform.hpp"
#include "../scene/components/MeshInterface.hpp"


namespace lve {

    RenderSystem::RenderSystem(std::shared_ptr<Device> device, vk::RenderPass renderPass) : device(std::move(device)) {
        pipeline= std::make_unique<GraphicsPipeline>(
                this->device->getLogicalDevice(),
                "model.vert.spv","model.frag.spv",
                GraphicsPipeline::defaultConfig(renderPass, {vk::DynamicState::eViewport, vk::DynamicState::eScissor})
        );
    }

    RenderSystem::~RenderSystem() {
        pipeline->destroy();
    }

    void RenderSystem::renderEntities(vk::CommandBuffer commandBuffer, const registry_t &registry) {
        pipeline->bind(commandBuffer);

        for (auto& entity : registry.view<Transform, MeshInterface>()) {
            auto& transform = registry.get<Transform>(entity);
            auto& meshInterface = registry.get<MeshInterface>(entity);

            SimplePushConstantData push{};
            push.offset = transform.translation;
            push.color = {0.2f, 0.0f, 0.2f};
            push.transform = transform.getWorldMatrix();

            commandBuffer.pushConstants(
                    pipeline->getLayout(),
                    vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment,
                    0,
                    sizeof(SimplePushConstantData),
                    &push
            );

            meshInterface.bind(commandBuffer);
            meshInterface.draw(commandBuffer);
        }
    }

} // namespace lve
