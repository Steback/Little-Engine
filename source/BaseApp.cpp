#include "BaseApp.hpp"

#include <fstream>
#include <string>

#include "nlohmann/json.hpp"

#include "Tools.hpp"
#include "Window.hpp"


using json = nlohmann::json;


namespace lve {


    BaseApp::BaseApp() {
        json config;
        std::ifstream file("data/config.json");

        if (file.is_open()) {
            file >> config;
        } else {
            THROW_EX("Failed to open file data/config.json");
        }

        file.close();

        window = std::make_shared<Window>(config["width"].get<int>(), config["height"].get<int>(),
                config["title"].get<std::string>().c_str());
    }

    BaseApp::~BaseApp() {
        window->destroy();
    }

    void BaseApp::run() {
        while (!window->shouldClose()) {
            glfwPollEvents();
        }
    }

} // namespace lve