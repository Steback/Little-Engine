#include "Transform.hpp"

#include <cmath>


namespace lve {

    Transform::Transform() = default;

    Transform::Transform(const vec3 &t, const vec3 &r, const vec3 &s) : translation(t), rotation(r), scale(s) {

    }

    const Vector3 &Transform::getTranslation() const {
        return translation;
    }

    void Transform::setTranslation(const Vector3 &translation_) {
        Transform::translation = translation_;
    }

    const Vector3 &Transform::getRotation() const {
        return rotation;
    }

    void Transform::setRotation(const Vector3 &rotation_) {
        Transform::rotation = rotation_;
    }

    const Vector3 &Transform::getScale() const {
        return scale;
    }

    void Transform::setScale(const Vector3 &scale_) {
        Transform::scale = scale_;
    }

    // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
    // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
    // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
    Matrix4 Transform::worldTransform() const {
        const float c3 = std::cos(rotation.z);
        const float s3 = std::sin(rotation.z);
        const float c2 = std::cos(rotation.x);
        const float s2 = std::sin(rotation.x);
        const float c1 = std::cos(rotation.y);
        const float s1 = std::sin(rotation.y);
        return mat4{
            {
                scale.x * (c1 * c3 + s1 * s2 * s3),
                scale.x * (c2 * s3),
                scale.x * (c1 * s2 * s3 - c3 * s1),
                0.0f,
            },{
                scale.y * (c3 * s1 * s2 - c1 * s3),
                scale.y * (c2 * c3),
                scale.y * (c1 * c3 * s2 + s1 * s3),
                0.0f,
            },{
                scale.z * (c2 * s1),
                scale.z * (-s2),
                scale.z * (c1 * c2),
                0.0f,
            }, {
                translation.x, translation.y, translation.z, 1.0f
            }
        };
    }

} // namespace lv