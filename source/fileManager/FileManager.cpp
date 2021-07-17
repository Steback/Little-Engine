#include "FileManager.hpp"

#include <fstream>

#include "spdlog/spdlog.h"

#include "../tools/Tools.hpp"


namespace lve {

    std::filesystem::path FileManager::root;
    std::filesystem::path FileManager::data;
    std::filesystem::path FileManager::shaders;

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

        data = root + "data";
        pathExists(data, "Data");

        shaders = root + "bin/shaders";
        pathExists(shaders, "Shaders");
    }

    FileManager::~FileManager() = default;

    std::filesystem::path FileManager::rootPath() {
        return root;
    }

    std::filesystem::path FileManager::dataPath() {
        return data;
    }

    std::filesystem::path FileManager::shadersPath() {
        return shaders;
    }

    std::string FileManager::getFile(const std::string& fileName) {
        std::filesystem::path path(fileName);
        return (existsFile(path) ? path.string() : "");
    }

    bool FileManager::existsFile(const std::filesystem::path& path) {
        bool exist = exists(path);

        if (!exist)
            spdlog::error("File {} not exist", path.string());

        return exist;
    }

    void FileManager::pathExists(const std::filesystem::path& path, const std::string& name) {
        if (!exists(path)) {
            THROW_EX(fmt::format("{} path not exists", name));
        } else {
            spdlog::info("{} path: {}", name, path.string());
        }
    }

    std::vector<char> FileManager::readFile(const std::string &name) {
        if (existsFile(name)) {
            std::ifstream file{name, std::ios::ate | std::ios::binary};
            size_t fileSize = (size_t)file.tellg();
            std::vector<char> buffer(fileSize);

            file.seekg(0);
            file.read(buffer.data(), fileSize);
            file.close();

            return buffer;
        } else {
            return std::vector<char>();
        }
    }

} // namespace lve
