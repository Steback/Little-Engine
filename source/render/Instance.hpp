#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_INSTANCE_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_INSTANCE_HPP


#include <vector>

#include "vulkan/vulkan.hpp"

#include "../Constants.hpp"


namespace lve {

    class Instance {
    public:
        explicit Instance(const std::vector<const char*>& reqLayer);

        Instance(const Instance&) = delete;

        ~Instance();

        Instance& operator =(const Instance&) = delete;

        void destroy();

        vk::PhysicalDevice pickPhysicalDevice(const std::vector<const char*>& extensions);

        [[nodiscard]] vk::Instance getHandle() const;

    private:
        vk::Instance handle{};
#ifdef LVE_DEBUG
        vk::DebugUtilsMessengerEXT debugMessenger{};
#endif
    };

} // namespace lve



#endif //LITTLEVULKANENGINE_SOURCE_RENDER_INSTANCE_HPP
