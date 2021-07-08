#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_DEVICE_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_DEVICE_HPP


#include <vector>

#include "vulkan/vulkan.hpp"

#include "../tools/Constants.hpp"


namespace lve {

    class Instance;

    class Device {
    public:
        explicit Device(const std::shared_ptr<Instance>& instance);

        Device(const Device&) = delete;

        ~Device();

        Device& operator=(const Device&) = delete;

        void destroy();

    private:
        void createLogicalDevice();

        vk::PhysicalDevice physicalDevice{};
        vk::Device logicalDevice{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_RENDER_DEVICE_HPP
