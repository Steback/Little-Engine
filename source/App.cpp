#include "App.hpp"

#include "files/FilesManager.hpp"
#include "logs/Logs.hpp"
#include "window/Window.hpp"
#include "graphics/Renderer.hpp"


namespace lve {

    App::App() {
        FilesManager::setRootPath();
        FilesManager::addPath("data");
        FilesManager::addPath("logs");
        FilesManager::addPath("bin/shaders");

        Logger::addFile("error.log");

        config = Config("config.json");
        window = std::make_shared<Window>(config.getAppName(), config.getWidth(), config.getHeight());
        renderer = std::make_unique<Renderer>(window, config);
    }

    App::~App() = default;

    void App::start() {
        renderer->setupDrawResources();
    }

    void App::loop() {
        while (window->isOpen()) {
            glfwPollEvents();
        }
    }

    void App::shutdown() {
        renderer->cleanup();
        window->destroy();
    }

    void App::run() {
        start();
        loop();
        shutdown();
    }

} // namespace lv