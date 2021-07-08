#include "BaseApp.hpp"

#include "spdlog/spdlog.h"
#include "CLI/App.hpp"

#include "config/Config.hpp"
#include "render/Window.hpp"
#include "fileManager/FileManager.hpp"
#include "render/pipeline/GraphicsPipeline.hpp"


namespace lve {

    std::unique_ptr<Config> BaseApp::config;

    BaseApp::BaseApp(CLI::App& cli) {
        bool validation = cli.get_option("-v")->as<bool>();

        if (validation)
            spdlog::info("Enable Validations Layers");

        FileManager::setupPaths();
        config = std::make_unique<Config>("config.json");
        window = std::make_shared<Window>(config->getWidth(), config->getHeight(), config->getAppName());
        graphicsPipeline = std::make_unique<GraphicsPipeline>("model");
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
        // TODO: Find for a better approach to add cli options dynamically
        cli.add_flag("-v, --validation", "Enable vulkan Validations Layers");
    }

} // namespace lve