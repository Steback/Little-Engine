#include "App.hpp"

#include "files/FilesManager.hpp"


namespace lve {

    App::App() {
        fileManager = std::make_unique<FilesManager>();
        fileManager->addPath("data");
        fileManager->addPath("logs");
        fileManager->addPath("bin/shaders");
    }

    App::~App() {

    }

    void App::start() {

    }

    void App::loop() {

    }

    void App::run() {
        start();
        loop();
    }

} // namespace lv