#ifndef LITTLEVULKANENGINE_APP_HPP
#define LITTLEVULKANENGINE_APP_HPP


#include <memory>

#include "utils/NonCopyable.hpp"
#include "config/Config.hpp"


namespace lve {

    class FilesManager;

    class App : NonCopyable {
    public:
        App();

        ~App() override;

        void start();

        void loop();

        void run();

    private:
        std::unique_ptr<FilesManager> fileManager;
        Config config;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_APP_HPP
