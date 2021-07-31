#ifndef LITTLEVULKANENGINE_COMMON_HPP
#define LITTLEVULKANENGINE_COMMON_HPP


#include <cmath>


namespace lve {

    inline float pi() {
        return 3.14159265358979323846264338327950288;
    }

    inline float twoPi() {
        return 2 * pi();
    }

    inline float mod(float a, float b) {
        return a - b * std::floor(a / b);
    }

}


#endif //LITTLEVULKANENGINE_COMMON_HPP
