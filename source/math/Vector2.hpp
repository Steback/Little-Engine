#ifndef LITTLEVULKANENGINE_VECTOR2_HPP
#define LITTLEVULKANENGINE_VECTOR2_HPP


#include <string>


namespace lve {

    class Vector2 {
    public:
        Vector2();

        explicit Vector2(float n);

        Vector2(float x, float y);

        explicit Vector2(const float p[2]);

        Vector2(const Vector2& v);

        Vector2& operator=(const Vector2& v);

        float& operator[](size_t index);

        const float& operator[](size_t index) const;

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

        [[nodiscard]] float length() const;

        [[nodiscard]] Vector2 normal() const;

        Vector2& normalize();

        float* ptr();

        [[nodiscard]] std::string str() const;

    public:
        float x{}, y{};
    };

    inline Vector2 operator+(float n, const Vector2& v) {
        return  v + n;
    }

    inline Vector2 operator-(float n, const Vector2& v) {
        return  v - n;
    }

    inline Vector2 operator*(float n, const Vector2& v) {
        return  v * n;
    }

    using vec2 = Vector2;

} // namespace lve


#endif //LITTLEVULKANENGINE_VECTOR2_HPP
