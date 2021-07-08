#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_DEVICE_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_DEVICE_HPP


#include <memory>
#include <vector>

#include "vulkan/vulkan.hpp"

#include "../tools/Constants.hpp"


namespace lve {

    class Window;

    class Device {
    public:
        explicit Device(std::shared_ptr<Window> window);

        Device(const Device&) = delete;

        ~Device();

        Device& operator=(const Device&) = delete;

        void destroy();

        static bool checkExtensionsSupport(const vk::PhysicalDevice &devic, const std::vector<const char*>& extensions);

    private:
        void createInstance();

        void pickPhysicalDevice();

        std::shared_ptr<Window> window;
        vk::Instance instance{};
#ifdef LVE_DEBUG
        vk::DebugUtilsMessengerEXT debugMessenger{};
#endif
        vk::PhysicalDevice physicalDevice{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_RENDER_DEVICE_HPP
