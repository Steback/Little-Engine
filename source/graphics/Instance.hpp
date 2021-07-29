#ifndef LITTLEVULKANENGINE_INSTANCE_HPP
#define LITTLEVULKANENGINE_INSTANCE_HPP


#include <vector>

#include "vulkan/vulkan.h"

#include "utils/Macros.hpp"
#include "utils/NonCopyable.hpp"


namespace lve {

    class Instance : NonCopyable {
    public:
        Instance(const std::vector<const char*>& layers, const char* appName);

        ~Instance() override;

        VkPhysicalDevice pickPhysicalDevice(const std::vector<const char*>& extensions);

        const VkInstance& getHandle();

        static std::vector<const char*> getRequiredExtensions();

        static bool checkExtensionsSupport(const VkPhysicalDevice &device, const std::vector<const char *> &extensions);

    private:
        static void hasGflwRequiredInstanceExtensions();

        static bool checkLayersSupport(const std::vector<const char*>& layers);

    private:
        VkInstance instance{};
#ifdef LVE_DEBUG
        VkDebugUtilsMessengerEXT debugMessenger{};
#endif
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_INSTANCE_HPP
