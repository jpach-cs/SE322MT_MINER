#include "doctest.h"

#include "../src/model/Vector2D.hpp"

#include <stdexcept>

TEST_SUITE("Vector2D") {
    TEST_CASE("constructors initialize both components") {
        const Vector2D zero;
        const Vector2D value(3.0f, -4.0f);

        CHECK(zero.x == doctest::Approx(0.0f));
        CHECK(zero.y == doctest::Approx(0.0f));
        CHECK(value.x == doctest::Approx(3.0f));
        CHECK(value.y == doctest::Approx(-4.0f));
    }

    TEST_CASE("length returns the vector magnitude") {
        CHECK(Vector2D(3.0f, 4.0f).length() == doctest::Approx(5.0f));
        CHECK(Vector2D().length() == doctest::Approx(0.0f));
    }

    TEST_CASE("normalization returns a unit vector") {
        const Vector2D result = Vector2D(3.0f, 4.0f).normalized();

        CHECK(result.x == doctest::Approx(0.6f));
        CHECK(result.y == doctest::Approx(0.8f));
        CHECK(result.length() == doctest::Approx(1.0f));
    }

    TEST_CASE("normalizing a zero vector returns a zero vector") {
        const Vector2D result = Vector2D().normalized();

        CHECK(result.x == doctest::Approx(0.0f));
        CHECK(result.y == doctest::Approx(0.0f));
    }

    TEST_CASE("addition and subtraction operate component-wise") {
        const Vector2D first(5.0f, -2.0f);
        const Vector2D second(-3.0f, 7.0f);
        const Vector2D sum = first.add(second);
        const Vector2D difference = first.subtract(second);

        CHECK(sum.x == doctest::Approx(2.0f));
        CHECK(sum.y == doctest::Approx(5.0f));
        CHECK(difference.x == doctest::Approx(8.0f));
        CHECK(difference.y == doctest::Approx(-9.0f));
    }

    TEST_CASE("dot product combines matching components") {
        const Vector2D first(3.0f, -4.0f);
        const Vector2D second(2.0f, 5.0f);

        CHECK(first.dot(second) == doctest::Approx(3.0f * 2.0f + (-4.0f) * 5.0f));
        CHECK(Vector2D().dot(Vector2D(10.0f, -2.0f)) == doctest::Approx(0.0f));
    }

    TEST_CASE("multiplication and division scale both components") {
        const Vector2D value(6.0f, -8.0f);
        const Vector2D product = value.multiply(0.5f);
        const Vector2D quotient = value.divide(2.0f);

        CHECK(product.x == doctest::Approx(3.0f));
        CHECK(product.y == doctest::Approx(-4.0f));
        CHECK(quotient.x == doctest::Approx(3.0f));
        CHECK(quotient.y == doctest::Approx(-4.0f));
    }

    TEST_CASE("division by zero throws a domain error") {
        CHECK_THROWS_AS(Vector2D(1.0f, 2.0f).divide(0.0f), std::domain_error);
    }

    TEST_CASE("arithmetic operators match the named methods") {
        const Vector2D first(5.0f, -2.0f);
        const Vector2D second(-3.0f, 7.0f);

        CHECK(first + second == first.add(second));
        CHECK(first - second == first.subtract(second));
        CHECK(first * 0.5f == first.multiply(0.5f));
        CHECK(0.5f * first == first.multiply(0.5f));
        CHECK(first / 2.0f == first.divide(2.0f));
        CHECK(-first == Vector2D(-5.0f, 2.0f));
    }

    TEST_CASE("compound assignment operators update in place") {
        Vector2D value(6.0f, -8.0f);

        value += Vector2D(1.0f, 2.0f);
        CHECK(value == Vector2D(7.0f, -6.0f));
        value -= Vector2D(3.0f, 4.0f);
        CHECK(value == Vector2D(4.0f, -10.0f));
        value *= 2.0f;
        CHECK(value == Vector2D(8.0f, -20.0f));
        value /= 4.0f;
        CHECK(value == Vector2D(2.0f, -5.0f));
    }

    TEST_CASE("equality compares both components") {
        CHECK(Vector2D(1.0f, 2.0f) == Vector2D(1.0f, 2.0f));
        CHECK(Vector2D(1.0f, 2.0f) != Vector2D(2.0f, 1.0f));
    }

    TEST_CASE("division operators by zero throw a domain error") {
        Vector2D value(1.0f, 2.0f);

        CHECK_THROWS_AS(value / 0.0f, std::domain_error);
        CHECK_THROWS_AS(value /= 0.0f, std::domain_error);
    }

    TEST_CASE("cast copies both components to raylib Vector2") {
        const Vector2 converted = Vector2D(1.5f, -2.5f).cast();

        CHECK(converted.x == doctest::Approx(1.5f));
        CHECK(converted.y == doctest::Approx(-2.5f));
    }
}
