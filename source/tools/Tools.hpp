#ifndef LITTLEVULKANENGINE_TOOLS_HPP
#define LITTLEVULKANENGINE_TOOLS_HPP


#include "fmt/format.h"


#define THROW_EX(message) { \
    throw std::runtime_error(fmt::format("{} {} {}", message, __LINE__, __FILE__)); \
}

#define VK_CHECK_RESULT(result, message) { \
    if (result != VK_SUCCESS) THROW_EX(message) \
}

namespace lve {

    inline uint32_t castU32(size_t n) {
        return static_cast<uint32_t>(n);
    }

} // namespace lve


#endif //LITTLEVULKANENGINE_TOOLS_HPP
