#ifndef LITTLEVULKANENGINE_SOURCE_LOGGER_LOGGER_HPP
#define LITTLEVULKANENGINE_SOURCE_LOGGER_LOGGER_HPP


#include <string>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"


namespace lve {

    class Logger {
    public:
        static void setup();

        static void error(const std::string& message);

    private:
        Logger();

    private:
        static std::shared_ptr<spdlog::logger> file;
    };

}


#endif //LITTLEVULKANENGINE_SOURCE_LOGGER_LOGGER_HPP
