#include "BaseApp.hpp"

#include "config/Config.hpp"
#include "render/Window.hpp"
#include "fileManager/FileManager.hpp"
#include "render/pipeline/GraphicsPipeline.hpp"
#include "render/RenderEngine.hpp"
#include "logger/Logger.hpp"
#include "mesh/Mesh.hpp"
#include "render/Device.hpp"
#include "render/pipeline/GraphicsPipeline.hpp"
#include "scene/Scene.hpp"
#include "scene/Entity.hpp"
#include "scene/components/Transform.hpp"


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

        scene = std::make_unique<Scene>();
        Entity* entity = scene->addEntity("Triangle");
        entity->addComponent<component::Transform>(vec2{0.0f, 0.0f}, 3.14159f, vec2{1.0f, 1.0f});

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

            for (auto& [id, entity] : scene->getEntities()) {
                auto& transform = entity->getComponent<component::Transform>();

                SimplePushConstantData push{};
                push.offset = transform.translation;
                push.color = {0.2f, 0.0f, 0.2f};
                push.transform = transform.getWorldMatrix();

                commandBuffer.pushConstants(
                        renderEngine->getLayout(),
                        vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment,
                        0,
                        sizeof(SimplePushConstantData),
                        &push
                );

                model->draw(commandBuffer);
            }

            renderEngine->endDraw();
        }
    }

    void BaseApp::shutdown() {
        renderEngine->getDevice()->getLogicalDevice().waitIdle();

        model->destroy();

        renderEngine->cleanup();
        window->destroy();
    }

} // namespace lve