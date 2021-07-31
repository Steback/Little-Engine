#ifndef LITTLEVULKANENGINE_TRANSFORM_HPP
#define LITTLEVULKANENGINE_TRANSFORM_HPP


#include "math/Vector3.hpp"
#include "math/Matrix4.hpp"


namespace lve {

    class RenderSystem;

    class Transform {
        friend RenderSystem;

    public:
        Transform();

        Transform(const vec3& t, const vec3& r, const vec3& s);

        [[nodiscard]] const Vector3 &getTranslation() const;

        void setTranslation(const Vector3 &translation_);

        [[nodiscard]] const Vector3 &getRotation() const;

        void setRotation(const Vector3 &rotation_);

        [[nodiscard]] const Vector3 &getScale() const;

        void setScale(const Vector3 &scale_);

        [[nodiscard]] Matrix4 worldTransform() const;

    public:
        const static uint32_t type = 1 << 0;

    private:
        Vector3 translation;
        Vector3 rotation;
        Vector3 scale;
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_TRANSFORM_HPP
