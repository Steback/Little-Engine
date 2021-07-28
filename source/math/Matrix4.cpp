#include "Matrix4.hpp"


namespace lve {

    Matrix4::Matrix4() = default;

    Matrix4::Matrix4(float n) {
        rows[0][0] = n;
        rows[1][1] = n;
        rows[2][2] = n;
        rows[3][3] = n;
    }

    Matrix4::Matrix4(const float (*m)[4]) {
        rows[0] = Vector4(m[0]);
        rows[1] = Vector4(m[1]);
        rows[2] = Vector4(m[2]);
        rows[3] = Vector4(m[3]);
    }

    Matrix4::Matrix4(const float *m) {
        rows[0] = Vector4(m[0], m[1], m[2], m[3]);
        rows[1] = Vector4(m[4], m[5], m[6], m[7]);
        rows[2] = Vector4(m[8], m[9], m[10], m[11]);
        rows[3] = Vector4(m[12], m[13], m[14], m[15]);
    }

    Matrix4::Matrix4(const Vector4 *m) {
        rows[0] = m[0];
        rows[1] = m[1];
        rows[2] = m[2];
        rows[3] = m[3];
    }

    Matrix4::Matrix4(const Vector4 &a, const Vector4 &b, const Vector4 &c, const Vector4 &d) {
        rows[0] = a;
        rows[1] = b;
        rows[2] = c;
        rows[3] = d;
    }

    Matrix4::Matrix4(const Matrix4 &m) {
        std::copy(std::begin(m.rows), std::end(m.rows), std::begin(rows));
    }

    Vector4 &Matrix4::operator[](unsigned int i) {
        return rows[i];
    }

    const Vector4 &Matrix4::operator[](unsigned int i) const {
        return rows[i];
    }

    Matrix4 Matrix4::operator+(const Matrix4 &m) {
        return Matrix4(rows[0] + m[0], rows[1] + m[1], rows[2] + m[2], rows[3] + m[3]);
    }

    Matrix4 Matrix4::operator-(const Matrix4 &m) {
        return Matrix4(rows[0] - m[0], rows[1] - m[1], rows[2] - m[2], rows[3] - m[3]);
    }

    Matrix4 Matrix4::operator*(float n) {
        return Matrix4(rows[0] * n, rows[1] * n, rows[2] * n, rows[3] * n);
    }

    Vector4 Matrix4::operator*(const Vector4 &v) {
        return Vector4(rows[0] * v, rows[1] * v, rows[2] * v, rows[3] * v);
    }

    Matrix4 Matrix4::operator*(const Matrix4 &m) {
        Matrix4 result;

        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                result[row][col] = rows[row][0] * m[0][col] + rows[row][1] * m[1][col] + rows[row][2] * m[2][col] + rows[row][3] * m[3][col];
            }
        }

        return result;
    }

    Matrix4 Matrix4::operator/(float n) {
        float f = 1.0f / n;
        return Matrix4(rows[0] * f, rows[1] * f, rows[2] * f, rows[3] * f);
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

    bool Matrix4::operator==(const Matrix4 &m) {
        return rows[0] == m[0] && rows[1] == m[1] && rows[2] == m[2] && rows[3] == m[3];
    }

    bool Matrix4::operator!=(const Matrix4 &m) {
        return !(*this == m);
    }

    float Matrix4::determinant() const {
        return rows[0][0] * (rows[1][1] * (rows[2][2] * rows[3][3] - rows[2][3] * rows[3][2]) + rows[1][2] * (rows[2][3] * rows[3][1] - rows[2][1] * rows[3][3]) + rows[1][3] * (rows[2][1] * rows[3][2] - rows[2][2] * rows[3][1])) +
               rows[0][1] * (rows[1][0] * (rows[2][3] * rows[3][2] - rows[2][2] * rows[3][3]) + rows[1][2] * (rows[2][0] * rows[3][3] - rows[2][3] * rows[3][0]) + rows[1][3] * (rows[2][2] * rows[3][0] - rows[2][0] * rows[3][2])) +
               rows[0][2] * (rows[1][0] * (rows[2][1] * rows[3][3] - rows[2][3] * rows[3][1]) + rows[1][1] * (rows[2][3] * rows[3][0] - rows[2][0] * rows[3][3]) + rows[1][3] * (rows[2][0] * rows[3][1] - rows[2][1] * rows[3][0])) +
               rows[0][3] * (rows[1][0] * (rows[2][2] * rows[3][1] - rows[2][1] * rows[3][2]) + rows[1][1] * (rows[2][0] * rows[3][2] - rows[2][2] * rows[3][0]) + rows[1][2] * (rows[2][1] * rows[3][0] - rows[2][0] * rows[3][1]));
    }

    Matrix4 Matrix4::inverse() {
        float n00 = rows[0][0];
        float n01 = rows[0][1];
        float n02 = rows[0][2];
        float n03 = rows[0][3];

        float n10 = rows[1][0];
        float n11 = rows[1][1];
        float n12 = rows[1][2];
        float n13 = rows[1][3];

        float n20 = rows[2][0];
        float n21 = rows[2][1];
        float n22 = rows[2][2];
        float n23 = rows[2][3];

        float n30 = rows[3][0];
        float n31 = rows[3][1];
        float n32 = rows[3][2];
        float n33 = rows[3][3];

        float p00 = n11 * (n22 * n33 - n23 * n32) + n12 * (n23 * n31 - n21 * n33) + n13 * (n21 * n32 - n22 * n31);
        float p10 = n10 * (n23 * n32 - n22 * n33) + n12 * (n20 * n33 - n23 * n30) + n13 * (n22 * n30 - n20 * n32);
        float p20 = n10 * (n21 * n33 - n23 * n31) + n11 * (n23 * n30 - n20 * n33) + n13 * (n20 * n31 - n21 * n30);
        float p30 = n10 * (n22 * n31 - n21 * n32) + n11 * (n20 * n32 - n22 * n30) + n12 * (n21 * n30 - n20 * n31);

        float t = 1.0F / (n00 * p00 + n01 * p10 + n02 * p20 + n03 * p30);

        return Matrix4({p00 * t,
                       (n01 * (n23 * n32 - n22 * n33) + n02 * (n21 * n33 - n23 * n31) + n03 * (n22 * n31 - n21 * n32)) * t,
                       (n01 * (n12 * n33 - n13 * n32) + n02 * (n13 * n31 - n11 * n33) + n03 * (n11 * n32 - n12 * n31)) * t,
                       (n01 * (n13 * n22 - n12 * n23) + n02 * (n11 * n23 - n13 * n21) + n03 * (n12 * n21 - n11 * n22)) * t},
                       {p10 * t,
                       (n00 * (n22 * n33 - n23 * n32) + n02 * (n23 * n30 - n20 * n33) + n03 * (n20 * n32 - n22 * n30)) * t,
                       (n00 * (n13 * n32 - n12 * n33) + n02 * (n10 * n33 - n13 * n30) + n03 * (n12 * n30 - n10 * n32)) * t,
                       (n00 * (n12 * n23 - n13 * n22) + n02 * (n13 * n20 - n10 * n23) + n03 * (n10 * n22 - n12 * n20)) * t},
                       {p20 * t,
                       (n00 * (n23 * n31 - n21 * n33) + n01 * (n20 * n33 - n23 * n30) + n03 * (n21 * n30 - n20 * n31)) * t,
                       (n00 * (n11 * n33 - n13 * n31) + n01 * (n13 * n30 - n10 * n33) + n03 * (n10 * n31 - n11 * n30)) * t,
                       (n00 * (n13 * n21 - n11 * n23) + n01 * (n10 * n23 - n13 * n20) + n03 * (n11 * n20 - n10 * n21)) * t},
                       {p30 * t,
                       (n00 * (n21 * n32 - n22 * n31) + n01 * (n22 * n30 - n20 * n32) + n02 * (n20 * n31 - n21 * n30)) * t,
                       (n00 * (n12 * n31 - n11 * n32) + n01 * (n10 * n32 - n12 * n30) + n02 * (n11 * n30 - n10 * n31)) * t,
                       (n00 * (n11 * n22 - n12 * n21) + n01 * (n12 * n20 - n10 * n22) + n02 * (n10 * n21 - n11 * n20)) * t});
    }

    Matrix4 Matrix4::transpose() {
        return Matrix4({rows[0][0], rows[1][0], rows[2][0], rows[3][0]},
                       {rows[0][1], rows[1][1], rows[2][1], rows[3][1]},
                       {rows[0][2], rows[1][2], rows[2][2], rows[3][2]},
                       {rows[0][3], rows[1][3], rows[2][3], rows[3][3]});
    }

    std::string Matrix4::str() {
        return rows[0].str() + '\n' + rows[1].str() + '\n' + rows[2].str() + '\n' + rows[3].str() + '\n';
    }

} // namespace lve
