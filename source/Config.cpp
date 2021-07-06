#include "Config.hpp"

#include <filesystem>
#include <fstream>

#include "Tools.hpp"
#include "BaseApp.hpp"
#include "FileManager.hpp"
#include "json.hpp"


namespace lve {

    Config::Config() = default;

    Config::Config(const std::string &fileName) {
        file = BaseApp::fileManager->getFile(FileDirectory::DATA, fileName);

        if (!file.empty()) {
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

} // namespace lve