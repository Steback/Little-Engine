#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_PIPELINE_PIPELINE_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_PIPELINE_PIPELINE_HPP


#include <string>

#include "vulkan/vulkan.h"


namespace lve {

    class Pipeline {
    public:
        Pipeline(const std::string& shadersName);

    private:
        VkPipeline pipeline{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_RENDER_PIPELINE_PIPELINE_HPP
