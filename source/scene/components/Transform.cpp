#include "Transform.hpp"

#include <cmath>

#include "../../math/Matrix2.hpp"


namespace lve::component {

        Transform::Transform() = default;

        Transform::Transform(const vec2& t, float r, const vec2& s)
                : translation(t), rotation(r), scale(s) {

        }

        Matrix2 Transform::getWorldMatrix() const {
            const float s = std::sin(rotation);
            const float c = std::cos(rotation);
            Matrix2 rotMatrix({c, -s}, {s, c});
            Matrix2 scaleMatrix({scale.x, 0.0f}, {0.0f, scale.y});

            return rotMatrix * scaleMatrix;
        }

} // namespace lve::component