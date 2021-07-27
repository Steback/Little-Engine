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
    class Mesh;
    class Scene;

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
        std::unique_ptr<Mesh> model;
        std::unique_ptr<Scene> scene;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_BASEAPP_HPP
