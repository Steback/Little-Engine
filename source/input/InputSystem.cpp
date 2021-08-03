#include "InputSystem.hpp"


namespace lve {

    InputSystem::InputSystem() = default;

    InputSystem::~InputSystem() = default;

    bool InputSystem::setKey(std::size_t key, bool value) {
        return keys[key] = value;
    }

    bool InputSystem::getKey(std::size_t key) {
        return keys[key];
    }

} // namespace lv