#pragma once
#include "raylib.h"

class Vector2D {
public:
    float x;
    float y;

    constexpr Vector2D() noexcept : x(0.0f), y(0.0f) {}
    constexpr Vector2D(float xValue, float yValue) noexcept : x(xValue), y(yValue) {}

    float length() const noexcept;
    Vector2D normalized() const noexcept;
    Vector2 cast() const noexcept;

    Vector2D add(const Vector2D& other) const noexcept;
    Vector2D subtract(const Vector2D& other) const noexcept;
    Vector2D multiply(float scalar) const noexcept;
    Vector2D divide(float scalar) const;
};