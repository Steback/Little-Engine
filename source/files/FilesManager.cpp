#include "FilesManager.hpp"

#include "spdlog/spdlog.h"

#include "File.hpp"


namespace lve {

    FilesManager::FilesManager() {
        std::filesystem::path root(absolute(std::filesystem::current_path()));

        if (root.filename() == "Release") {
            root = root.parent_path().parent_path();
        } else if (root.filename() == "bin") {
            root = root.parent_path();
        }

        paths["root"] = root;

        spdlog::info("root path: {}", paths["root"].string());
    }

    FilesManager::~FilesManager() = default;

    void FilesManager::addPath(const std::string &name) {
        std::filesystem::path path = paths["root"] / name;

        if (!exists(path)) {
            spdlog::error("{} not exists", path.string());
        } else {
            paths[name] = path;
            spdlog::info("{} path: {}", name, paths[name].string());
        }
    }

    File FilesManager::getFile(const std::string &fileName) {
        for (auto& [name, path] : paths) {
            std::filesystem::path filePath = path / fileName;

            if (exists(filePath)) return File(filePath);
        }

        spdlog::error("{} not exists", fileName);

        return File();
    }

    std::filesystem::path FilesManager::getPath(const std::string& name) {
        return paths[name];
    }

} // namespace lv