#ifndef LITTLEVULKANENGINE_COMPONENTS_TRANSFORM_HPP
#define LITTLEVULKANENGINE_COMPONENTS_TRANSFORM_HPP


#include "../../math/Vector3.hpp"


namespace lve {

    class Matrix4;

    class Transform {
    public:
        Transform();

        Transform(const vec3& t, const vec3& r, const vec3& s);

        [[nodiscard]] Matrix4 getWorldMatrix() const;

    public:
        vec3 translation;
        vec3 scale{1.0f};
        vec3 rotation{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_COMPONENTS_TRANSFORM_HPP
