#ifndef LITTLEVULKANENGINE_LOGS_HPP
#define LITTLEVULKANENGINE_LOGS_HPP


#include "spdlog/spdlog.h"

#include "Logger.hpp"
#include "utils/Macros.hpp"


#define LVE_LOG_ERROR(message) { \
    lve::Logger::getFile("error.log")->error(LVE_ERROR_MESSAGE(message)); \
}

#define LVE_LOG_ERROR_EXIT(message) { \
    LVE_LOG_ERROR(message) \
    std::exit(EXIT_FAILURE); \
}


#endif //LITTLEVULKANENGINE_LOGS_HPP
