#ifndef LITTLEVULKANENGINE_VECTOR4_HPP
#define LITTLEVULKANENGINE_VECTOR4_HPP


#include <string>


namespace lve {

    class Vector4 {
    public:
        Vector4();

        explicit Vector4(float n);

        explicit Vector4(const float p[4]);

        Vector4(float x, float y, float z, float w);

        Vector4(const Vector4& v);

        float& operator[](unsigned int i);

        const float& operator[](unsigned int i) const;

        Vector4& operator=(const Vector4& v);

        Vector4 operator+(float n) const;

        Vector4 operator+(const Vector4& v) const;

        Vector4 operator-(float n) const;

        Vector4 operator-(const Vector4& v) const;

        Vector4 operator*(float n) const;

        float operator*(const Vector4& v) const;

        Vector4 operator/(float n) const;

        Vector4& operator+=(float n);

        Vector4& operator+=(const Vector4& v);

        Vector4& operator-=(float n);

        Vector4& operator-=(const Vector4& v);

        Vector4& operator*=(float n);

        Vector4& operator/=(float n);

        bool operator==(const Vector4& v) const;

        bool operator!=(const Vector4& v) const;

        [[nodiscard]] Vector4 normal() const;

        Vector4& normalize();

        [[nodiscard]] float length() const;

        float* ptr();

        [[nodiscard]] std::string str() const;

    public:
        float x{}, y{}, z{}, w{};
    };


} // namespace lve


#endif //LITTLEVULKANENGINE_VECTOR4_HPP
