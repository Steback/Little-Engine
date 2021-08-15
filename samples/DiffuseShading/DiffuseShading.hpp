#ifndef LITTLEVULKANENGINE_DIFFUSESHADING_HPP
#define LITTLEVULKANENGINE_DIFFUSESHADING_HPP


#include "App.hpp"

#include "KeyboardMovementController.hpp"
#include "entity/Entity.hpp"


namespace lve {

    class DiffuseShading : public App {
    public:
        DiffuseShading();

        ~DiffuseShading() override;

        void start() override;

        void onUpdate(float deltaTime) override;

        void onDrawEntity(VkPipelineLayout layout, VkCommandBuffer commandBuffer, id_t entityID) override;

    private:
        KeyboardMovementController cameraController;
        id_t cameraID{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_DIFFUSESHADING_HPP
