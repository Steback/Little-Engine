#ifndef LITTLEVULKANENGINE_MATRIX4_HPP
#define LITTLEVULKANENGINE_MATRIX4_HPP


#include <string>

#include "Vector4.hpp"


namespace lve {

    class Matrix3;

    class Matrix4 {
    public:
        Matrix4();

        explicit Matrix4(float n);

        Matrix4(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d);

        explicit Matrix4(const Vector4 m[4]);

        explicit Matrix4(const float m[16]);

        explicit Matrix4(const Matrix3& m);

        Matrix4(const Matrix4& m);

        Matrix4& operator=(const Matrix4& m);

        Vector4& operator[](size_t row);

        const Vector4& operator[](size_t row) const;

        Matrix4 operator+(const Matrix4& m) const;

        Matrix4 operator-(const Matrix4& m) const;

        Matrix4 operator*(float n) const;

        Vector4 operator*(const Vector4& v) const;

        Matrix4 operator*(const Matrix4& m) const;

        Matrix4 operator/(float n) const;

        Matrix4& operator+=(const Matrix4& m);

        Matrix4& operator-=(const Matrix4& m);

        Matrix4& operator*=(float n);

        Matrix4& operator*=(const Matrix4& m);

        Matrix4& operator/=(float n);

        bool operator==(const Matrix4& m) const;

        bool operator!=(const Matrix4& m) const;

        [[nodiscard]] Matrix3 getSubMatrix(size_t row, size_t col) const;

        [[nodiscard]] float determinant() const;

        [[nodiscard]] Matrix4 adjugate() const;

        [[nodiscard]] Matrix4 inverse() const;

        [[nodiscard]] Matrix4 transpose() const;

        [[nodiscard]] std::string str(bool newLine = true) const;

    private:
        Vector4 rows[4];
    };

    inline Matrix4 operator*(float n, const Matrix4& m) {
        return m * n;
    }

    inline Matrix4 operator/(float n, const Matrix4& m) {
        return m / n;
    }

    using mat4 = Matrix4;

} // namespace lve


#endif //LITTLEVULKANENGINE_MATRIX4_HPP
