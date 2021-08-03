#ifndef LITTLEVULKANENGINE_INPUTSYSTEM_HPP
#define LITTLEVULKANENGINE_INPUTSYSTEM_HPP


#include <array>

#include "GLFW/glfw3.h"

#include "utils/NonCopyable.hpp"


namespace lve {

    enum MouseState {
        RELEASE = GLFW_RELEASE,
        PRESS = GLFW_PRESS
    };

    class InputSystem : public NonCopyable {
    public:
        InputSystem();

        ~InputSystem() override;

        bool setKey(std::size_t key, bool value);

        bool getKey(std::size_t key);

    private:
        std::array<bool, 512> keys{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_INPUTSYSTEM_HPP
