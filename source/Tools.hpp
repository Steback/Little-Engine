#ifndef LITTLEENGINE_TOOLS_HPP
#define LITTLEENGINE_TOOLS_HPP


#include <cstdint>
#include <cstddef>

#include "fmt/format.h"


#ifdef NDEBUG
#define ERROR_MESSAGE(message) { fmt::format("{}", message) }
#else
#define ERROR_MESSAGE(message) { fmt::format("{} {} {}", message, __FILE__, __LINE__) }
#endif

#define THROW_EX(message) { \
    throw std::runtime_error(ERROR_MESSAGE(message)); \
}

uint32_t castU32(size_t n);


#endif //LITTLEENGINE_TOOLS_HPP
