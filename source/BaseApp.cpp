#include "BaseApp.hpp"

#include "Tools.hpp"
#include "Window.hpp"
#include "FileManager.hpp"


namespace lve {

    std::unique_ptr<FileManager> BaseApp::fileManager;
    std::unique_ptr<Config> BaseApp::config;

    BaseApp::BaseApp() {
        fileManager = std::make_unique<FileManager>();
        config = std::make_unique<Config>("config.json");
        window = std::make_shared<Window>(config->getWidth(), config->getHeight(), config->getAppName());
    }

    BaseApp::~BaseApp() = default;

    void BaseApp::run() {
        loop();
        shutdown();
    }

    void BaseApp::loop() {
        while (!window->shouldClose()) {
            glfwPollEvents();
        }
    }

    void BaseApp::shutdown() {
        window->destroy();
    }

} // namespace lve