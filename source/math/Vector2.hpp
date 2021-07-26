#ifndef LITTLEVULKANENGINE_SOURCE_MATH_VECTOR2_HPP
#define LITTLEVULKANENGINE_SOURCE_MATH_VECTOR2_HPP


#include <string>


namespace lve {

    class Vector2 {
    public:
        Vector2();

        explicit Vector2(float n);

        Vector2(float x, float y);

        Vector2(const Vector2& v);

        explicit Vector2(const float source[2]);

        Vector2& set(float x_, float y_);

        float& operator[](unsigned int i);

        const float& operator[](unsigned int i) const;

        Vector2& operator=(const Vector2& v);

        Vector2 operator+(float n) const;

        Vector2 operator+(const Vector2& v) const;

        Vector2 operator-(float n) const;

        Vector2 operator-(const Vector2& v) const;

        Vector2 operator*(float n) const;

        float operator*(const Vector2& v) const;

        Vector2 operator/(float n) const;

        Vector2& operator+=(float n);

        Vector2& operator+=(const Vector2& v);

        Vector2& operator-=(float n);

        Vector2& operator-=(const Vector2& v);

        Vector2& operator*=(float n);

        Vector2& operator/=(float n);

        bool operator==(const Vector2& v) const;

        bool operator!=(const Vector2& v) const;

        [[nodiscard]] Vector2 normal() const;

        Vector2& normalize();

        [[nodiscard]] float length() const;

        float* ptr();

        [[nodiscard]] std::string str() const;

    public:
        float x{};
        float y{};
    };

    using vec2 = Vector2;

} // namespace lve

inline lve::vec2 operator*(float n, const lve::vec2& v) {
    return v * n;
}


#endif //LITTLEVULKANENGINE_SOURCE_MATH_VECTOR2_HPP
