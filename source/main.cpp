#include "spdlog/spdlog.h"
#include "CLI/App.hpp"
#include "CLI/Formatter.hpp"
#include "CLI/Config.hpp"

#include "BaseApp.hpp"
#include "config/Config.hpp"


int main(int argc, char** argv) {
    CLI::App cli("Little Vulkan Engine");
    lve::Config::setupCliOptions(cli);
    CLI11_PARSE(cli, argc, argv);

    lve::BaseApp app(cli);

    try {
        app.run();
    } catch (const std::runtime_error& e) {
        spdlog::error(e.what());
    }

    return 0;
}
