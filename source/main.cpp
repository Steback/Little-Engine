#include "App.hpp"
#include "logs/Logs.hpp"


int main(int argc, char** argv) {
    lve::App app;

    try {
        app.run();
    } catch (const std::runtime_error& e) {
        LVE_LOG_ERROR(e.what());
    }

    return 0;
}
