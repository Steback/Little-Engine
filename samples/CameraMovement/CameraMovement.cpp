#include "CameraMovement.hpp"

#include "entity/components/Transform.hpp"
#include "Mesh/Mesh.hpp"
#include "entity/components/MeshInterface.hpp"
#include "assets/AssetsManager.hpp"
#include "graphics/Renderer.hpp"


namespace lve {

    CameraMovement::CameraMovement() : App("Camera Movement") {
        cameraID = scene->searchEntity("camera")->getId();
    }

    CameraMovement::~CameraMovement() = default;

    void CameraMovement::start() {
        Entity* entity = scene->addEntity("cube");
        entity->addComponent<Transform>(vec3(0.0f, 0.0f, 2.5f), vec3(), vec3(0.5f, 0.5f, 0.5f));
        entity->addComponent<MeshInterface>(assetsManager->addMesh("models/colored_cube.obj"));

        renderer->setupDrawResources();
    }

    void CameraMovement::update(float deltaTime) {
        auto& transform = scene->getEntity(cameraID)->getComponent<Transform>();

        cameraController.moveInPlaneZX(input, deltaTime, transform);
        camera.setViewYXZ(transform.getTranslation(), transform.getRotation());
    }

} // namespace lv