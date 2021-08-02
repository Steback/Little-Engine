#ifndef LITTLEVULKANENGINE_CAMERA_HPP
#define LITTLEVULKANENGINE_CAMERA_HPP


#include "math/Matrix4.hpp"


namespace lve {

    class Camera {
    public:
        Camera();

        void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);

        void setPerspectiveProjection(float fovy, float aspect, float near, float far);

        [[nodiscard]] const Matrix4& getProjection() const;

    private:
        Matrix4 projection{1.0f};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_CAMERA_HPP
