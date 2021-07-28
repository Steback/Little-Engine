#include "Vector4.hpp"

#include <cmath>


namespace lve {

    Vector4::Vector4() = default;

    Vector4::Vector4(float n) : x(n), y(n), z(n), w(n) {

    }

    Vector4::Vector4(const float *p) : x(p[0]), y(p[1]), z(p[2]), w(p[3]) {

    }

    Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {

    }

    Vector4::Vector4(const Vector4 &v)  = default;

    float &Vector4::operator[](unsigned int i) {
        return ((&x)[i]);
    }

    const float &Vector4::operator[](unsigned int i) const {
        return ((&x)[i]);
    }

    Vector4 &Vector4::operator=(const Vector4 &v) = default;

    Vector4 Vector4::operator+(float n) const {
        return Vector4(x + n, y + n, z + n, w + n);
    }

    Vector4 Vector4::operator+(const Vector4 &v) const {
        return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    Vector4 Vector4::operator-(float n) const {
        return Vector4(x - n, y - n, z - n, w - n);
    }

    Vector4 Vector4::operator-(const Vector4 &v) const {
        return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    Vector4 Vector4::operator*(float n) const {
        return Vector4(x * n, y * n, z * n, w * n);
    }

    float Vector4::operator*(const Vector4 &v) const {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }

    Vector4 Vector4::operator/(float n) const {
        float f = 1.0f / n;
        return Vector4(x * f, y * f, z * f, w * f);
    }

    Vector4 &Vector4::operator+=(float n) {
        return *this = *this + n;
    }

    Vector4 &Vector4::operator+=(const Vector4 &v) {
        return *this = *this + v;
    }

    Vector4 &Vector4::operator-=(float n) {
        return *this = *this - n;
    }

    Vector4 &Vector4::operator-=(const Vector4 &v) {
        return *this = *this - v;
    }

    Vector4 &Vector4::operator*=(float n) {
        return *this = *this * n;
    }

    Vector4 &Vector4::operator/=(float n) {
        return *this = *this / n;
    }

    bool Vector4::operator==(const Vector4 &v) const {
        return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w);
    }

    bool Vector4::operator!=(const Vector4 &v) const {
        return !(*this == v);
    }

    Vector4 Vector4::normal() const {
        return *this / length();
    }

    Vector4 &Vector4::normalize() {
        return *this = normal();
    }

    float Vector4::length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    float *Vector4::ptr() {
        return (&x);
    }

    std::string Vector4::str() const {
        return "| " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " " + std::to_string(w) + " |";
    }

} // namespace lve
