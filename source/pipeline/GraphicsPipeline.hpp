#ifndef LITTLEENGINE_GRAPHICSPIPELINE_HPP
#define LITTLEENGINE_GRAPHICSPIPELINE_HPP


#include "Pipeline.hpp"


class GraphicsPipeline : public Pipeline {
public:
    GraphicsPipeline(const std::shared_ptr<Device>& device, const std::string &vert, const std::string &frag,
                     const ConfigInfo& configInfo);

    ~GraphicsPipeline();

    static ConfigInfo defaultConfigInfo(uint32_t width, uint32_t height);
};


#endif //LITTLEENGINE_GRAPHICSPIPELINE_HPP
