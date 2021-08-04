#ifndef LITTLEVULKANENGINE_CAMERAMOVEMENT_HPP
#define LITTLEVULKANENGINE_CAMERAMOVEMENT_HPP


#include "App.hpp"

#include "KeyboardMovementController.hpp"
#include "entity/Entity.hpp"


namespace lve {

    class CameraMovement : public App {
    public:
        CameraMovement();

        ~CameraMovement() override;

        void start() override;

        void update(float deltaTime) override;

    private:
        KeyboardMovementController cameraController;
        id_t cameraID;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_CAMERAMOVEMENT_HPP
