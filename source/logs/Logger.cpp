#include "Logger.hpp"

#include "spdlog/spdlog.h"

#include "files/File.hpp"
#include "files/FilesManager.hpp"


namespace lve {

    std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> Logger::files;

    Logger::Logger() = default;

    Logger::~Logger() = default;

    void Logger::addFile(const std::string& name) {
        File file(FilesManager::getPath("logs") / name);
        file.remove();

        files[file.getName()] = spdlog::basic_logger_mt("LVE", file.getPath().string());

        spdlog::info("{} file created", file.getPath().string());
    }

    std::shared_ptr<spdlog::logger> Logger::getFile(const std::string& name) {
        return files[name];
    }

} // namespace lv