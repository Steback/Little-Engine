#include "FileManager.hpp"

#include <fstream>

#include "spdlog/spdlog.h"

#include "../tools/Tools.hpp"


namespace lve {

    std::filesystem::path FileManager::root;
    std::filesystem::path FileManager::data;
    std::filesystem::path FileManager::shaders;
    std::filesystem::path FileManager::logs;

    FileManager::FileManager() = default;


    void FileManager::setupPaths() {
        root = std::filesystem::current_path();

#ifdef _WIN64
        if (root.filename().string() == "Release") {
            root = root.parent_path().parent_path();
        }
#endif

        if (root.filename().string() == "bin") {
            root = root.parent_path();
        }

        spdlog::info("Root path: {}", root.string());

        logs = root + "logs";

        if (!exists(logs))
            std::filesystem::create_directories(logs);

        spdlog::info("Logs path: {}", logs.string());

        data = root + "data";
        spdlog::info("Data path: {}", data.string());

        shaders = root + "bin/shaders";
        spdlog::info("Shaders path: {}", shaders.string());
    }

    std::filesystem::path FileManager::rootPath() {
        return root;
    }

    std::filesystem::path FileManager::dataPath() {
        return data;
    }

    std::filesystem::path FileManager::shadersPath() {
        return shaders;
    }

    std::filesystem::path FileManager::logsPath() {
        return logs;
    }

    std::string FileManager::getFile(const std::string& fileName) {
        std::filesystem::path path(fileName);
        return (fileExists(path) ? path.string() : "");
    }

    bool FileManager::fileExists(const std::filesystem::path& path) {
        return exists(path);
    }

    std::vector<char> FileManager::readFile(const std::string &name) {
        if (fileExists(name)) {
            std::ifstream file{name, std::ios::ate | std::ios::binary};
            size_t size = static_cast<size_t>(file.tellg());
            std::vector<char> buffer(size);

            file.seekg(0);
            file.read(buffer.data(), static_cast<std::streamsize>(size));
            file.close();

            return buffer;
        } else {
            return std::vector<char>();
        }
    }

} // namespace lve
