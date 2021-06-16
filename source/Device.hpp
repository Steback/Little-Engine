#ifndef LITTLEENGINE_DEVICE_HPP
#define LITTLEENGINE_DEVICE_HPP


#include <string>
#include <vector>
#include <memory>

#include "vulkan/vulkan.hpp"


class Window;

class Device {
public:
    explicit Device(std::shared_ptr<Window> window);

    ~Device();

    void destroy();

private:
    static bool checkLayers(const std::vector<const char*>& layers, const std::vector<vk::LayerProperties>& properties);

    void createInstance();

    void selectPhysicalDevice();

private:
    std::shared_ptr<Window> window;
    vk::Instance instance;
    vk::DebugUtilsMessengerEXT debugUtilsMessenger;
    vk::PhysicalDevice physicalDevice;
};


#endif //LITTLEENGINE_DEVICE_HPP
