#ifndef LITTLEVULKANENGINE_KEYBOARDMOVEMENTCONTROLLER_HPP
#define LITTLEVULKANENGINE_KEYBOARDMOVEMENTCONTROLLER_HPP


#include "GLFW/glfw3.h"


namespace lve {

    class Transform;
    class InputSystem;

    class KeyboardMovementController {
    public:
        enum KeyMappings {
             moveLeft = GLFW_KEY_A,
             moveRight = GLFW_KEY_D,
             moveForward = GLFW_KEY_W,
             moveBackward = GLFW_KEY_S,
             moveUp = GLFW_KEY_E,
             moveDown = GLFW_KEY_Q,
             lookLeft = GLFW_KEY_LEFT,
             lookRight = GLFW_KEY_RIGHT,
             lookUp = GLFW_KEY_UP,
             lookDown = GLFW_KEY_DOWN
        };

    public:
        KeyboardMovementController();

        void moveInPlaneZX(InputSystem& input, float deltaTime, Transform& transform) const;

    public:
        float moveSpeed{3.0f};
        float lockSpeed{1.5f};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_KEYBOARDMOVEMENTCONTROLLER_HPP
