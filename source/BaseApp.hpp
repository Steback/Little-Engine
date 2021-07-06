#ifndef LITTLEVULKANENGINE_BASEAPP_HPP
#define LITTLEVULKANENGINE_BASEAPP_HPP


#include <memory>

#include "Config.hpp"


namespace lve {

    class FileManager;
    class Window;

    class BaseApp {
    public:
        BaseApp();

        ~BaseApp();

        void run();

        void loop();

        void shutdown();

        static std::unique_ptr<FileManager> fileManager;
        static std::unique_ptr<Config> config;

    private:
        std::shared_ptr<Window> window;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_BASEAPP_HPP
