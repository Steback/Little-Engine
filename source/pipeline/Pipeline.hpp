#ifndef LITTLEENGINE_PIPELINE_HPP
#define LITTLEENGINE_PIPELINE_HPP


#include <string>
#include <vector>

#include "vulkan/vulkan.hpp"


class Device;

class Pipeline {
public:
    struct ConfigInfo{
        vk::Viewport viewport;
        vk::Rect2D scissor;
        vk::PipelineViewportStateCreateInfo viewportInfo;
        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        vk::PipelineRasterizationStateCreateInfo rasterizationInfo;
        vk::PipelineMultisampleStateCreateInfo multisampleInfo;
        vk::PipelineColorBlendAttachmentState colorBlendAttachment;
        vk::PipelineColorBlendStateCreateInfo colorBlendInfo;
        vk::PipelineDepthStencilStateCreateInfo depthStencilInfo;
        vk::PipelineLayout layout{nullptr};
        vk::RenderPass renderPass{nullptr};
        uint32_t subpass = 0;
    };

public:
    Pipeline(const std::shared_ptr<Device>& device, const std::string& vert, const std::string& frag);

    ~Pipeline();

    void create(const std::string& vert, const std::string& frag, const ConfigInfo& configInfo);

    void cleanup();

protected:
    static std::vector<char> readFile(const std::string& name);

    vk::ShaderModule createShaderModule(const std::vector<char>& code);

protected:
    vk::Device logicalDevice;
    vk::Pipeline pipeline;
    vk::ShaderModule vertModule;
    vk::ShaderModule fragModule;
};


#endif //LITTLEENGINE_PIPELINE_HPP
