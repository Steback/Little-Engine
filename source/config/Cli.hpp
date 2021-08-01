#ifndef LITTLEVULKANENGINE_CLI_HPP
#define LITTLEVULKANENGINE_CLI_HPP


#include <string>
#include <unordered_map>

#include "CLI/App.hpp"
#include "CLI/Formatter.hpp"
#include "CLI/Config.hpp"


namespace lve {

    class Cli {
    public:
        static void setup(CLI::App& app);

        static bool checkOption(const std::string& name);

    private:
        static std::unordered_map<std::string, bool> options;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_CLI_HPP
