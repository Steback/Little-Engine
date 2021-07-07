#include "BaseApp.hpp"

#include "spdlog/spdlog.h"
#include "CLI/App.hpp"

#include "Config.hpp"
#include "Window.hpp"
#include "FileManager.hpp"


namespace lve {

    std::unique_ptr<FileManager> BaseApp::fileManager;
    std::unique_ptr<Config> BaseApp::config;

    BaseApp::BaseApp(CLI::App& cli) {
        bool validation = cli.get_option("-v")->as<bool>();

        if (validation)
            spdlog::info("Enable Validations Layers");

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

    void BaseApp::setupCliOptions(CLI::App &cli) {
        cli.add_flag("-v, --validation", "Enable vulkan Validations Layers");
    }

} // namespace lve