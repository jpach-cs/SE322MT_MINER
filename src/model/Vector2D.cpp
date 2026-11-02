#include "Vector2D.hpp"

#include <cmath>
#include <stdexcept>

float Vector2D::length() const noexcept {
    return std::sqrt(x * x + y * y);
}

Vector2D Vector2D::normalized() const noexcept {
    const float magnitude = length();
    if (magnitude == 0.0f) {
        return {};
    }

    return this->multiply(1.0f / magnitude);
}

Vector2D Vector2D::add(const Vector2D& other) const noexcept {
    return {x + other.x, y + other.y};
}

Vector2D Vector2D::subtract(const Vector2D& other) const noexcept {
    return {x - other.x, y - other.y};
}

Vector2D Vector2D::multiply(float scalar) const noexcept {
    return {x * scalar, y * scalar};
}

Vector2D Vector2D::divide(float scalar) const {
    if (scalar == 0.0f) {
        throw std::domain_error("cannot divide a Vector2D by zero");
    }

    return {x / scalar, y / scalar};
}

Vector2 Vector2D::cast() const noexcept {
    Vector2 result;
    result.x = x;
    result.y = y;
    return result;
}