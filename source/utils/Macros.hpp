#ifndef LITTLEVULKANENGINE_MACROS_HPP
#define LITTLEVULKANENGINE_MACROS_HPP


#include "fmt/format.h"


#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LVE_THROW_EX(message) { \
    throw std::runtime_error(fmt::format("{} {} {}", message, __FILENAME__, __LINE__)); \
}


#endif //LITTLEVULKANENGINE_MACROS_HPP
