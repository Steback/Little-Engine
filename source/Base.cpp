#include "Base.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

#include "Window.hpp"
#include "Device.hpp"
#include "pipeline/GraphicsPipeline.hpp"

using json = nlohmann::json;


Base::Base()  {
    json config;
    {
        std::ifstream file("data/config.json");
        file >> config;
        file.close();
    }

    window = std::make_shared<Window>(config["title"].get<std::string>().c_str(), config["width"].get<int>(),
                                      config["height"].get<int>());
    device = std::make_shared<Device>(window);
    pipeline = std::make_shared<GraphicsPipeline>(device, "model.vert.spv", "model.frag.spv",
                                                  GraphicsPipeline::defaultConfigInfo(window->getWidth(), window->getHeight()));

    setup();
}

Base::~Base() {
    destroy();
    pipeline->cleanup();
    device->destroy();
    window->destroy();
}

void Base::setup() {

}

void Base::update() {

}

void Base::destroy() {

}

void Base::loop() {
    while (window->isOpen()) {
        glfwPollEvents();
        update();
    }
}
