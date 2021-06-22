#include "Device.hpp"

#include <utility>

#include "Window.hpp"
#include "Instance.hpp"


Device::Device(std::shared_ptr<Window> window, std::shared_ptr<Instance> instance)
        : window(std::move(window)), instance(std::move(instance)) {
    std::vector<const char*> reqExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    physicalDevice = this->instance->selectPhysicalDevice(reqExtensions);
}

void Device::destroy() {

}
