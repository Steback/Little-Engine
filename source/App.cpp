#include "App.hpp"

#include "files/FilesManager.hpp"
#include "logs/Logs.hpp"
#include "window/Window.hpp"


namespace lve {

    App::App() {
        FilesManager::setRootPath();
        FilesManager::addPath("data");
        FilesManager::addPath("logs");
        FilesManager::addPath("bin/shaders");

        Logger::addFile("error.log");

        config = Config("config.json");

        window = std::make_shared<Window>(config.getAppName(), config.getWidth(), config.getHeight());
    }

    App::~App() {

    }

    void App::start() {

    }

    void App::loop() {
        while (window->isOpen()) {
            glfwPollEvents();
        }
    }

    void App::run() {
        start();
        loop();
    }

} // namespace lv