#include "Pipeline.hpp"

#include "spdlog/spdlog.h"

#include "../../fileManager/FileManager.hpp"


namespace lve {

    Pipeline::Pipeline(const std::string& shadersName) {
        auto vertCode = FileManager::readFile(FileManager::shadersPath() + shadersName + ".vert.spv");
        auto fragCode = FileManager::readFile(FileManager::shadersPath() + shadersName + ".frag.spv");

        spdlog::info("Vertex Shader Code Size: {}", vertCode.size());
        spdlog::info("Fragment Shader Code Size: {}", fragCode.size());
    }

} // namespace lve