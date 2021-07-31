#ifndef LITTLEVULKANENGINE_MATRIX3_HPP
#define LITTLEVULKANENGINE_MATRIX3_HPP


#include "Vector3.hpp"


namespace lve {

    class Matrix2;

    class Matrix3 {
    public:
        Matrix3();

        explicit Matrix3(float n);

        Matrix3(const Vector3& a, const Vector3& b, const Vector3& c);

        explicit Matrix3(const Vector3 m[3]);

        explicit Matrix3(const float m[9]);

        Matrix3(const Matrix3& m);

        Matrix3& operator=(const Matrix3& m);

        Vector3& operator[](size_t row);

        const Vector3& operator[](size_t row) const;

        Matrix3 operator+(const Matrix3& m) const;

        Matrix3 operator-(const Matrix3& m) const;

        Matrix3 operator*(float n) const;

        Vector3 operator*(const Vector3& v) const;

        Matrix3 operator*(const Matrix3& m) const;

        Matrix3 operator/(float n) const;

        Matrix3& operator+=(const Matrix3& m);

        Matrix3& operator-=(const Matrix3& m);

        Matrix3& operator*=(float n);

        Matrix3& operator*=(const Matrix3& m);

        Matrix3& operator/=(float n);

        bool operator==(const Matrix3& m);

        bool operator!=(const Matrix3& m);

        [[nodiscard]] Matrix2 getSubMatrix(size_t row, size_t col) const;

        [[nodiscard]] float determinant() const;

        [[nodiscard]] Matrix3 adjugate() const;

        [[nodiscard]] Matrix3 inverse() const;

        [[nodiscard]] Matrix3 transpose() const;

        [[nodiscard]] std::string str(bool newLine = true) const;

    private:
        Vector3 rows[3];
    };

    inline Matrix3 operator*(float n, const Matrix3& m) {
        return m * n;
    }

    inline Matrix3 operator/(float n, const Matrix3& m) {
        return m * n;
    }

    using mat3 = Matrix3;

} // namespace lve


#endif //LITTLEVULKANENGINE_MATRIX3_HPP
