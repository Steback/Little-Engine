#include "Cli.hpp"


namespace lve {

    std::unordered_map<std::string, bool> Cli::options;

    void Cli::setup(CLI::App& app) {
        std::string immediateMode = "--immediate_mode";
        options[immediateMode] = false;
        app.add_flag(immediateMode, options[immediateMode], "Set Present Mode to VK_PRESENT_MODE_IMMEDIATE_KHR, because VK_PRESENT_MODE_FIFO_KHR can cause PC freezing in Linux, if VK_PRESENT_MODE_MAILBOX_KHR is not available");
    }

    bool Cli::checkOption(const std::string &name) {
        return options[name];
    }

} // namespace lv