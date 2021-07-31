#include "RenderSystem.hpp"

#include <cmath>

#include "Device.hpp"
#include "pipelines/GraphicsPipeline.hpp"
#include "Mesh/Mesh.hpp"
#include "math/Common.hpp"


namespace lve {

    static vec3 translation = {0.0f, 0.0f, 0.5f};
    static vec3 rotation;
    static vec3 scale = {0.5f, 0.5f, 0.5f};

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

    void RenderSystem::renderEntities(VkCommandBuffer commandBuffer, Mesh* mesh) {
        pipeline->bind(commandBuffer);
        rotation.y = mod(rotation.y + 0.01f, twoPi());
        rotation.x = mod(rotation.x + 0.005f, twoPi());

        SimplePushConstantData push{};
        push.color = {0.2f, 0.0f, 0.02f};
        push.transform = worldTransform();

        vkCmdPushConstants(
                commandBuffer,
                pipeline->getLayout(),
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push);

        mesh->bind(commandBuffer);
        mesh->draw(commandBuffer);
    }


    // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
    // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
    // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
    mat4 RenderSystem::worldTransform() const {
        const float c3 = std::cos(rotation.z);
        const float s3 = std::sin(rotation.z);
        const float c2 = std::cos(rotation.x);
        const float s2 = std::sin(rotation.x);
        const float c1 = std::cos(rotation.y);
        const float s1 = std::sin(rotation.y);
        return mat4{
            {
                scale.x * (c1 * c3 + s1 * s2 * s3),
                scale.x * (c2 * s3),
                scale.x * (c1 * s2 * s3 - c3 * s1),
                0.0f,
            },{
                scale.y * (c3 * s1 * s2 - c1 * s3),
                scale.y * (c2 * c3),
                scale.y * (c1 * c3 * s2 + s1 * s3),
                0.0f,
            },{
                scale.z * (c2 * s1),
                scale.z * (-s2),
                scale.z * (c1 * c2),
                0.0f,
            }, {
                translation.x, translation.y, translation.z, 1.0f
            }
        };
    }

} // namespace lv