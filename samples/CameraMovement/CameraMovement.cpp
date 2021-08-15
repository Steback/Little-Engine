#include "CameraMovement.hpp"

#include "entity/components/Transform.hpp"
#include "Mesh/Mesh.hpp"
#include "entity/components/MeshInterface.hpp"
#include "assets/AssetsManager.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/RenderSystem.hpp"


namespace lve {

    struct SimplePushConstantData {
        mat4 transform{1.f};
        alignas(16) vec3 color{};
    };

    CameraMovement::CameraMovement() : App("Camera Movement") {

    }

    CameraMovement::~CameraMovement() = default;

    void CameraMovement::start() {
        scene->load("CameraMovement/scene.json");
        cameraID = scene->searchEntity("camera")->getId();

        renderer->setupDrawResources();

        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        renderSystem = std::make_unique<RenderSystem>(
                renderer->getDevice(),
                renderer->getRenderPass(),
                "CameraMovement",
                std::vector<VkPushConstantRange>{pushConstantRange}
        );
    }

    void CameraMovement::onUpdate(float deltaTime) {
        auto& transform = scene->getEntity(cameraID)->getComponent<Transform>();

        cameraController.moveInPlaneZX(input, deltaTime, transform);
        camera.setViewYXZ(transform.getTranslation(), transform.getRotation());
    }

    void CameraMovement::onDrawEntity(VkPipelineLayout layout, VkCommandBuffer commandBuffer, id_t entityID) {
        auto entity = scene->getEntity(entityID);
        auto& transform = entity->getComponent<Transform>();
        auto& meshInterface = entity->getComponent<MeshInterface>();
        auto viewProjection = camera.getProjection() * camera.getView();

        SimplePushConstantData push{};
        push.color = {0.2f, 0.0f, 0.02f};
        push.transform = viewProjection * transform.worldTransform();

        vkCmdPushConstants(
                commandBuffer,
                layout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push);

        meshInterface.bind(commandBuffer);
        meshInterface.draw(commandBuffer);
    }

} // namespace lv