#ifndef LITTLEVULKANENGINE_CONFIG_HPP
#define LITTLEVULKANENGINE_CONFIG_HPP


#include <string>


namespace lve {

    class Config {
    public:
        Config();

        explicit Config(const std::string& fileName);

        void load();

        void save();

        [[nodiscard]] int getWidth() const;

        [[nodiscard]] int getHeight() const;

        [[nodiscard]] std::string getAppName() const;

    private:
        int width{};
        int height{};
        std::string appName{};
        std::string file{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_CONFIG_HPP
