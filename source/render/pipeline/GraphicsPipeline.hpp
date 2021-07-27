#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_PIPELINE_GRAPHICSPIPELINE_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_PIPELINE_GRAPHICSPIPELINE_HPP


#include <string>

#include "Pipeline.hpp"
#include "../../math/Vector2.hpp"
#include "../../math/Vector3.hpp"
#include "../../math/Matrix2.hpp"


namespace lve {

    struct SimplePushConstantData {
        mat2 transform{1.0f};
        vec2 offset;
        alignas(16) vec3 color;
    };

    class GraphicsPipeline : public Pipeline {
    public:
        GraphicsPipeline(const vk::Device& device, const std::string& vert, const std::string& frag,
                         const Pipeline::Config &config);

        ~GraphicsPipeline();

        void create(const std::string& vert, const std::string& frag, const Config& config);

        void destroy() override;

        [[nodiscard]] const vk::PipelineLayout &getLayout() const;

        void bind(const vk::CommandBuffer& cmdBuffer);

        static Config defaultConfig(const vk::RenderPass& renderPass, const std::vector<vk::DynamicState>& dynamicStateEnables);

    private:
        vk::PipelineLayout layout{};
        vk::ShaderModule vertShaderModule{};
        vk::ShaderModule fragShaderModule{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_RENDER_PIPELINE_GRAPHICSPIPELINE_HPP
