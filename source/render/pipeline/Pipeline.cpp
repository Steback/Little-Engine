#include "Pipeline.hpp"

#include "../../tools/Tools.hpp"


namespace lve {

    Pipeline::Pipeline(const vk::Device& device) : device{device} {

    }

    Pipeline::~Pipeline() = default;

    void Pipeline::destroy() {
        device.destroy(pipeline);
    }

    const vk::Pipeline &Pipeline::getPipeline() const {
        return pipeline;
    }

    vk::ShaderModule Pipeline::createShaderModule(const std::vector<char> &code) {
        vk::ShaderModuleCreateInfo createInfo(
                {},
                castU32(code.size()),
                reinterpret_cast<const uint32_t*>(code.data())
        );

        return device.createShaderModule(createInfo);
    }

} // namespace lve