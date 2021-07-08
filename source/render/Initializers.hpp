#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_INITIALIZERS_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_INITIALIZERS_HPP


namespace lve::initializers {

    inline VkApplicationInfo applicationInfo(const char* appName = "", uint32_t appVersion = 0,
                                             const char* engineName = "", uint32_t engineVersion = 0) {
        return {
            VK_STRUCTURE_TYPE_APPLICATION_INFO,
            nullptr,
            appName,
            appVersion,
            engineName,
            engineVersion,
            VK_API_VERSION_1_2
        };
    }

    inline VkInstanceCreateInfo instanceCreateInfo(const VkApplicationInfo& appInfo = {}, uint32_t flags = 0) {
        return {
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            nullptr,
            flags,
            &appInfo
        };
    }

} // namespace lve::initializers


#endif //LITTLEVULKANENGINE_SOURCE_RENDER_INITIALIZERS_HPP
