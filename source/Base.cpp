#include "Base.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

#include "Window.hpp"

using json = nlohmann::json;


Base::Base()  {
    json config;
    std::ifstream file("data/config.json");
    file >> config;
    file.close();

    window = std::make_shared<Window>(config["title"].get<std::string>().c_str(), config["width"].get<int>(),
                                      config["height"].get<int>());
    setup();
}

Base::~Base() {
    destroy();
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
