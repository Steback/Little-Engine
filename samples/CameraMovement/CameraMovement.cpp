#include "CameraMovement.hpp"

#include "entity/components/Transform.hpp"
#include "Mesh/Mesh.hpp"
#include "entity/components/MeshInterface.hpp"
#include "assets/AssetsManager.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/RenderSystem.hpp"


namespace lve {

    CameraMovement::CameraMovement() : App("Camera Movement") {

    }

    CameraMovement::~CameraMovement() = default;

    void CameraMovement::start() {
        scene->load("CameraMovement/scene.json");
        cameraID = scene->searchEntity("camera")->getId();

        renderer->setupDrawResources();

        renderSystem = std::make_unique<RenderSystem>(renderer->getDevice(), renderer->getRenderPass());
    }

    void CameraMovement::onUpdate(float deltaTime) {
        auto& transform = scene->getEntity(cameraID)->getComponent<Transform>();

        cameraController.moveInPlaneZX(input, deltaTime, transform);
        camera.setViewYXZ(transform.getTranslation(), transform.getRotation());
    }

} // namespace lv