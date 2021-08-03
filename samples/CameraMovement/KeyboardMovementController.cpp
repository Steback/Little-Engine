#include "KeyboardMovementController.hpp"

#include <limits>
#include <cmath>

#include "entity/Entity.hpp"
#include "math/Vector3.hpp"
#include "input/InputSystem.hpp"
#include "entity/components/Transform.hpp"
#include "math/Common.hpp"


namespace lve {

    KeyboardMovementController::KeyboardMovementController() = default;

    void KeyboardMovementController::moveInPlaneZX(InputSystem& input, float deltaTime, Transform& transform) const {
        vec3 rotation = transform.getRotation();
        vec3 translation = transform.getTranslation();

        vec3 rotate;
        if (input.getKey(KeyMappings::lookRight)) rotate.y += 1.0f;
        if (input.getKey(KeyMappings::lookLeft)) rotate.y -= 1.0f;
        if (input.getKey(KeyMappings::lookUp)) rotate.x += 1.0f;
        if (input.getKey(KeyMappings::lookDown)) rotate.x -= 1.0f;

        if (rotate * rotate > std::numeric_limits<float>::epsilon())
            rotation += lockSpeed * deltaTime * rotate.normal();

        rotation.x = std::clamp(rotation.x, -1.5f, 1.5f);
        rotation.y = mod(rotation.y, twoPi());

        float yaw = rotation.y;
        const vec3 forwardDir{std::sin(yaw), 0.f, std::cos(yaw)};
        const vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
        const vec3 upDir{0.f, -1.f, 0.f};

        vec3 moveDir{0.f};
        if (input.getKey(KeyMappings::moveForward)) moveDir += forwardDir;
        if (input.getKey(KeyMappings::moveBackward)) moveDir -= forwardDir;
        if (input.getKey(KeyMappings::moveRight)) moveDir += rightDir;
        if (input.getKey(KeyMappings::moveLeft)) moveDir -= rightDir;
        if (input.getKey(KeyMappings::moveUp)) moveDir += upDir;
        if (input.getKey(KeyMappings::moveDown)) moveDir -= upDir;

        if (moveDir * moveDir > std::numeric_limits<float>::epsilon())
            translation += moveSpeed * deltaTime * moveDir.normal();

        transform.setTranslation(translation);
        transform.setRotation(rotation);
    }

} // namespace lve