#include "BaseApp.hpp"

#include "spdlog/spdlog.h"

#include "config/Config.hpp"
#include "render/Window.hpp"
#include "fileManager/FileManager.hpp"
#include "render/pipeline/GraphicsPipeline.hpp"
#include "render/RenderEngine.hpp"

namespace lve {

    std::unique_ptr<Config> BaseApp::config;

    BaseApp::BaseApp(CLI::App& cli) {
        FileManager::setupPaths();
        config = std::make_unique<Config>("config.json", cli);
        window = std::make_shared<Window>(config->getWidth(), config->getHeight(), config->getAppName());
        renderEngine = std::make_unique<RenderEngine>(window);
    }

    BaseApp::~BaseApp() = default;

    void BaseApp::run() {
        loop();
        shutdown();
    }

    void BaseApp::loop() {
        while (!window->shouldClose()) {
            glfwPollEvents();
            renderEngine->draw();
        }
    }

    void BaseApp::shutdown() {
        renderEngine->cleanup();
        window->destroy();
    }

    void BaseApp::loadModels() {

    }

} // namespace lve