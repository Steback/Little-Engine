#ifndef LITTLEVULKANENGINE_FILEMANAGER_HPP
#define LITTLEVULKANENGINE_FILEMANAGER_HPP


#include <filesystem>
#include <vector>


namespace lve {

    class FileManager {
    public:
        ~FileManager();

        static void setupPaths();

        [[nodiscard]] static std::filesystem::path rootPath();

        [[nodiscard]] static std::filesystem::path dataPath();

        [[nodiscard]] static std::filesystem::path shadersPath();

        [[nodiscard]] static std::string getFile(const std::string& fileName) ;

        static bool existsFile(const std::filesystem::path& path);

        static void pathExists(const std::filesystem::path& path, const std::string& name);

        static std::vector<char> readFile(const std::string& name);

    private:
        FileManager();

        static std::filesystem::path root;
        static std::filesystem::path data;
        static std::filesystem::path shaders;
    };

} // namespace lve

inline std::string operator+(const std::filesystem::path& path, const std::string& name) {
    return (std::filesystem::path(path) /= name).string();
}


#endif //LITTLEVULKANENGINE_FILEMANAGER_HPP
