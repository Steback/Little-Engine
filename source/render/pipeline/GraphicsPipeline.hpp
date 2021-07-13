#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_PIPELINE_GRAPHICSPIPELINE_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_PIPELINE_GRAPHICSPIPELINE_HPP


#include <string>

#include "Pipeline.hpp"


namespace lve {

    class GraphicsPipeline : public Pipeline {
    public:
        GraphicsPipeline(const vk::Device& device, const std::string& vert, const std::string& frag,
                         const Pipeline::Config &config);

        ~GraphicsPipeline();

        void create(const std::string& vert, const std::string& frag, const Config& config);

        void destroy() override;

        [[nodiscard]] const vk::PipelineLayout &getLayout() const;

        static Config defaultConfig(const vk::RenderPass& renderPass, uint32_t width, uint32_t height);

    private:
        vk::PipelineLayout layout{};
        vk::ShaderModule vertShaderModule{};
        vk::ShaderModule fragShaderModule{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_RENDER_PIPELINE_GRAPHICSPIPELINE_HPP
