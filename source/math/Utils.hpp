#ifndef LITTLEVULKANENGINE_MATH_UTILS_HPP
#define LITTLEVULKANENGINE_MATH_UTILS_HPP


#include <cmath>


namespace lve {

    inline float pi() {
        return 3.14159265358979323846264338327950288;
    }

    inline float mod(float a, float b) {
        return a - b * std::floor(a / b);
    }

} // namespace lve


#endif //LITTLEVULKANENGINE_MATH_UTILS_HPP
