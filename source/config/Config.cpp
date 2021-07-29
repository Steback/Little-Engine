#include "Config.hpp"

#include "utils/Json.hpp"
#include "files/FilesManager.hpp"


namespace lve {

    Config::Config() = default;

    Config::Config(const std::string& name)  {
        file = FilesManager::getFile(name);
        load();
    }

    Config::~Config() = default;

    void Config::load() {
        json data;
        file.read(data);

        appName = data["appName"].get<std::string>();
        width = data["width"].get<int>();
        height = data["height"].get<int>();
    }

    void Config::save() {
        json data = {
                {"appName", appName},
                {"width", width},
                {"height", height}
        };

        file.write(data);
    }

    const std::string &Config::getAppName() const {
        return appName;
    }

    void Config::setAppName(const std::string &appName_) {
        Config::appName = appName_;
    }

    int Config::getWidth() const {
        return width;
    }

    void Config::setWidth(int width_) {
        Config::width = width_;
    }

    int Config::getHeight() const {
        return height;
    }

    void Config::setHeight(int height_) {
        Config::height = height_;
    }

} // namespace lv