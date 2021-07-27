#ifndef LITTLEVULKANENGINE_COMPONENTS_TRANSFORM_HPP
#define LITTLEVULKANENGINE_COMPONENTS_TRANSFORM_HPP


#include "../../math/Vector2.hpp"


namespace lve {

    class Matrix2;

    namespace component {

        class Transform {
        public:
            Transform();

            Transform(const vec2& t, float r, const vec2& s);

            [[nodiscard]] Matrix2 getWorldMatrix() const;

        public:
            vec2 translation;
            vec2 scale{1.0f};
            float rotation{};
        };

    } // namespace component

} // namespace lve


#endif //LITTLEVULKANENGINE_COMPONENTS_TRANSFORM_HPP
