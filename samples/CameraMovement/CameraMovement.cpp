#include "CameraMovement.hpp"

#include "entity/components/Transform.hpp"
#include "Mesh/Mesh.hpp"
#include "entity/components/MeshInterface.hpp"
#include "assets/AssetsManager.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/RenderSystem.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshInterface.hpp"
#include "math/Matrix3.hpp"


namespace lve {

    struct SimplePushConstantData {
        mat4 transform{1.f};
        mat4 normalMatrix{1.f};
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
                "model",
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
        auto modelMatrix = transform.worldTransform();
        push.transform = viewProjection * modelMatrix;
        push.normalMatrix = mat4(transform.normalMatrix());

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