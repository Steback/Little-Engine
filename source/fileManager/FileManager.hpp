#ifndef LITTLEVULKANENGINE_FILEMANAGER_HPP
#define LITTLEVULKANENGINE_FILEMANAGER_HPP


#include <filesystem>


namespace lve {

    enum FileDirectory {
        ROOT,
        DATA
    };

    class FileManager {
    public:
        FileManager();

        ~FileManager();

        [[nodiscard]] std::string getFile(FileDirectory dir, const std::string& fileName) const;

        [[nodiscard]] std::string getRoot() const;

        [[nodiscard]] std::string getData() const;

        static bool existsFile(const std::filesystem::path& path) ;

    private:
        std::filesystem::path root{};
        std::filesystem::path data{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_FILEMANAGER_HPP
