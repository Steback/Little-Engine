#include "Vector3.hpp"

#include <cmath>

#include "Vector2.hpp"


namespace lve {

    Vector3::Vector3() = default;

    Vector3::Vector3(float n) : x(n), y(n), z(n) {  }

    Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {  }

    Vector3::Vector3(const Vector3 &v) = default;

    Vector3::Vector3(const float *source) : x(source[0]), y(source[1]), z(source[2]) {  }

    Vector3 &Vector3::set(float x_, float y_, float z_) {
        x = x_; y = y_; z = z_;
        return *this;
    }

    float& Vector3::operator[](unsigned int i) {
        return ((&x)[i]);
    }

    const float& Vector3::operator[](unsigned int i) const {
        return ((&x)[i]);
    }

    Vector3 &Vector3::operator=(const Vector3 &v) = default;

    Vector3 Vector3::operator+(float n) const {
        return Vector3(x + n, y + n, z + n);
    }

    Vector3 Vector3::operator+(const Vector3 &v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3 Vector3::operator-(float n) const {
        return Vector3(x - n, y - n, z - n);
    }

    Vector3 Vector3::operator-(const Vector3 &v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    Vector3 Vector3::operator*(float n) const {
        return Vector3(x * n, y * n, z * n);
    }

    float Vector3::operator*(const Vector3 &v) const {
        return (x * v.x) + (y * v.y) + (z * v.z);
    }

    Vector3 Vector3::operator/(float n) const {
        float f = 1.0f / n;
        return Vector3(x * f, y * f, z * f);
    }

    Vector3 &Vector3::operator+=(float n) {
        return set(x + n, y + n, z + n);
    }

    Vector3 &Vector3::operator+=(const Vector3 &v) {
        return set(x + v.x, y + v.y, z + v.z);
    }

    Vector3 &Vector3::operator-=(float n) {
        return set(x - n, y - n, z - n);
    }

    Vector3 &Vector3::operator-=(const Vector3 &v) {
        return set(x - v.x, y - v.y, z - v.z);
    }

    Vector3 &Vector3::operator*=(float n) {
        return set(x * n, y * n, z * n);
    }

    Vector3 &Vector3::operator/=(float n) {
        float f = 1.0f / n;
        return set(x * f, y * f, z * f);
    }

    bool Vector3::operator==(const Vector3 &v) const {
        return (x == v.x) && (y == v.y) && (z == v.z);
    }

    bool Vector3::operator!=(const Vector3 &v) const {
        return (x != v.x) && (y != v.y) && (z != v.z);
    }

    Vector3 Vector3::normal() const {
        return Vector3(*this) / length();
    }

    Vector3 &Vector3::normalize() {
        return *this /= length();
    }

    float Vector3::length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    float *Vector3::ptr() {
        return (&x);
    }

    std::string Vector3::str() const {
        return "| " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " |";
    }

} // namespace lve
