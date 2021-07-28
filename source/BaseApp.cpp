#include "BaseApp.hpp"

#include "config/Config.hpp"
#include "render/Window.hpp"
#include "fileManager/FileManager.hpp"
#include "render/pipeline/GraphicsPipeline.hpp"
#include "render/Renderer.hpp"
#include "logger/Logger.hpp"
#include "render/Device.hpp"
#include "scene/Scene.hpp"
#include "scene/Entity.hpp"
#include "resources/AssetsManager.hpp"
#include "render/RenderSystem.hpp"
#include "scene/components/Transform.hpp"
#include "scene/components/MeshInterface.hpp"
#include "math/Utils.hpp"


namespace lve {

    std::unique_ptr<Config> BaseApp::config;

    BaseApp::BaseApp(CLI::App& cli) {
        FileManager::setupPaths();
        Logger::setup();
        config = std::make_unique<Config>("config.json", cli);
        window = std::make_shared<Window>(config->getWidth(), config->getHeight(), config->getAppName());
        renderer = std::make_unique<Renderer>(window);

        std::vector<Mesh::Vertex> vertices{
                { {0.0f, -0.5f}, {1.0f, 0.0f, 0.0f} },
                { {0.5f, 0.5f}, {0.0f, 1.0f, 0.0f} },
                { {-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f} }
        };

        assetsManager = std::make_unique<AssetsManager>(renderer->getDevice());

        scene = std::make_unique<Scene>();
        Entity* entity = scene->addEntity("Triangle");
        entity->addComponent<Transform>(vec2{0.0f, 0.0f}, lve::pi(), vec2{1.0f, 1.0f});
        entity->addComponent<MeshInterface>(assetsManager->addMesh(0, vertices));
    }

    BaseApp::~BaseApp() = default;

    void BaseApp::run() {
        renderer->setupDrawResources();
        loop();
        shutdown();
    }

    void BaseApp::loop() {
        RenderSystem renderSystem(renderer->getDevice(), renderer->getRenderPass());

        while (!window->shouldClose()) {
            glfwPollEvents();

            if (auto commandBuffer = renderer->beginFrame()) {
                renderer->beginSwapChainRenderPass(commandBuffer);
                renderSystem.renderEntities(commandBuffer, scene->getRegistry());
                renderer->endSwapChainRenderPass(commandBuffer);
                renderer->endFrame();
            }
        }

        renderer->getDevice()->getLogicalDevice().waitIdle();
    }

    void BaseApp::shutdown() {
        assetsManager->cleanup();
        renderer->cleanup();
        window->destroy();
    }

} // namespace lve