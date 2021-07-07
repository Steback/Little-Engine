#include "FileManager.hpp"

#include "spdlog/spdlog.h"

#include "../tools/Tools.hpp"


namespace lve {

    FileManager::FileManager() {
        root = std::filesystem::current_path();

        if (root.filename().string() == "bin")
            root = root.parent_path();

        spdlog::info("Root path: {}", root.string());

        for (const auto& dir : std::filesystem::directory_iterator(root)) {
            const std::filesystem::path& path = dir.path();

            if (path.filename() == "data") data = path;
        }

        if (!exists(data)) {
            THROW_EX("Data path not exists");
        } else {
            spdlog::info("Data path: {}", data.string());
        }
    }

    FileManager::~FileManager() = default;

    std::string FileManager::getFile(FileDirectory dir, const std::string& fileName) const {
        std::filesystem::path path;

        switch (dir) {
            case ROOT:
                path = root; path.append(fileName);
                return (existsFile(path) ? path.string() : "");
            case DATA:
                path = data; path.append(fileName);
                return (existsFile(path) ? path.string() : "");
        }

        return "";
    }

    std::string FileManager::getRoot() const {
        return root.string();
    }

    std::string FileManager::getData() const {
        return data.string();
    }

    bool FileManager::existsFile(const std::filesystem::path& path) {
        if (exists(path)) {
            return true;
        } else {
            spdlog::error("File {} not exist", path.string());
            return false;
        }
    }

} // namespace lve
