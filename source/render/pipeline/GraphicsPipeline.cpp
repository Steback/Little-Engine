#include "GraphicsPipeline.hpp"

#include <array>
#include <cassert>

#include "../../tools/Tools.hpp"
#include "../../fileManager/FileManager.hpp"
#include "../../mesh/Mesh.hpp"
#include "../../logger/Logger.hpp"


namespace lve {

    GraphicsPipeline::GraphicsPipeline(const vk::Device& device, const std::string& vert, const std::string& frag,
                                       const Pipeline::Config &config) : Pipeline(device) {
        create(vert, frag, config);
    }

    GraphicsPipeline::~GraphicsPipeline() = default;

    void GraphicsPipeline::create(const std::string &vert, const std::string &frag, const Pipeline::Config &config) {
        assert(config.renderPass &&
            "Cannot create graphics pipeline: no renderPass provided in config info");

        vertShaderModule = createShaderModule(FileManager::readFile(FileManager::shadersPath() + vert));
        fragShaderModule = createShaderModule(FileManager::readFile(FileManager::shadersPath() + frag));

        std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;
        shaderStages[0] = vk::PipelineShaderStageCreateInfo(
                {}, // flags
                vk::ShaderStageFlagBits::eVertex, // stage
                vertShaderModule, // module
                "main", // pName
                nullptr // pSpecializationInfo
        );
        shaderStages[1] = vk::PipelineShaderStageCreateInfo(
                {}, // flags
                vk::ShaderStageFlagBits::eFragment, // stage
                fragShaderModule, // module
                "main", // pName
                nullptr // pSpecializationInfo
        );


        auto bindings = Mesh::Vertex::getBindingDescriptions();
        auto attributes = Mesh::Vertex::getAttributeDescriptions();
        vk::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo(
                {}, // flags
                castU32(bindings.size()), // vertexBindingDescriptionCount
                bindings.data(), // pVertexBindingDescriptions
                castU32(attributes.size()), // vertexAttributeDescriptionCount
                attributes.data() // pVertexAttributeDescriptions
        );

        vk::PipelineLayoutCreateInfo layoutInfo(
                {},
                0,
                nullptr,
                0,
                nullptr
        );

        layout = device.createPipelineLayout(layoutInfo);

        vk::GraphicsPipelineCreateInfo createInfo(
                {}, // flags
                castU32(shaderStages.size()), // stageCount
                shaderStages.data(), // pStages
                &vertexInputStateCreateInfo, // pVertexInputState
                &config.inputAssemblyInfo, // pInputAssemblyState
                nullptr, // pTessallationState
                &config.viewportInfo, // pViewportState
                &config.rasterizationInfo, // pRasterizationState
                &config.multisampleInfo, // pMultisampleState
                &config.depthStencilInfo, // pDepthStencilState
                &config.colorBlendInfo, // pColorBlendState
                nullptr, // pDynamicState
                layout, // layout
                config.renderPass, // renderPass
                config.subpass // subpass
        );

        vk::Result result;
        std::tie( result, pipeline ) = device.createGraphicsPipeline( nullptr, createInfo );
        switch (result) {
            case vk::Result::eSuccess: break;
            case vk::Result::ePipelineCompileRequiredEXT:
                EXIT_ERROR("Failed to create graphics pipeline")
        }

        device.destroy(vertShaderModule);
        vertShaderModule = nullptr;

        device.destroy(fragShaderModule);
        fragShaderModule = nullptr;
    }

    void GraphicsPipeline::destroy() {
        device.destroy(pipeline);
        device.destroy(layout);
    }

    const vk::PipelineLayout &GraphicsPipeline::getLayout() const {
        return layout;
    }

    Pipeline::Config GraphicsPipeline::defaultConfig(const vk::RenderPass& renderPass, uint32_t width, uint32_t height) {
        Config config;

        config.inputAssemblyInfo = vk::PipelineInputAssemblyStateCreateInfo(
                {}, // flags
                vk::PrimitiveTopology::eTriangleList, // topology
                false // primitiveRestartEnable
        );

        config.viewport = vk::Viewport(
                0.0f, // x
                0.0f, // y
                static_cast<float>(width), // width
                static_cast<float>(height), // height
                0.0f, // minDepth
                1.0f // maxDepth
        );

        config.scissor = vk::Rect2D(
                {0, 0}, // offset
                {width, height} // extent
        );

        config.viewportInfo = vk::PipelineViewportStateCreateInfo(
                {}, // flags
                1, // viewportCount
                &config.viewport, // pViewports
                1, // scissorCount
                &config.scissor // pScissors
        );

        config.rasterizationInfo = vk::PipelineRasterizationStateCreateInfo(
                {}, // flags
                false, // depthClampEnable
                false, // rasterizerDiscardEnable
                vk::PolygonMode::eFill, // polygonMode
                vk::CullModeFlagBits::eNone, // cullMode
                vk::FrontFace::eClockwise, // frontFace
                false, // depthBiasEnable
                0.0f, // depthBiasConstantFactor
                0.0f, // depthBiasClamp
                0.0f, // depthBiasSlopeFactor
                1.0f // lineWidth
        );

        config.multisampleInfo = vk::PipelineMultisampleStateCreateInfo(
                {}, // flags
                vk::SampleCountFlagBits::e1, // rasterizationSamples
                false, // sampleShadingEnable
                1.0f, // minSampleShading
                nullptr, // pSampleMask
                false, // alphaToCoverageEnable
                false // alphaToOneEnable
        );

        config.colorBlendAttachment = vk::PipelineColorBlendAttachmentState(
                false,  // blendEnable
                vk::BlendFactor::eOne, // srcColorBlendFactor
                vk::BlendFactor::eZero, // dstColorBlendFactor
                vk::BlendOp::eAdd, // colorBlendOp
                vk::BlendFactor::eOne, // srcAlphaBlendFactor
                vk::BlendFactor::eZero, // dstAlphaBlendFactor
                vk::BlendOp::eAdd, // alphaBlendOp
                vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB
                    | vk::ColorComponentFlagBits::eA // colorBlendAttachment
        );

        config.colorBlendInfo = vk::PipelineColorBlendStateCreateInfo(
                {}, // flags
                false, // logicOpEnable
                vk::LogicOp::eCopy, // logicOp
                1, // attachmentCount
                &config.colorBlendAttachment, // pAttachments
                std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f} // blendConstants
        );

        config.depthStencilInfo = vk::PipelineDepthStencilStateCreateInfo(
                {}, // flags
                false, // depthTestEnable
                false, // depthWriteEnable
                vk::CompareOp::eLess, // depthCompareOp
                false, // depthBoundsTestEnable
                false, // stencilTestEnable
                {}, // front
                {}, // back
                0.0f, // minDepthBounds
                1.0f // maxDepthBounds
        );

        config.renderPass = renderPass;

        return config;
    }

    void GraphicsPipeline::bind(const vk::CommandBuffer& cmdBuffer) {
        Pipeline::bind(cmdBuffer, vk::PipelineBindPoint::eGraphics);
    }

} // namespace lve
