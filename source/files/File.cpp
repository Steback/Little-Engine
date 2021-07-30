#include "File.hpp"

#include <fstream>

#include "utils/Macros.hpp"


namespace lve {

    File::File() = default;

    File::File(std::filesystem::path path) : path(std::move(path)) {

    }

    File::~File() = default;

    const std::filesystem::path &File::getPath() const {
        return path;
    }

    void File::setPath(const std::filesystem::path &newPath) {
        File::path = newPath;
    }

    std::vector<char> File::read() {
        std::ifstream file{path, std::ios::ate | std::ios::binary};

        if (!file.is_open()) LVE_THROW_EX("failed to open file: " + path.string());

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), static_cast<std::streamsize>(fileSize));

        file.close();

        return buffer;
    }

    void File::remove() {
        if (exists(path)) std::filesystem::remove(path);
    }

    std::string File::getName() const {
        return path.filename();
    }

    void File::read(json& data) {
        std::ifstream file(path);
        file >> data;
        file.close();
    }

    void File::write(json& data) {
        std::ofstream file(path);
        file << std::setw(4) << data;
        file.close();
    }

} // namespace lv