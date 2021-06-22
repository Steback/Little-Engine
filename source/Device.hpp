#ifndef LITTLEENGINE_DEVICE_HPP
#define LITTLEENGINE_DEVICE_HPP


#include <memory>

#include "vulkan/vulkan.hpp"


class Window;
class Instance;

class Device {
public:
    Device(std::shared_ptr<Window> window, std::shared_ptr<Instance> instance);

    void destroy();

private:
    std::shared_ptr<Window> window;
    std::shared_ptr<Instance> instance;
    vk::PhysicalDevice physicalDevice;
};


#endif //LITTLEENGINE_DEVICE_HPP
