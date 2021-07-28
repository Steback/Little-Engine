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


namespace lve {

    std::unique_ptr<Config> BaseApp::config;

    BaseApp::BaseApp(CLI::App& cli) {
        FileManager::setupPaths();
        Logger::setup();
        config = std::make_unique<Config>("config.json", cli);
        window = std::make_shared<Window>(config->getWidth(), config->getHeight(), config->getAppName());
        renderer = std::make_unique<Renderer>(window);

        std::vector<Mesh::Vertex> vertices{
                // left face (white)
                {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
                {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
                {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
                {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
                {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
                {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

                // right face (yellow)
                {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
                {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
                {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
                {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
                {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
                {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

                // top face (orange, remember y axis points down)
                {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
                {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
                {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
                {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
                {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
                {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

                // bottom face (red)
                {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
                {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
                {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
                {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
                {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
                {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

                // nose face (blue)
                {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
                {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
                {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
                {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
                {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
                {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

                // tail face (green)
                {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
                {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
                {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
                {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
                {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
                {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}}
        };

        for (auto& v : vertices) {
            v.position += {.0f, .0f, .0f};
        }

        assetsManager = std::make_unique<AssetsManager>(renderer->getDevice());

        scene = std::make_unique<Scene>();
        Entity* entity = scene->addEntity("Triangle");
        entity->addComponent<Transform>(vec3{.0f, .0f, .5f}, vec3(0.0f), vec3{.5f, .5f, .5f});
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