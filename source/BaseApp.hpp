#ifndef LITTLEVULKANENGINE_BASEAPP_HPP
#define LITTLEVULKANENGINE_BASEAPP_HPP


#include <memory>


namespace CLI {
    class App;
}

namespace lve {

    class Window;
    class Config;
    class Device;
    class GraphicsPipeline;

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
        std::shared_ptr<Device> device;
        std::unique_ptr<GraphicsPipeline> graphicsPipeline;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_BASEAPP_HPP
