#ifndef LITTLEVULKANENGINE_APP_HPP
#define LITTLEVULKANENGINE_APP_HPP


#include <memory>

#include "utils/NonCopyable.hpp"
#include "config/Config.hpp"


namespace lve {

    class Window;
    class Renderer;
    class Scene;
    class AssetsManager;

    class App : NonCopyable {
    public:
        App();

        ~App() override;

        void start();

        void loop();

        virtual void update() = 0;

        void shutdown();

        void run();

    protected:
        Config config;
        std::shared_ptr<Window> window;
        std::unique_ptr<Renderer> renderer;
        std::unique_ptr<Scene> scene;
        std::unique_ptr<AssetsManager> assetsManager;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_APP_HPP
