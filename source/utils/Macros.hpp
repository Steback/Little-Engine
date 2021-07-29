#ifndef LITTLEVULKANENGINE_MACROS_HPP
#define LITTLEVULKANENGINE_MACROS_HPP


#include "fmt/format.h"


#define LVE_THROW_EX(message) { \
    throw std::runtime_error(fmt::format("{} {} {}", message, FILENAME, __LINE__)); \
}


#endif //LITTLEVULKANENGINE_MACROS_HPP
