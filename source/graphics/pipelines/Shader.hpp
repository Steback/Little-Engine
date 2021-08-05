#ifndef LITTLEVULKANENGINE_SHADER_HPP
#define LITTLEVULKANENGINE_SHADER_HPP


#include <string>

#include "vulkan/vulkan.h"
#include "spirv_cross.hpp"


namespace lve {

    class Shader {
    public:
        Shader() = default;

        Shader(const VkDevice& device, const std::string& fileName, const VkShaderStageFlagBits& stage);

        ~Shader();

        void destroy();

        [[nodiscard]] VkPipelineShaderStageCreateInfo getPipelineStageCreateInfo(uint32_t flags = 0) const;

        const VkShaderModule& getModule();

        VkShaderStageFlags getStage();

    private:
        void createShaderModule(const std::vector<char>& code, VkShaderModule& module);

    private:
        VkDevice device{};
        VkShaderModule module{};
        VkShaderStageFlagBits stage{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SHADER_HPP
