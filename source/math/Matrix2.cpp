#include "Matrix2.hpp"

#include "utils/Macros.hpp"


#define NEW_LINE(newLine) (newLine ? '\n' : ' ')


namespace lve {

    Matrix2::Matrix2() = default;

    Matrix2::Matrix2(float n) {
        rows[0][0] = n;
        rows[1][1] = n;
    }

    Matrix2::Matrix2(const Vector2 &a, const Vector2 &b) {
        rows[0] = a;
        rows[1] = b;
    }

    Matrix2::Matrix2(const Vector2 *m) {
        rows[0] = m[0];
        rows[1] = m[1];
    }

    Matrix2::Matrix2(const float *m) {
        rows[0] = {m[0], m[1]};
        rows[0] = {m[2], m[3]};
    }

    Matrix2::Matrix2(const Matrix2 &m) {
        rows[0] = m[0];
        rows[1] = m[1];
    }

    Matrix2 &Matrix2::operator=(const Matrix2 &m) {
        rows[0] = m[0];
        rows[1] = m[1];
        return *this;
    }

    Vector2 &Matrix2::operator[](size_t row) {
        return rows[row];
    }

    const Vector2 &Matrix2::operator[](size_t row) const {
        return rows[row];
    }

    Matrix2 Matrix2::operator+(const Matrix2 &m) const {
        return {rows[0] + m[0], rows[1] + m[1]};
    }

    Matrix2 Matrix2::operator-(const Matrix2 &m) const {
        return {rows[0] - m[0], rows[1] - m[1]};
    }

    Matrix2 Matrix2::operator*(float n) const {
        return {rows[0] * n, rows[1] * n};
    }

    Vector2 Matrix2::operator*(const Vector2 &v) const {
        return {rows[0] * v, rows[1] * v};
    }

    Matrix2 Matrix2::operator*(const Matrix2 &m) const {
        return {
            { rows[0][0] * m[0][0] + rows[0][1] * m[1][0], rows[0][0] * m[0][1] + rows[0][1] * m[1][1] },
            { rows[1][0] * m[0][0] + rows[1][1] * m[1][0], rows[1][0] * m[0][0] + rows[1][1] * m[1][0] }
        };
    }

    Matrix2 Matrix2::operator/(float n) const {
        float f = 1.0f / n;
        return *this * f;
    }

    Matrix2 &Matrix2::operator+=(const Matrix2 &m) {
        return *this = *this + m;
    }

    Matrix2 &Matrix2::operator-=(const Matrix2 &m) {
        return *this = *this - m;
    }

    Matrix2 &Matrix2::operator*=(float n) {
        return *this = *this * n;
    }

    Matrix2 &Matrix2::operator*=(const Matrix2 &m) {
        return *this = *this * m;
    }

    Matrix2 &Matrix2::operator/=(float n) {
        return *this = *this / n;
    }

    bool Matrix2::operator==(const Matrix2 &m) {
        return rows[0] == m[0] && rows[1] == m[1];
    }

    bool Matrix2::operator!=(const Matrix2 &m) {
        return !(*this == m);
    }

    float Matrix2::determinant() const {
        return rows[0][0] * rows[1][1] - rows[0][1] * rows[1][0];
    }

    Matrix2 Matrix2::adjugate() const {
        return {{rows[1][1], rows[0][0]}, {-rows[0][1], -rows[1][0]}};
    }

    Matrix2 Matrix2::inverse() const {
        return adjugate() / determinant();
    }

    Matrix2 Matrix2::transpose() const {
        return {{rows[0][0], rows[1][0]}, {rows[0][1], rows[1][1]}};
    }

    std::string Matrix2::str(bool newLine) const {
        return rows[0].str() + NEW_LINE(newLine) + rows[1].str() + NEW_LINE(newLine);
    }

} // namespace lv