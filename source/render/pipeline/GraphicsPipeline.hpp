#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_PIPELINE_GRAPHICSPIPELINE_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_PIPELINE_GRAPHICSPIPELINE_HPP


#include <string>

#include "Pipeline.hpp"


namespace lve {

    class GraphicsPipeline : public Pipeline {
    public:
        GraphicsPipeline(const std::string& shadersName);

    private:

    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_RENDER_PIPELINE_GRAPHICSPIPELINE_HPP
