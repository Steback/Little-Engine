#ifndef LITTLEVULKANENGINE_TOOLS_HPP
#define LITTLEVULKANENGINE_TOOLS_HPP


#include "fmt/format.h"


#define THROW_EX(message) { \
    throw std::runtime_error(fmt::format("{} {} {}", message, __FILE__, __LINE__)); \
}

#define VK_CHECK_RESULT(result, message) { \
    if (result != VK_SUCCESS) THROW_EX(message) \
}

#define VK_HPP_CHECK_RESULT(result, message) { \
    if (result != vk::Result::eSuccess) THROW_EX(message) \
}

#define VK_ERROR(result, message) { \
    THROW_EX(fmt::format("{} Error code: {}", message, vk::to_string(result))) \
}

namespace lve {

    inline uint32_t castU32(size_t n) {
        return static_cast<uint32_t>(n);
    }

} // namespace lve


#endif //LITTLEVULKANENGINE_TOOLS_HPP
