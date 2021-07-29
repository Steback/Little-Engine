#ifndef LITTLEVULKANENGINE_FILESMANAGER_HPP
#define LITTLEVULKANENGINE_FILESMANAGER_HPP


#include <string>
#include <unordered_map>
#include <filesystem>


#include "utils/NonCopyable.hpp"


namespace lve {

    class File;

    class FilesManager : NonCopyable {
    public:
        FilesManager();

        ~FilesManager() override;

        void addPath(const std::string& name);

        File getFile(const std::string& fileName);

    private:
        std::unordered_map<std::string, std::filesystem::path> paths;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_FILESMANAGER_HPP
