#include "spdlog/spdlog.h"

#include "App.hpp"


int main(int argc, char** argv) {
    lve::App app;

    try {
        app.run();
    } catch (const std::runtime_error& e) {
        spdlog::error(e.what());
    }

    return 0;
}
