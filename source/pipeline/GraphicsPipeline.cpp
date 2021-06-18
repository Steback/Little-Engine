#include "GraphicsPipeline.hpp"


GraphicsPipeline::GraphicsPipeline(const std::shared_ptr<Device>& device, const std::string &vert, const std::string &frag,
                                   const ConfigInfo& configInfo)
        : Pipeline(device, vert, frag) {
    create(vert, frag, configInfo);
}

GraphicsPipeline::~GraphicsPipeline() = default;


GraphicsPipeline::ConfigInfo GraphicsPipeline::defaultConfigInfo(uint32_t width, uint32_t height) {
    Pipeline::ConfigInfo configInfo{};

    configInfo.inputAssemblyInfo = vk::PipelineInputAssemblyStateCreateInfo(
            {},
            vk::PrimitiveTopology::eTriangleList,
            false
    );

    configInfo.viewport = vk::Viewport(
            0.0f,
            0.0f,
            static_cast<float>(width),
            static_cast<float>(height),
            0.0f,
            1.0f
    );

    configInfo.scissor = vk::Rect2D(
            {0, 0},
            {width, height}
    );

    configInfo.viewportInfo = vk::PipelineViewportStateCreateInfo(
            {},
            1,
            &configInfo.viewport,
            1,
            &configInfo.scissor
    );

    configInfo.rasterizationInfo = vk::PipelineRasterizationStateCreateInfo(
            {},
            false,
            false,
            vk::PolygonMode::eFill,
            vk::CullModeFlagBits::eNone,
            vk::FrontFace::eClockwise,
            false,
            0.0f,
            0.0f,
            0.0f,
            1.0f
    );

    configInfo.multisampleInfo = vk::PipelineMultisampleStateCreateInfo(
            {},
            vk::SampleCountFlagBits::e1,
            false
    );

    configInfo.colorBlendAttachment = vk::PipelineColorBlendAttachmentState(
            false,
            vk::BlendFactor::eZero,
            vk::BlendFactor::eZero,
            vk::BlendOp::eAdd,
            vk::BlendFactor::eZero,
            vk::BlendFactor::eZero,
            vk::BlendOp::eAdd,
            vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
    );

    configInfo.colorBlendInfo = vk::PipelineColorBlendStateCreateInfo(
            {},
            false,
            vk::LogicOp::eCopy,
            configInfo.colorBlendAttachment,
            { { 1.0f, 1.0f, 1.0f, 1.0f } }
    );

    vk::StencilOpState stencilOpState(vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::StencilOp::eKeep,
                                      vk::CompareOp::eAlways);
    configInfo.depthStencilInfo = vk::PipelineDepthStencilStateCreateInfo(
            {},
            true,
            true,
            vk::CompareOp::eLess,
            false,
            false,
            stencilOpState,
            stencilOpState
    );

    return configInfo;
}
