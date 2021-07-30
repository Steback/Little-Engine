#ifndef LITTLEVULKANENGINE_RENDERER_HPP
#define LITTLEVULKANENGINE_RENDERER_HPP


#include <memory>

#include "vulkan/vulkan.h"

#include "utils/NonCopyable.hpp"


namespace lve {

    class Window;
    class Config;
    class Instance;
    class Device;
    class SwapChain;

    class Renderer : NonCopyable {
    public:
        explicit Renderer(std::shared_ptr<Window> window, const Config& config);

        ~Renderer() override;

        void cleanup();

        void setupDrawResources();

    private:
        std::shared_ptr<Window> window;
        std::shared_ptr<Instance> instance;
        std::shared_ptr<Device> device;
        std::unique_ptr<SwapChain> swapChain;
        VkSurfaceKHR surface{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_RENDERER_HPP
