#include "Base.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

#include "Window.hpp"
#include "Device.hpp"
#include "pipeline/Pipeline.hpp"

using json = nlohmann::json;


Base::Base()  {
    json config;
    {
        std::ifstream file("data/config.json");
        file >> config;
        file.close();
    }

    mainWindow = std::make_shared<Window>(config["title"].get<std::string>().c_str(), config["width"].get<int>(),
                                          config["height"].get<int>());
    device = std::make_shared<Device>(mainWindow);
    pipeline = std::make_shared<Pipeline>("model.vert.spv", "model.frag.spv");

    setup();
}

Base::~Base() {
    destroy();
    device->destroy();
    mainWindow->destroy();
}

void Base::setup() {

}

void Base::update() {

}

void Base::destroy() {

}

void Base::loop() {
    while (mainWindow->isOpen()) {
        glfwPollEvents();
        update();
    }
}
