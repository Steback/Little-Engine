#ifndef LITTLEVULKANENGINE_APP_HPP
#define LITTLEVULKANENGINE_APP_HPP


#include <memory>

#include "utils/NonCopyable.hpp"
#include "config/Config.hpp"
#include "graphics/Camera.hpp"
#include "input/InputSystem.hpp"


namespace lve {

    class Window;
    class Renderer;
    class Scene;
    class AssetsManager;

    class App : NonCopyable {
    public:
        explicit App(const std::string& appName);

        ~App() override;

        void loop();

        virtual void start() = 0;

        virtual void update(float deltaTime) = 0;

        void shutdown();

        void run();

    protected:
        Config config;
        Camera camera;
        InputSystem input;
        std::shared_ptr<Window> window;
        std::unique_ptr<Renderer> renderer;
        std::unique_ptr<Scene> scene;
        std::unique_ptr<AssetsManager> assetsManager;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_APP_HPP
