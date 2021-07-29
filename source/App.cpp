#include "App.hpp"

#include "files/FilesManager.hpp"
#include "logs/Logs.hpp"
#include "files/File.hpp"


namespace lve {

    App::App() {
        fileManager = std::make_unique<FilesManager>();
        fileManager->addPath("data");
        fileManager->addPath("logs");
        fileManager->addPath("bin/shaders");

        Logger::addFile(File(fileManager->getPath("logs") / "error.log"));

        config = Config(fileManager->getFile("config.json"));
        spdlog::info("App Name: {}", config.getAppName());
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