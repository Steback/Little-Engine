#ifndef LITTLEENGINE_INSTANCE_HPP
#define LITTLEENGINE_INSTANCE_HPP


#include <unordered_map>
#include <vector>

#include "vulkan/vulkan.hpp"

#include "Constants.hpp"


class Instance {
#ifdef ENGINE_DEBUG
    friend VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageFunc(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                           VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                                           VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
                                                           void* pUserData);
#endif

public:
    explicit Instance(const vk::ApplicationInfo& appInfo, const std::vector<const char*>& reqLayers);

    ~Instance();

    void destroy();

    [[nodiscard]] const vk::Instance& getHandle() const;

    void destroy(const vk::SurfaceKHR& surface);

private:
    vk::Instance instance{};
#ifdef ENGINE_DEBUG
    vk::DebugUtilsMessengerEXT debugMessenger{};
    std::unordered_map<uint32_t, std::string> debugMessages;
#endif
};


#endif //LITTLEENGINE_INSTANCE_HPP
