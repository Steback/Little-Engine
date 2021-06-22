#include "Base.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

#include "Window.hpp"
#include "Instance.hpp"
#include "Device.hpp"
#include "Constants.hpp"

using json = nlohmann::json;


Base::Base()  {
    json config;
    std::ifstream file("data/config.json");
    file >> config;
    file.close();

    auto appName = config["title"].get<std::string>();
    window = std::make_shared<Window>(appName.c_str(), config["width"].get<int>(),
                                      config["height"].get<int>());

    std::vector<const char*> reqLayers;
#ifdef ENGINE_DEBUG
    reqLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

    vk::ApplicationInfo appInfo(
            appName.c_str(),
            VK_MAKE_VERSION(0, 0, 1),
            "Custom Engine",
            VK_VERSION_1_2);

    instance = std::make_shared<Instance>(appInfo, reqLayers);
    device = std::make_shared<Device>(window, instance);

    setup();
}

Base::~Base() {
    destroy();
    instance->destroy();
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
