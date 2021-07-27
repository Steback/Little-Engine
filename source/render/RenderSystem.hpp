#ifndef LITTLEVULKANENGINE_RENDERSYSTEM_HPP
#define LITTLEVULKANENGINE_RENDERSYSTEM_HPP


#include <memory>

#include "vulkan/vulkan.hpp"

#include "../scene/Scene.hpp"


namespace lve {

    class Device;
    class GraphicsPipeline;

    class RenderSystem {
    public:
        RenderSystem(std::shared_ptr<Device> device, vk::RenderPass renderPass);

        RenderSystem(const RenderSystem&) = delete;

        ~RenderSystem();

        RenderSystem& operator=(const RenderSystem&) = delete;

        void renderEntities(vk::CommandBuffer commandBuffer, const registry_t& registry);

    private:
        std::shared_ptr<Device> device;
        std::unique_ptr<GraphicsPipeline> pipeline;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_RENDERSYSTEM_HPP
