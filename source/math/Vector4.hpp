#ifndef LITTLEVULKANENGINE_VECTOR4_HPP
#define LITTLEVULKANENGINE_VECTOR4_HPP


#include <string>


namespace lve {

    class Vector4 {
    public:
        Vector4();

        explicit Vector4(float n);

        Vector4(float x, float y, float z, float w);

        explicit Vector4(const float p[4]);

        Vector4(const Vector4& v);

        Vector4& operator=(const Vector4& v);

        float& operator[](size_t index);

        const float& operator[](size_t index) const;

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

        [[nodiscard]] float length() const;

        [[nodiscard]] Vector4 normal() const;

        Vector4& normalize();

        float* ptr();

        [[nodiscard]] std::string str() const;

    public:
        float x{}, y{}, z{}, w{};
    };

    inline Vector4 operator+(float n,  const Vector4& v) {
        return v + n;
    }

    inline Vector4 operator-(float n,  const Vector4& v) {
        return v - n;
    }

    inline Vector4 operator*(float n,  const Vector4& v) {
        return v * n;
    }

    using vec4 = Vector4;

} // namespace lve


#endif //LITTLEVULKANENGINE_VECTOR4_HPP
