#ifndef LITTLEVULKANENGINE_FILE_HPP
#define LITTLEVULKANENGINE_FILE_HPP


#include <vector>
#include <filesystem>


namespace lve {

    class File {
    public:
        File();

        explicit File(std::filesystem::path  path);

        ~File();

        [[nodiscard]] const std::filesystem::path &getPath() const;

        void setPath(const std::filesystem::path &newPath);

        std::vector<char> read();

    private:
        std::filesystem::path path;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_FILE_HPP
