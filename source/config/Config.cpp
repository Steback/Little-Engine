#include "Config.hpp"

#include <filesystem>
#include <fstream>

#include "CLI/App.hpp"

#include "../tools/Tools.hpp"
#include "../tools/json.hpp"
#include "../fileManager/FileManager.hpp"


namespace lve {

    Config::Config() = default;

    Config::Config(const std::string &fileName, CLI::App& cli) {
        file = FileManager::getFile(FileManager::dataPath() + fileName);

        if (!file.empty()) {
            validationLayers = cli.get_option("-v")->as<bool>();
            deviceInfo = cli.get_option("-d")->as<bool>();
            load();
        } else {
            THROW_EX("Failed to load config file");
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

    bool Config::reqValidationLayers() const {
        return validationLayers;
    }

    bool Config::reqDeviceInfo() const {
        return deviceInfo;
    }

    void Config::setupCliOptions(CLI::App &cli) {
        // TODO: Find for a better approach to add cli options dynamically
        cli.add_flag("-v, --validation", "Enable vulkan Validations Layers");
        cli.add_flag("-d, --deviceInfo", "Enable print physical device info");
    }

} // namespace lve