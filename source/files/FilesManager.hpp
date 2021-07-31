#ifndef LITTLEVULKANENGINE_FILESMANAGER_HPP
#define LITTLEVULKANENGINE_FILESMANAGER_HPP


#include <string>
#include <unordered_map>
#include <filesystem>


namespace lve {

    class File;

    class FilesManager {
    public:
        ~FilesManager();

        static void setRootPath();

        static void addPath(const std::string& name, bool create = false);

        static File getFile(const std::string& fileName);

        static std::filesystem::path getPath(const std::string& name);

    private:
        FilesManager();

        static std::unordered_map<std::string, std::filesystem::path> paths;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_FILESMANAGER_HPP
