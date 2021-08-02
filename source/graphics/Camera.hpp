#ifndef LITTLEVULKANENGINE_CAMERA_HPP
#define LITTLEVULKANENGINE_CAMERA_HPP


#include "math/Matrix4.hpp"
#include "math/Vector3.hpp"


namespace lve {

    class Camera {
    public:
        Camera();

        void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);

        void setPerspectiveProjection(float fovy, float aspect, float near, float far);

        void setViewDirection(const vec3& position, const vec3& direction, const vec3& up = {0.0f, -1.0f, 0.0f});

        void setViewTarget(const vec3& position, const vec3& target, const vec3& up = {0.f, -1.f, 0.f});

        void setViewYXZ(const vec3& position, const vec3& rotation);

        [[nodiscard]] const Matrix4& getProjection() const;

        [[nodiscard]] const Matrix4& getView() const;

    private:
        Matrix4 projection{1.0f};
        Matrix4 view{1.0f};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_CAMERA_HPP
