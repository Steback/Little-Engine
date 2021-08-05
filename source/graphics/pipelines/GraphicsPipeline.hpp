#ifndef LITTLEVULKANENGINE_GRAPHICSPIPELINE_HPP
#define LITTLEVULKANENGINE_GRAPHICSPIPELINE_HPP


#include <string>

#include "Pipeline.hpp"
#include "Shader.hpp"


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
        Shader vertexShader;
        Shader fragmentShader;
        VkDevice device{};
        VkPipeline pipeline{};
        VkPipelineLayout layout{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_GRAPHICSPIPELINE_HPP
