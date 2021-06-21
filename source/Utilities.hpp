#ifndef LITTLEENGINE_UTILITIES_HPP
#define LITTLEENGINE_UTILITIES_HPP


#include <cstdint>
#include <cstddef>


#define THROW_EX(message) { \
    throw std::runtime_error(message); \
}

uint32_t castU32(size_t n);


#endif //LITTLEENGINE_UTILITIES_HPP
