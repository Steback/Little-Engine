#ifndef LITTLEVULKANENGINE_FILEMANAGER_HPP
#define LITTLEVULKANENGINE_FILEMANAGER_HPP


#include <filesystem>
#include <vector>


namespace lve {

    class FileManager {
    public:
        FileManager();

        ~FileManager();

        [[nodiscard]] std::filesystem::path rootPath() const;

        [[nodiscard]] std::filesystem::path dataPath() const;

        [[nodiscard]] std::filesystem::path shadersPath() const;

        [[nodiscard]] static std::string getFile(const std::string& fileName) ;

        static bool existsFile(const std::filesystem::path& path);

        static void pathExists(const std::filesystem::path& path, const std::string& name);

        static std::vector<char> readFile(const std::string& name);

    private:
        std::filesystem::path root{};
        std::filesystem::path data{};
        std::filesystem::path shaders{};
    };

} // namespace lve

inline std::string operator+(const std::filesystem::path& path, const std::string& name) {
    return (std::filesystem::path(path) /= name).string();
}


#endif //LITTLEVULKANENGINE_FILEMANAGER_HPP
