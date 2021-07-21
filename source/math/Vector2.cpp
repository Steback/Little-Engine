#include "Vector2.hpp"

#include <cmath>


namespace lve {

    Vector2::Vector2() = default;

    Vector2::Vector2(float n) : x(n), y(n) {  }

    Vector2::Vector2(float x, float y) : x(x), y(y) {  }

    Vector2::Vector2(const Vector2 &v) = default;

    Vector2 &Vector2::set(float x_, float y_) {
        x = x_; y = y_;
        return *this;
    }

    float Vector2::operator[](unsigned int i) {
        return ((&x)[i]);
    }

    float Vector2::operator[](unsigned int i) const {
        return ((&x)[i]);
    }

    Vector2 &Vector2::operator=(const Vector2 &v) = default;

    Vector2 Vector2::operator+(float n) const {
        return Vector2(x + n, y + n);
    }

    Vector2 Vector2::operator+(const Vector2 &v) const {
        return Vector2(x + v.x, y + v.y);
    }

    Vector2 Vector2::operator-(float n) const {
        return Vector2(x - n, y - n);
    }

    Vector2 Vector2::operator-(const Vector2 &v) const {
        return Vector2(x - v.x, y - v.y);
    }

    Vector2 Vector2::operator*(float n) const {
        return Vector2(x * n, y * n);
    }

    float Vector2::operator*(const Vector2 &v) const {
        return x * x + y * y;
    }

    Vector2 Vector2::operator/(float n) const {
        float f = 1.0f / n;
        return Vector2(x * f, y * f);
    }

    Vector2 &Vector2::operator+=(float n) {
        return set(x + n, y + n);
    }

    Vector2 &Vector2::operator+=(const Vector2 &v) {
        return set(x + v.x, y + v.y);
    }

    Vector2 &Vector2::operator-=(float n) {
        return set(x - n, y - n);
    }

    Vector2 &Vector2::operator-=(const Vector2 &v) {
        return set(x - v.x, y - v.y);
    }

    Vector2 &Vector2::operator*=(float n) {
        return set(x * n, y * n);
    }

    Vector2 &Vector2::operator/=(float n) {
        float f = 1.0f / n;
        return set(x * f, y * f);
    }

    bool Vector2::operator==(const Vector2 &v) const {
        return (x == x) && (y == y);
    }

    bool Vector2::operator!=(const Vector2 &v) const {
        return (x != x) && (y != y);
    }

    Vector2 Vector2::normal() const {
        return Vector2(*this) / length();
    }

    Vector2 &Vector2::normalize() {
        return *this /= length();
    }

    float Vector2::length() const {
        return std::sqrt(x * x + y * y);
    }

    float *Vector2::ptr() {
        return (&x);
    }

    std::string Vector2::str() const {
        return "| " + std::to_string(x) + " " + std::to_string(y) + " |";
    }

} // namespace lve
