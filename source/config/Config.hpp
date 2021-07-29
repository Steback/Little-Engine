#ifndef LITTLEVULKANENGINE_CONFIG_HPP
#define LITTLEVULKANENGINE_CONFIG_HPP


#include <string>

#include "files/File.hpp"


namespace lve {

    class Config {
    public:
        Config();

        explicit Config(const File& file);

        ~Config();

        void load();

        void save();

        [[nodiscard]] const std::string &getAppName() const;

        void setAppName(const std::string &appName_);

        [[nodiscard]] int getWidth() const;

        void setWidth(int width_);

        [[nodiscard]] int getHeight() const;

        void setHeight(int height_);

    private:
        File file;
        std::string appName;
        int width{};
        int height{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_CONFIG_HPP
