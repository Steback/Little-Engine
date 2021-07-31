#ifndef LITTLEVULKANENGINE_MATRIX2_HPP
#define LITTLEVULKANENGINE_MATRIX2_HPP


#include "Vector2.hpp"


namespace lve {

    class Matrix2 {
    public:
        Matrix2();

        explicit Matrix2(float n);

        Matrix2(const Vector2& a, const Vector2& b);

        explicit Matrix2(const Vector2 m[2]);

        explicit Matrix2(const float m[4]);

        Matrix2(const Matrix2& m);

        Matrix2& operator=(const Matrix2& m);

        Vector2& operator[](size_t row);

        const Vector2& operator[](size_t row) const;

        Matrix2 operator+(const Matrix2& m) const;

        Matrix2 operator-(const Matrix2& m) const;

        Matrix2 operator*(float n) const;

        Vector2 operator*(const Vector2& v) const;

        Matrix2 operator*(const Matrix2& m) const;

        Matrix2 operator/(float n) const;

        Matrix2& operator+=(const Matrix2& m);

        Matrix2& operator-=(const Matrix2& m);

        Matrix2& operator*=(float n);

        Matrix2& operator*=(const Matrix2& m);

        Matrix2& operator/=(float n);

        bool operator==(const Matrix2& m);

        bool operator!=(const Matrix2& m);

        [[nodiscard]] float determinant() const;

        [[nodiscard]] Matrix2 adjugate() const;

        [[nodiscard]] Matrix2 inverse() const;

        [[nodiscard]] Matrix2 transpose() const;

        [[nodiscard]] std::string str(bool newLine = true) const;

    private:
        Vector2 rows[2];
    };

    inline Matrix2 operator*(float n, const Matrix2& m) {
        return m * n;
    }

    inline Matrix2 operator/(float n, const Matrix2& m) {
        return m * n;
    }

    using mat2 = Matrix2;

} // namespace lve


#endif //LITTLEVULKANENGINE_MATRIX2_HPP
