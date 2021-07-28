#ifndef LITTLEVULKANENGINE_MATRIX4_HPP
#define LITTLEVULKANENGINE_MATRIX4_HPP


#include "Vector4.hpp"


namespace lve {

    class Matrix4 {
    public:
        Matrix4();

        explicit Matrix4(float n);

        explicit Matrix4(const float m[4][4]);

        explicit Matrix4(const float m[16]);

        explicit Matrix4(const Vector4 m[4]);

        Matrix4(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d);

        Matrix4(const Matrix4& m);

        Vector4& operator[](unsigned int i);

        const Vector4& operator[](unsigned int i) const;

        Matrix4 operator+(const Matrix4& m);

        Matrix4 operator-(const Matrix4& m);

        Matrix4 operator*(float n);

        Vector4 operator*(const Vector4& v);

        Matrix4 operator*(const Matrix4& m);

        Matrix4 operator/(float n);

        Matrix4& operator+=(const Matrix4& m);

        Matrix4& operator-=(const Matrix4& m);

        Matrix4& operator*=(float n);

        Matrix4& operator*=(const Matrix4& m);

        Matrix4& operator/=(float n);

        bool operator==(const Matrix4& m);

        bool operator!=(const Matrix4& m);

        [[nodiscard]] float determinant() const;

        Matrix4 inverse();

        Matrix4 transpose();

        std::string str();

    public:
        Vector4 rows[4];
    };

    using mat4 = Matrix4;

} // namespace lve


#endif //LITTLEVULKANENGINE_MATRIX4_HPP
