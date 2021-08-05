#include "Shader.hpp"

#include "utils/Macros.hpp"
#include "files/File.hpp"
#include "files/FilesManager.hpp"


namespace lve {

    Shader::Shader(const VkDevice& device, const std::string& fileName, const VkShaderStageFlagBits& stage)
            : device(device), stage(stage) {
        auto vertCode = FilesManager::getFile(fileName).read();
        createShaderModule(vertCode, module);
    }

    Shader::~Shader() = default;

    void Shader::destroy() {
        vkDestroyShaderModule(device, module, nullptr);
    }

    VkPipelineShaderStageCreateInfo Shader::getPipelineStageCreateInfo(uint32_t flags) const {
        return {
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            nullptr,
            flags,
            stage,
            module,
            "main",
            nullptr
        };
    }

    const VkShaderModule &Shader::getModule() {
        return module;
    }

    VkShaderStageFlags Shader::getStage() {
        return stage;
    }

    void Shader::createShaderModule(const std::vector<char> &code, VkShaderModule &module) {
        VkShaderModuleCreateInfo createInfo{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        LVE_VK_CHECK_RESULT(vkCreateShaderModule(device, &createInfo, nullptr, &module),
                            "Failed to create shader module");
    }

} // namespace lv