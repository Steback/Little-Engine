#include "Matrix2.hpp"


namespace lve {

    Matrix2::Matrix2(float diagonal) {
        rows[0][0] = diagonal;
        rows[1][1] = diagonal;
    }

    Matrix2::Matrix2(const Matrix2 &other) {
        rows[0] = other[0];
        rows[1] = other[1];
    }

    Matrix2::Matrix2(const float *source) {
        rows[0] = Vector2(source);
    }

    Matrix2::Matrix2(const Vector2 *source) {
        rows[0] = source[0];
        rows[1] = source[1];
    }

    Matrix2::Matrix2(const Vector2& i, const Vector2& j) {
        rows[0] = i;
        rows[1] = j;
    }

    Vector2 &Matrix2::operator[](uint32_t index) {
        return rows[index];
    }

    const Vector2 &Matrix2::operator[](uint32_t index) const {
        return rows[index];
    }

    Matrix2 Matrix2::operator+(float scalar) const {
        return Matrix2(rows[0] + scalar, rows[1] + scalar);
    }

    Matrix2 Matrix2::operator+(const Matrix2 &other) const {
        return Matrix2(rows[0] + other[0], rows[1] + other[1]);
    }

    Matrix2 Matrix2::operator-(float scalar) const {
        return Matrix2(rows[0] - scalar, rows[1] - scalar);
    }

    Matrix2 Matrix2::operator-(const Matrix2 &other) const {
        return Matrix2(rows[0] - other[0], rows[1] - other[1]);
    }

    Matrix2 Matrix2::operator*(float scalar) const {
        return Matrix2(rows[0] * scalar, rows[1] * scalar);
    }

    Vector2 Matrix2::operator*(const Vector2 &scalar) const {
        return Vector2(rows[0] * scalar, rows[1] * scalar);
    }

    Matrix2 Matrix2::operator*(const Matrix2 &other) const {
        Matrix2 result;

        for (uint32_t row = 0; row < 2; ++row) {
            for (uint32_t col = 0; col < 2; ++col) {
                result[row][col] = rows[0][col] * other[row][0] + rows[1][col] * other[row][1];
            }
        }

        return result;
    }

    Matrix2 Matrix2::operator/(float scalar) const {
        return Matrix2(rows[0] / scalar, rows[1] / scalar);
    }

    Matrix2 &Matrix2::operator+=(float scalar) {
        return *this = *this + scalar;
    }

    Matrix2 &Matrix2::operator+=(const Matrix2 &other) {
        return *this = *this + other;
    }

    Matrix2 &Matrix2::operator-=(float scalar) {
        return *this = *this - scalar;
    }

    Matrix2 &Matrix2::operator-=(const Matrix2 &other) {
        return *this = *this - other;
    }

    Matrix2 &Matrix2::operator*=(float scalar) {
        return *this = *this * scalar;
    }

    Matrix2 &Matrix2::operator*=(const Matrix2 &other) {
        return *this = *this * other;
    }

    Matrix2 &Matrix2::operator/=(float scalar) {
        return *this = *this / scalar;
    }

    bool Matrix2::operator==(const Matrix2 &other) {
        return rows[0] == other[0] && rows[1] == other[1];
    }

    bool Matrix2::operator!=(const Matrix2 &other) {
        return rows[0] != other[0] && rows[1] != other[1];
    }

    float Matrix2::determinant() {
        return rows[0][0] * rows[1][1] - rows[0][1] * rows[1][0];
    }

    Matrix2 Matrix2::adjoint() {
        return Matrix2({rows[1][1], -rows[0][1]}, {-rows[1][0], rows[0][0]});
    }

    Matrix2 Matrix2::inverse() {
        return (1.0f / determinant()) * adjoint();
    }

    Matrix2 Matrix2::transpose() {
        return Matrix2({rows[0][0], rows[1][0]}, {rows[0][1], rows[1][1]});
    }

    std::string Matrix2::str() {
        return rows[0].str() + '\n' + rows[1].str() + '\n';
    }

} // namespace lve
