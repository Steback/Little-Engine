#include "Logger.hpp"

#include "../fileManager/FileManager.hpp"


namespace lve {

    std::shared_ptr<spdlog::logger> Logger::file;

    Logger::Logger() = default;

    void Logger::setup() {
        for (auto& logFile : std::filesystem::directory_iterator(FileManager::logsPath()))
            std::filesystem::remove(logFile);

        file = spdlog::basic_logger_mt("LVE", FileManager::logsPath() + "errors.log");
    }

    void Logger::error(const std::string &message) {
#if !defined(NDEBUG)
        spdlog::error(message);
#endif
        file->error(message);
    }

}