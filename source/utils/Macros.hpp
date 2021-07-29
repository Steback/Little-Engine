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


#endif //LITTLEVULKANENGINE_MACROS_HPP
