#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_DEVICE_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_DEVICE_HPP


#include <memory>
#include <vector>

#include "vulkan/vulkan.h"


namespace lve {

    class Window;

    class Device {
    public:
        explicit Device(std::shared_ptr<Window> window);

        Device(const Device&) = delete;

        ~Device();

        Device& operator=(const Device&) = delete;

        void destroy();

        static VkPhysicalDeviceProperties getPhysicalDeviceProperties(const VkPhysicalDevice &device);

        static std::vector<VkExtensionProperties> getExtensionsProperties(const VkPhysicalDevice &device);

        static bool checkExtensionsSupport(const VkPhysicalDevice& device, const std::vector<const char*>& extensions);

    private:
        void createInstance();

        void pickPhysicalDevice();

        std::shared_ptr<Window> window;
        VkInstance instance{};
        VkPhysicalDevice physicalDevice{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_RENDER_DEVICE_HPP
