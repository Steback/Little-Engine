#include "Matrix4.hpp"

#include "Matrix3.hpp"


#define NEW_LINE(newLine) (newLine ? '\n' : ' ')


namespace lve {

    Matrix4::Matrix4() = default;

    Matrix4::Matrix4(float n) {
        rows[0][0] = n;
        rows[1][1] = n;
        rows[2][2] = n;
        rows[3][3] = n;
    }

    Matrix4::Matrix4(const Vector4 &a, const Vector4 &b, const Vector4 &c, const Vector4& d) {
        rows[0] = a;
        rows[1] = b;
        rows[2] = c;
        rows[3] = d;
    }

    Matrix4::Matrix4(const Vector4 *m) {
        rows[0] = m[0];
        rows[1] = m[1];
        rows[2] = m[2];
        rows[3] = m[3];
    }

    Matrix4::Matrix4(const float *m) {
        rows[0] = {m[0], m[1], m[2], m[3]};
        rows[1] = {m[4], m[5], m[6], m[7]};
        rows[2] = {m[8], m[9], m[10], m[11]};
        rows[3] = {m[12], m[13], m[14], m[15]};
    }

    Matrix4::Matrix4(const Matrix4 &m) {
        rows[0] = m[0];
        rows[1] = m[1];
        rows[2] = m[2];
        rows[3] = m[3];
    }

    Matrix4 &Matrix4::operator=(const Matrix4 &m) {
        rows[0] = m[0];
        rows[1] = m[1];
        rows[2] = m[2];
        rows[3] = m[3];
        return *this;
    }

    Vector4 &Matrix4::operator[](size_t row) {
        return rows[row];
    }

    const Vector4 &Matrix4::operator[](size_t row) const {
        return rows[row];
    }

    Matrix4 Matrix4::operator+(const Matrix4 &m) const {
        return {rows[0] + m[0], rows[1] + m[1], rows[2] + m[2], rows[3] + m[3]};
    }

    Matrix4 Matrix4::operator-(const Matrix4 &m) const {
        return {rows[0] - m[0], rows[1] - m[1], rows[2] - m[2], rows[3] - m[3]};
    }

    Matrix4 Matrix4::operator*(float n) const {
        return {rows[0] * n, rows[1] * n, rows[2] * n, rows[3] * n};
    }

    Vector4 Matrix4::operator*(const Vector4 &v) const {
        return {rows[0] * v, rows[1] * v, rows[2] * v, rows[3] * v};
    }

    Matrix4 Matrix4::operator*(const Matrix4 &m) const {
        // TODO: I don't understand why GML use this method and how it works...
        const vec4 srcA0 = rows[0];
        const vec4 srcA1 = rows[1];
        const vec4 srcA2 = rows[2];
        const vec4 srcA3 = rows[3];

        const vec4 srcB0 = m[0];
        const vec4 srcB1 = m[1];
        const vec4 srcB2 = m[2];
        const vec4 srcB3 = m[3];

        mat4 Result;
        Result[0] = srcA0 * srcB0[0] + srcA1 * srcB0[1] + srcA2 * srcB0[2] + srcA3 * srcB0[3];
        Result[1] = srcA0 * srcB1[0] + srcA1 * srcB1[1] + srcA2 * srcB1[2] + srcA3 * srcB1[3];
        Result[2] = srcA0 * srcB2[0] + srcA1 * srcB2[1] + srcA2 * srcB2[2] + srcA3 * srcB2[3];
        Result[3] = srcA0 * srcB3[0] + srcA1 * srcB3[1] + srcA2 * srcB3[2] + srcA3 * srcB3[3];

        return Result;
//        return {
//            {
//                rows[0][0] * m[0][0] + rows[0][1] * m[1][0] + rows[0][2] * m[2][0] + rows[0][3] * m[3][0],
//                rows[0][0] * m[0][1] + rows[0][1] * m[1][1] + rows[0][2] * m[2][1] + rows[0][3] * m[3][1],
//                rows[0][0] * m[0][2] + rows[0][1] * m[1][2] + rows[0][2] * m[2][2] + rows[0][3] * m[3][2],
//                rows[0][0] * m[0][3] + rows[0][1] * m[1][3] + rows[0][2] * m[2][3] + rows[0][3] * m[3][3]
//            }, {
//                rows[1][0] * m[0][0] + rows[1][1] * m[1][0] + rows[1][2] * m[2][0] + rows[1][3] * m[3][0],
//                rows[1][0] * m[0][1] + rows[1][1] * m[1][1] + rows[1][2] * m[2][1] + rows[1][3] * m[3][1],
//                rows[1][0] * m[0][2] + rows[1][1] * m[1][2] + rows[1][2] * m[2][2] + rows[1][3] * m[3][2],
//                rows[1][0] * m[0][3] + rows[1][1] * m[1][3] + rows[1][2] * m[2][3] + rows[1][3] * m[3][3]
//            }, {
//                rows[2][0] * m[0][0] + rows[2][1] * m[1][0] + rows[2][2] * m[2][0] + rows[2][3] * m[3][0],
//                rows[2][0] * m[0][1] + rows[2][1] * m[1][1] + rows[2][2] * m[2][1] + rows[2][3] * m[3][1],
//                rows[2][0] * m[0][2] + rows[2][1] * m[1][2] + rows[2][2] * m[2][2] + rows[2][3] * m[3][2],
//                rows[2][0] * m[0][3] + rows[2][1] * m[1][3] + rows[2][2] * m[2][3] + rows[2][3] * m[3][3]
//            }, {
//                rows[3][0] * m[0][0] + rows[3][1] * m[1][0] + rows[3][2] * m[2][0] + rows[3][3] * m[3][0],
//                rows[3][0] * m[0][1] + rows[3][1] * m[1][1] + rows[3][2] * m[2][1] + rows[3][3] * m[3][1],
//                rows[3][0] * m[0][2] + rows[3][1] * m[1][2] + rows[3][2] * m[2][2] + rows[3][3] * m[3][2],
//                rows[3][0] * m[0][3] + rows[3][1] * m[1][3] + rows[3][2] * m[2][3] + rows[3][3] * m[3][3]
//            }
//        };
    }

    Matrix4 Matrix4::operator/(float n) const {
        float f = 1.0f / n;
        return *this * f;
    }

    Matrix4 &Matrix4::operator+=(const Matrix4 &m) {
        return *this = *this + m;
    }

    Matrix4 &Matrix4::operator-=(const Matrix4 &m) {
        return *this = *this - m;
    }

    Matrix4 &Matrix4::operator*=(float n) {
        return *this = *this * n;
    }

    Matrix4 &Matrix4::operator*=(const Matrix4 &m) {
        return *this = *this * m;
    }

    Matrix4 &Matrix4::operator/=(float n) {
        return *this = *this / n;
    }

    bool Matrix4::operator==(const Matrix4 &m) const {
        return rows[0] == m[0] && rows[1] == m[1] && rows[2] == m[2] && rows[3] == m[3];
    }

    bool Matrix4::operator!=(const Matrix4 &m) const {
        return !(*this == m);
    }

    Matrix3 Matrix4::getSubMatrix(size_t row, size_t col) const {
        Matrix3 matrix;
        size_t rowCount = 0;
        size_t colCount;

        for (size_t i = 0; i < 4; ++i) {
            if (i != row) {
                colCount = 0;

                for (size_t j = 0; j < 4; ++j) {
                    if (j != col) {
                        matrix[rowCount][colCount] = (*this)[i][j];
                        colCount++;
                    }
                }

                rowCount++;
            }
        }

        return matrix;
    }

    float Matrix4::determinant() const {
        return rows[0][0] * getSubMatrix(0, 0).determinant() -
                rows[0][1] * getSubMatrix(0, 1).determinant() +
                rows[0][2] * getSubMatrix(0, 2).determinant() -
                rows[0][3] * getSubMatrix(0, 3).determinant();
    }

    Matrix4 Matrix4::adjugate() const {
        return {
            {
                getSubMatrix(0, 0).determinant(),
                getSubMatrix(0, 1).determinant(),
                getSubMatrix(0, 2).determinant(),
                getSubMatrix(0, 3).determinant()
            }, {
                getSubMatrix(1, 0).determinant(),
                getSubMatrix(1, 1).determinant(),
                getSubMatrix(1, 2).determinant(),
                getSubMatrix(1, 3).determinant()
            }, {
                getSubMatrix(2, 0).determinant(),
                getSubMatrix(2, 1).determinant(),
                getSubMatrix(2, 2).determinant(),
                getSubMatrix(2, 3).determinant()
            }, {
                getSubMatrix(3, 0).determinant(),
                getSubMatrix(3, 1).determinant(),
                getSubMatrix(3, 2).determinant(),
                getSubMatrix(3, 3).determinant()
            }
        };
    }

    Matrix4 Matrix4::inverse() const {
        return adjugate() / determinant();
    }

    Matrix4 Matrix4::transpose() const {
        return {
            { rows[0][0], rows[1][0], rows[2][0], rows[3][0] },
            { rows[0][1], rows[1][1], rows[2][1], rows[3][1] },
            { rows[0][2], rows[1][2], rows[2][2], rows[3][2] },
            { rows[0][3], rows[1][3], rows[2][3], rows[3][3] }
        };
    }

    std::string Matrix4::str(bool newLine) const {
        return rows[0].str() + NEW_LINE(newLine) +
                rows[1].str() + NEW_LINE(newLine) +
                rows[2].str() + NEW_LINE(newLine) +
                rows[3].str() + NEW_LINE(newLine);
    }

} // namespace lv