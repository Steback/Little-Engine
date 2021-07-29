#ifndef LITTLEVULKANENGINE_APP_HPP
#define LITTLEVULKANENGINE_APP_HPP


#include <memory>

#include "utils/NonCopyable.hpp"
#include "config/Config.hpp"


namespace lve {

    class Window;
    class Renderer;

    class App : NonCopyable {
    public:
        App();

        ~App() override;

        void start();

        void loop();

        void shutdown();

        void run();

    private:
        Config config;
        std::shared_ptr<Window> window;
        std::unique_ptr<Renderer> renderer;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_APP_HPP
