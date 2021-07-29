#ifndef LITTLEVULKANENGINE_MACROS_HPP
#define LITTLEVULKANENGINE_MACROS_HPP


#include "fmt/format.h"


#if !defined(NDEBUG)
#define LVE_DEBUG
#endif

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LVE_ERROR_MESSAGE(message) fmt::format("{} {} {}", message, __FILENAME__, __LINE__)

#define LVE_THROW_EX(message) { \
    throw std::runtime_error(LVE_ERROR_MESSAGE(message)); \
}

#define LVE_CASTU32(num) static_cast<uint32_t>(num)

#define LVE_VK_CHECK_RESULT(result, message) { \
    if (result != VK_SUCCESS) LVE_THROW_EX(message); \
}

#define LVE_VK_CHECK_RESULT_EXIT(result, message) { \
    if (result != VK_SUCCESS) LVE_LOG_ERROR_EXIT(message); \
}


#endif //LITTLEVULKANENGINE_MACROS_HPP
