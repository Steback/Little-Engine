#include "App.hpp"
#include "logs/Logs.hpp"

#include "config/Cli.hpp"


int main(int argc, char** argv) {
    CLI::App app{"App description"};
    lve::Cli::setup(app);
    CLI11_PARSE(app, argc, argv);

    lve::App engine;

    try {
        engine.run();
    } catch (const std::runtime_error& e) {
        LVE_LOG_ERROR(e.what());
    }

    return 0;
}
