#ifndef LITTLEVULKANENGINE_BASEAPP_HPP
#define LITTLEVULKANENGINE_BASEAPP_HPP


#include <memory>


namespace CLI {
    class App;
}

namespace lve {

    class Window;
    class Config;
    class RenderEngine;
    class Scene;
    class AssetsManager;

    class BaseApp {
    public:
        explicit BaseApp(CLI::App& cli);

        ~BaseApp();

        void run();

        void loop();

        void shutdown();

        static std::unique_ptr<Config> config;

    private:
        std::shared_ptr<Window> window;
        std::unique_ptr<RenderEngine> renderEngine;
        std::unique_ptr<Scene> scene;
        std::unique_ptr<AssetsManager> assetsManager;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_BASEAPP_HPP
