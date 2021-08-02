#include "Camera.hpp"

#include <cassert>
#include <limits>
#include <cmath>


namespace lve {

    Camera::Camera() = default;

    void Camera::setOrthographicProjection(float left, float right, float top, float bottom, float near, float far) {
        projection = mat4(1.0f);
        projection[0][0] = 2.0f / (right - left);
        projection[1][1] = 2.0f / (bottom - top);
        projection[2][2] = 1.0f / (far - near);
        projection[3][0] = -(right + left) / (right - left);
        projection[3][1] = -(bottom + top) / (bottom - top);
        projection[3][2] = -near / (far - near);
    }

    void Camera::setPerspectiveProjection(float fovy, float aspect, float near, float far) {
        assert(std::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
        const float tanHalfFovy = std::tan(fovy * 0.5f);
        projection = mat4{0.0f};
        projection[0][0] = 1.0f / (aspect * tanHalfFovy);
        projection[1][1] = 1.0f / (tanHalfFovy);
        projection[2][2] = far / (far - near);
        projection[2][3] = 1.0f;
        projection[3][2] = -(far * near) / (far - near);
    }

    const Matrix4 &Camera::getProjection() const {
        return projection;
    }

} // namespace lv