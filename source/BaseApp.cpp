#include "BaseApp.hpp"

#include "spdlog/spdlog.h"

#include "config/Config.hpp"
#include "render/Window.hpp"
#include "render/Device.hpp"
#include "fileManager/FileManager.hpp"
#include "render/pipeline/GraphicsPipeline.hpp"


namespace lve {

    std::unique_ptr<Config> BaseApp::config;

    BaseApp::BaseApp(CLI::App& cli) {
        FileManager::setupPaths();
        config = std::make_unique<Config>("config.json", cli);
        window = std::make_shared<Window>(config->getWidth(), config->getHeight(), config->getAppName());
        device = std::make_shared<Device>(window);
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
        device->destroy();
        window->destroy();
    }

} // namespace lve