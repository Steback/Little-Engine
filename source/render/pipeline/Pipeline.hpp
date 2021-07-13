#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_PIPELINE_PIPELINE_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_PIPELINE_PIPELINE_HPP


#include <vector>

#include "vulkan/vulkan.hpp"


namespace lve {

    class Device;

    class Pipeline {
    public:
        struct Config {
            vk::Viewport viewport;
            vk::Rect2D scissor;
            vk::PipelineViewportStateCreateInfo viewportInfo;
            vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
            vk::PipelineRasterizationStateCreateInfo rasterizationInfo;
            vk::PipelineMultisampleStateCreateInfo multisampleInfo;
            vk::PipelineColorBlendAttachmentState colorBlendAttachment;
            vk::PipelineColorBlendStateCreateInfo colorBlendInfo;
            vk::PipelineDepthStencilStateCreateInfo depthStencilInfo;
            vk::RenderPass renderPass = nullptr;
            uint32_t subpass = 0;
        };

    public:
        explicit Pipeline(const vk::Device& device);

        ~Pipeline();

        virtual void destroy();

        [[nodiscard]] const vk::Pipeline &getHandle() const;

        vk::ShaderModule createShaderModule(const std::vector<char>& code);

        void bind(const vk::CommandBuffer& cmdBuffer, vk::PipelineBindPoint bindPoint);

    protected:
        vk::Device device{};
        vk::Pipeline pipeline{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_RENDER_PIPELINE_PIPELINE_HPP
