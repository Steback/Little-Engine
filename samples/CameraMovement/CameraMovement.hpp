#ifndef LITTLEVULKANENGINE_CAMERAMOVEMENT_HPP
#define LITTLEVULKANENGINE_CAMERAMOVEMENT_HPP


#include "App.hpp"


namespace lve {

    class CameraMovement : public App {
    public:
        CameraMovement();

        ~CameraMovement() override;

        void update() override;

    private:

    };

} // namespace lve


#endif //LITTLEVULKANENGINE_CAMERAMOVEMENT_HPP
