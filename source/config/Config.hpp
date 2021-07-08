#ifndef LITTLEVULKANENGINE_CONFIG_HPP
#define LITTLEVULKANENGINE_CONFIG_HPP


#include <string>


namespace CLI {
    class App;
}


namespace lve {

    class Config {
    public:
        Config();

        explicit Config(const std::string& fileName, CLI::App& cli);

        void load();

        void save();

        [[nodiscard]] int getWidth() const;

        [[nodiscard]] int getHeight() const;

        [[nodiscard]] std::string getAppName() const;

        [[nodiscard]] bool reqDeviceInfo() const;

        static void setupCliOptions(CLI::App &cli);

    private:
        int width{};
        int height{};
        std::string appName{};
        std::string file{};
        bool deviceInfo{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_CONFIG_HPP
