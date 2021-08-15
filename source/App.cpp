#include "App.hpp"

#include <chrono>

#include "files/FilesManager.hpp"
#include "logs/Logs.hpp"
#include "window/Window.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/RenderSystem.hpp"
#include "assets/AssetsManager.hpp"
#include "scene/Scene.hpp"
#include "entity/Entity.hpp"
#include "entity/components/Transform.hpp"
#include "math/Common.hpp"


namespace lve {

    App::App(const std::string& appName) {
        FilesManager::setRootPath();
        FilesManager::addPath("data");
        FilesManager::addPath("logs", true);
        FilesManager::addPath("bin/shaders");
        FilesManager::addPath("assets");

        Logger::addFile("error.log");

        config = Config("config.json");
        config.setAppName(appName);
        window = std::make_shared<Window>(config.getAppName(), config.getWidth(), config.getHeight(), input);
        renderer = std::make_unique<Renderer>(window, config);
        assetsManager = std::make_unique<AssetsManager>(renderer->getDevice());
        scene = std::make_unique<Scene>(*assetsManager);
    }

    App::~App() = default;

    void App::loop() {
        auto currentTime = std::chrono::high_resolution_clock::now();

        while (window->isOpen()) {
            glfwPollEvents();
            update(currentTime);
            render();
        }
    }

    void App::update(std::chrono::time_point<std::chrono::system_clock>& currentTime) {
        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        float aspect = renderer->getAspectRatio();
        camera.setPerspectiveProjection(radians(50.f), aspect, 0.1f, 10.f);

        onUpdate(frameTime);
    }

    void App::render() {
        if (auto commandBuffer = renderer->beginFrame()) {
            renderer->beginSwapChainRenderPass(commandBuffer);

            // TODO: So awful, find a better way to add dynamic draw
            renderSystem->renderEntities(commandBuffer, camera, scene->getRegistry(), [&](VkPipelineLayout layout, VkCommandBuffer commandBuffer, id_t entityID){
                onDrawEntity(layout, commandBuffer, entityID);
            });

            renderer->endSwapChainRenderPass(commandBuffer);
            renderer->endFrame();
        }
    }

    void App::shutdown() {
        renderer->waitDeviceIde();

        renderSystem->destroy();
        assetsManager->cleanup();
        renderer->cleanup();
        window->destroy();
    }

    void App::run() {
        start();
        loop();
        shutdown();
    }



} // namespace lv