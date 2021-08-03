#include "CameraMovement.hpp"

#include "entity/components/Transform.hpp"


namespace lve {

    CameraMovement::CameraMovement() {
        cameraID = scene->searchEntity("camera")->getId();
    }

    CameraMovement::~CameraMovement() = default;

    void CameraMovement::update(float deltaTime) {
        auto& transform = scene->getEntity(cameraID)->getComponent<Transform>();

        cameraController.moveInPlaneZX(input, deltaTime, transform);
        camera.setViewYXZ(transform.getTranslation(), transform.getRotation());
    }

} // namespace lv