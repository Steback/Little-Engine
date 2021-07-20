#include "Config.hpp"

#include <filesystem>
#include <fstream>

#include "CLI/App.hpp"

#include "../extras/json.hpp"
#include "../fileManager/FileManager.hpp"
#include "../logger/Logger.hpp"
#include "../tools/Tools.hpp"


namespace lve {

    Config::Config() = default;

    Config::Config(const std::string &fileName, CLI::App& cli) {
        file = FileManager::getFile(FileManager::dataPath() + fileName);

        if (!file.empty()) {
            deviceInfo = cli.get_option("--deviceInfo")->as<bool>();
            load();
        } else {
            EXIT_ERROR("Failed to load config file");
        }
    }

    void Config::load() {
        json j;
        std::ifstream in(file);
        in >> j;
        in.close();

        width = j["width"].get<int>();
        height = j["height"].get<int>();
        appName = j["appName"].get<std::string>();
    }

    void Config::save() {
        json j = {
                {"width", width},
                {"height", height},
                {"appName", appName}
        };

        std::ofstream out(file);
        out << std::setw(4) << j << std::endl;
        out.close();
    }

    int Config::getWidth() const {
        return width;
    }

    int Config::getHeight() const {
        return height;
    }

    std::string Config::getAppName() const {
        return appName;
    }

    bool Config::reqDeviceInfo() const {
        return deviceInfo;
    }

    void Config::setupCliOptions(CLI::App &cli) {
        // TODO: Find for a better approach to add cli options dynamically
        cli.add_flag("--deviceInfo", "Enable print physical device info");
    }

} // namespace lve