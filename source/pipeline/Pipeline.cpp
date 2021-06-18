#include "Pipeline.hpp"

#include <array>
#include <fstream>
#include <filesystem>

#include "spdlog/spdlog.h"

#include "../Device.hpp"
#include "../Constants.hpp"
#include "../Utilities.hpp"


Pipeline::Pipeline(const std::shared_ptr<Device>& device, const std::string &vert, const std::string &frag)
        : logicalDevice(device->getLogicalDevice()) {

}

Pipeline::~Pipeline() = default;

void Pipeline::create(const std::string& vert, const std::string& frag, const ConfigInfo& configInfo) {
    assert(configInfo.layout &&
            "Cannot create graphics pipeline: no pipelineLayout provided in config info");
    assert(configInfo.renderPass &&
            "Cannot create graphics pipeline: no renderPass provided in config info");

    std::vector<char> vertCode = readFile(vert);
    std::vector<char> fragCode = readFile(frag);

    vertModule = createShaderModule(vertCode);
    fragModule = createShaderModule(fragCode);

    std::array<vk::PipelineShaderStageCreateInfo,2 > shaderStages{
            vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eVertex, vertModule, "main", {}),
            vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eFragment, fragModule, "main", {})
    };

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo(
            {},
            0, nullptr,
            0, nullptr
    );

    vk::GraphicsPipelineCreateInfo createInfo(
            {},
            shaderStages,
            &vertexInputInfo,
            &configInfo.inputAssemblyInfo,
            nullptr,
            &configInfo.viewportInfo,
            &configInfo.rasterizationInfo,
            &configInfo.multisampleInfo,
            &configInfo.depthStencilInfo,
            &configInfo.colorBlendInfo,
            nullptr,
            configInfo.layout,
            configInfo.renderPass
    );

    vk::Result result;
    std::tie(result, pipeline) = logicalDevice.createGraphicsPipeline(nullptr, createInfo);

    switch ( result ) {
        case vk::Result::eSuccess: break;
        case vk::Result::ePipelineCompileRequiredEXT: THROW_EX("Failed to create graphics pipeline!")
        default: assert(false);
    }
}

void Pipeline::cleanup() {
    logicalDevice.destroy(fragModule);
    logicalDevice.destroy(vertModule);
    logicalDevice.destroy(pipeline);
}

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

vk::ShaderModule Pipeline::createShaderModule(const std::vector<char> &code) {
    vk::ShaderModuleCreateInfo moduleCreateInfo;
    moduleCreateInfo.codeSize = castU32(code.size());
    moduleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    return logicalDevice.createShaderModule(moduleCreateInfo);
}
