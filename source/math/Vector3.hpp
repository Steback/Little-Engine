#ifndef LITTLEVULKANENGINE_SOURCE_MATH_VECTOR3_HPP
#define LITTLEVULKANENGINE_SOURCE_MATH_VECTOR3_HPP


#include <string>


namespace lve {

    class Vector2;

    class Vector3 {
    public:
        Vector3();

        explicit Vector3(float n);

        Vector3(float x, float y, float z);

        Vector3(const Vector3& v);

        explicit Vector3(const Vector2& v);

        Vector3& set(float x_, float y_, float z_);

        float operator[](unsigned int i);

        float operator[](unsigned int i) const;

        Vector3& operator=(const Vector3& v);

        Vector3 operator+(float n) const;

        Vector3 operator+(const Vector3& v) const;

        Vector3 operator-(float n) const;

        Vector3 operator-(const Vector3& v) const;

        Vector3 operator*(float n) const;

        float operator*(const Vector3& v) const;

        Vector3 operator/(float n) const;

        Vector3& operator+=(float n);

        Vector3& operator+=(const Vector3& v);

        Vector3& operator-=(float n);

        Vector3& operator-=(const Vector3& v);

        Vector3& operator*=(float n);

        Vector3& operator/=(float n);

        bool operator==(const Vector3& v) const;

        bool operator!=(const Vector3& v) const;

        [[nodiscard]] Vector3 normal() const;

        Vector3& normalize();

        [[nodiscard]] float length() const;

        float* ptr();

        [[nodiscard]] std::string str() const;

    public:
        float x{};
        float y{};
        float z{};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_MATH_VECTOR3_HPP
