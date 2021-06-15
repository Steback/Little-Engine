#include "vulkan/vulkan.hpp"#include "spdlog/spdlog.h"#include "Base.hpp"int main() {    std::unique_ptr<Base> engine = std::make_unique<Base>();    uint32_t extensionCount = 0;    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);    spdlog::info("{} extensions supported", extensionCount);    engine->loop();    return 0;}