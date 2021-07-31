#ifndef LITTLEVULKANENGINE_PIPELINE_HPP
#define LITTLEVULKANENGINE_PIPELINE_HPP


#include <vector>

#include "vulkan/vulkan.h"

#include "utils/NonCopyable.hpp"


namespace lve {

    class Pipeline {
    public:
        struct ConfigInfo : NonCopyable {
            VkPipelineViewportStateCreateInfo viewportInfo{};
            VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
            VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
            VkPipelineMultisampleStateCreateInfo multisampleInfo{};
            VkPipelineColorBlendAttachmentState colorBlendAttachment{};
            VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
            VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
            std::vector<VkDynamicState> dynamicStateEnables;
            VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
            VkRenderPass renderPass{nullptr};
            uint32_t subpass{0};
        };

    public:
        Pipeline() = default;

        ~Pipeline() = default;

        virtual void destroy() = 0;

        virtual void bind(const VkCommandBuffer& commandBuffer) const = 0;

        [[nodiscard]] virtual const VkPipeline& getPipeline() const = 0;

        [[nodiscard]] virtual const VkPipelineLayout& getLayout() const = 0;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_PIPELINE_HPP
