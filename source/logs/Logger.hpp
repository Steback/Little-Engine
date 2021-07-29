#ifndef LITTLEVULKANENGINE_LOGGER_HPP
#define LITTLEVULKANENGINE_LOGGER_HPP


#include <string>
#include <unordered_map>

#include "spdlog/sinks/basic_file_sink.h"


namespace lve {

    class File;

    class Logger {
    public:
        ~Logger();

        static void addFile(File file);

        static std::shared_ptr<spdlog::logger> getFile(const std::string& name);

    private:
        Logger();

        static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> files;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_LOGGER_HPP
