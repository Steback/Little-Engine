#include "App.hpp"

#include "files/FilesManager.hpp"
#include "logs/Logs.hpp"
#include "files/File.hpp"
#include "window/Window.hpp"


namespace lve {

    App::App() {
        fileManager = std::make_unique<FilesManager>();
        fileManager->addPath("data");
        fileManager->addPath("logs");
        fileManager->addPath("bin/shaders");

        Logger::addFile(File(fileManager->getPath("logs") / "error.log"));

        config = Config(fileManager->getFile("config.json"));

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