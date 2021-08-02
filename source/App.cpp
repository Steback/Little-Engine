#include "App.hpp"

#include "files/FilesManager.hpp"
#include "logs/Logs.hpp"
#include "window/Window.hpp"
#include "graphics/Renderer.hpp"
#include "Mesh/Mesh.hpp"
#include "graphics/RenderSystem.hpp"
#include "scene/Scene.hpp"
#include "assets/AssetsManager.hpp"
#include "entity/Entity.hpp"
#include "entity/components/Transform.hpp"
#include "entity/components/MeshInterface.hpp"
#include "graphics/Camera.hpp"
#include "math/Common.hpp"


namespace lve {

    App::App() {
        FilesManager::setRootPath();
        FilesManager::addPath("data");
        FilesManager::addPath("logs", true);
        FilesManager::addPath("bin/shaders");

        Logger::addFile("error.log");

        config = Config("config.json");
        window = std::make_shared<Window>(config.getAppName(), config.getWidth(), config.getHeight());
        renderer = std::make_unique<Renderer>(window, config);
        assetsManager = std::make_unique<AssetsManager>(renderer->getDevice());
    }

    App::~App() = default;

    void App::start() {
        scene = std::make_unique<Scene>();

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
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        };

        Entity* entity = scene->addEntity("cube");
        entity->addComponent<Transform>(vec3(0.0f, 0.0f, 2.5f), vec3(), vec3(0.5f, 0.5f, 0.5f));
        entity->addComponent<MeshInterface>(assetsManager->addMesh("cube", vertices));

        renderer->setupDrawResources();
    }

    void App::loop() {
        RenderSystem renderSystem(renderer->getDevice(), renderer->getRenderPass());
        Camera camera;
//         camera.setViewDirection(vec3(0.f), vec3(0.5f, 0.f, 1.f));
        camera.setViewTarget({-1.f, -2.f, -2.f}, {0.f, 0.f, 2.5f});

        while (window->isOpen()) {
            glfwPollEvents();

            float aspect = renderer->getAspectRatio();
//             camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
            camera.setPerspectiveProjection(radians(50.f), aspect, 0.1f, 10.f);

            if (auto commandBuffer = renderer->beginFrame()) {
                renderer->beginSwapChainRenderPass(commandBuffer);

                renderSystem.renderEntities(commandBuffer, scene->getRegistry(), camera);

                renderer->endSwapChainRenderPass(commandBuffer);
                renderer->endFrame();
            }
        }

        renderer->waitDeviceIde();
        renderSystem.destroy();
    }

    void App::shutdown() {
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