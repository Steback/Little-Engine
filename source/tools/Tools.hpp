#ifndef LITTLEVULKANENGINE_TOOLS_HPP
#define LITTLEVULKANENGINE_TOOLS_HPP


#include "fmt/format.h"


#define THROW_EX(message) { \
    throw std::runtime_error(fmt::format("{} {} {}", message, __LINE__, __FILE__)); \
}


namespace lve {



} // namespace lve


#endif //LITTLEVULKANENGINE_TOOLS_HPP
