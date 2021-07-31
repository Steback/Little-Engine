#ifndef LITTLEVULKANENGINE_GRAPHICSPIPELINE_HPP
#define LITTLEVULKANENGINE_GRAPHICSPIPELINE_HPP


#include <string>

#include "Pipeline.hpp"


namespace lve {

    class Device;

    class GraphicsPipeline : public Pipeline, NonCopyable {
    public:
        GraphicsPipeline();

        GraphicsPipeline(VkDevice device, const std::string& vertName, const std::string& fragName,
                         const ConfigInfo& configInfo, const std::vector<VkPushConstantRange>& constantRanges);

        ~GraphicsPipeline() override;

        void destroy() override;

        void bind(VkCommandBuffer const &commandBuffer) const override;

        [[nodiscard]] VkPipeline const &getPipeline() const override;

        [[nodiscard]] VkPipelineLayout const &getLayout() const override;

        static void defaultConfigInfo(ConfigInfo& configInfo, VkRenderPass renderPass);

    private:
        void createShaderModule(const std::vector<char>& code, VkShaderModule& module);

    private:
        VkDevice device{};
        VkPipeline pipeline{};
        VkPipelineLayout layout{};
        VkShaderModule vertShaderModule{};
        VkShaderModule fragShaderModule{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_GRAPHICSPIPELINE_HPP
