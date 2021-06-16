#ifndef LITTLEENGINE_CONSTANTS_HPP
#define LITTLEENGINE_CONSTANTS_HPP


#include <string>


#ifdef NDEBUG
#define ENGINE_RELEASE

const bool enableValidationLayers = false;
#else
#define ENGINE_DEBUG

const bool enableValidationLayers = true;
#endif

#define THROW_EX(message) { \
    throw std::runtime_error(message); \
}

const std::string SHADERS_DIR = "bin/shaders/";


#endif //LITTLEENGINE_CONSTANTS_HPP
