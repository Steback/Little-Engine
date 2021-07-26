#ifndef LITTLEVULKANENGINE_MATRIX2_HPP
#define LITTLEVULKANENGINE_MATRIX2_HPP


#include <string>

#include "Vector2.hpp"


namespace lve {

    class Matrix2 {
    public:
        explicit Matrix2(float diagonal = 1.0f);

        Matrix2(const Matrix2& other);

        explicit Matrix2(const float source[4]);

        explicit Matrix2(const Vector2 source[2]);

        Matrix2(const Vector2& i, const Vector2& j);

        Vector2& operator[](uint32_t index);

        const Vector2& operator[](uint32_t index) const;

        Matrix2 operator+(float scalar) const;

        Matrix2 operator+(const Matrix2& other) const;

        Matrix2 operator-(float scalar) const;

        Matrix2 operator-(const Matrix2& other) const;

        Matrix2 operator*(float scalar) const;

        Vector2 operator*(const Vector2& scalar) const;

        Matrix2 operator*(const Matrix2& other) const;

        Matrix2 operator/(float scalar) const;

        Matrix2& operator+=(float scalar);

        Matrix2& operator+=(const Matrix2& other);

        Matrix2& operator-=(float scalar);

        Matrix2& operator-=(const Matrix2& other);

        Matrix2& operator*=(float scalar);

        Matrix2& operator*=(const Matrix2& other);

        Matrix2& operator/=(float scalar);

        bool operator==(const Matrix2& other);

        bool operator!=(const Matrix2& other);

        float determinant();

        Matrix2 adjoint();

        Matrix2 inverse();

        Matrix2 transpose();

        std::string str();

    private:
        Vector2 rows[2];
    };

    inline Matrix2 operator*(float scalar, const Matrix2& matrix)  {
        return matrix * scalar;
    }

    inline Vector2 operator*(const Vector2& scalar, const Matrix2& matrix)  {
        return matrix * scalar;
    }

    using mat2 = Matrix2;

} // namespace lve


#endif //LITTLEVULKANENGINE_MATRIX2_HPP
