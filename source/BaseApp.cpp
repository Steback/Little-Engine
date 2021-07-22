#include "BaseApp.hpp"

#include "config/Config.hpp"
#include "render/Window.hpp"
#include "fileManager/FileManager.hpp"
#include "render/pipeline/GraphicsPipeline.hpp"
#include "render/RenderEngine.hpp"
#include "logger/Logger.hpp"
#include "mesh/Mesh.hpp"
#include "render/Device.hpp"


namespace lve {

    std::unique_ptr<Config> BaseApp::config;

    BaseApp::BaseApp(CLI::App& cli) {
        FileManager::setupPaths();
        Logger::setup();
        config = std::make_unique<Config>("config.json", cli);
        window = std::make_shared<Window>(config->getWidth(), config->getHeight(), config->getAppName());
        renderEngine = std::make_unique<RenderEngine>(window);

        std::vector<Mesh::Vertex> vertices{
                { {0.0f, -0.5f}, {1.0f, 0.0f, 0.0f} },
                { {0.5f, 0.5f}, {0.0f, 1.0f, 0.0f} },
                { {-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f} }
        };

        model = std::make_unique<Mesh>(renderEngine->getDevice(), vertices);
    }

    BaseApp::~BaseApp() = default;

    void BaseApp::run() {
        renderEngine->setupDrawResources();
        loop();
        shutdown();
    }

    void BaseApp::loop() {
        while (!window->shouldClose()) {
            glfwPollEvents();

            renderEngine->beginDraw({0.1f, 0.1f, 0.1f, 1.0f});

            vk::CommandBuffer commandBuffer = renderEngine->getCommandBuffer();
            model->bind(commandBuffer);
            model->draw(commandBuffer);

            renderEngine->endDraw();
        }
    }

    void BaseApp::shutdown() {
        renderEngine->getDevice()->getLogicalDevice().waitIdle();

        model->destroy();

        renderEngine->cleanup();
        window->destroy();
    }

    void BaseApp::loadModels() {

    }

} // namespace lve