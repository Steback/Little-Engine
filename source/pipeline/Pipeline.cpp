#include "Pipeline.hpp"

#include <fstream>
#include <filesystem>

#include "spdlog/spdlog.h"

#include "../Constants.hpp"


Pipeline::Pipeline(const std::string &vert, const std::string &frag) {
    std::vector<char> vertCode = readFile(vert);
    std::vector<char> fragCode = readFile(frag);

    spdlog::info("Vert Shader size: {}", vertCode.size());
    spdlog::info("Frag Shader size: {}", fragCode.size());
}

Pipeline::~Pipeline() = default;

std::vector<char> Pipeline::readFile(const std::string &name) {
    std::filesystem::path path(SHADERS_DIR + name);

    if (!exists(path)) {
        spdlog::error("{} doesn't exist", path.string());
        return std::vector<char>();
    } else {
        std::ifstream file(path, std::ios::ate | std::ios::binary);

        if (!file.is_open())
            spdlog::error("Failed to open file: {}", path.string());

        size_t size = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(size);

        file.seekg(0);
        file.read(buffer.data(), size);

        file.close();

        return buffer;
    }
}
