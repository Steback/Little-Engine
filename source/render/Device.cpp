#include "Device.hpp"

#include "spdlog/spdlog.h"

#include "../BaseApp.hpp"
#include "../config/Config.hpp"
#include "../tools/Tools.hpp"

#include "Instance.hpp"


namespace lve {

    Device::Device(const std::shared_ptr<Instance>& instance) {
        physicalDevice = instance->pickPhysicalDevice({VK_KHR_SWAPCHAIN_EXTENSION_NAME});
    }

    Device::~Device() = default;

    void Device::destroy() {

    }

    void Device::createLogicalDevice() {

    }

} // namespace lve