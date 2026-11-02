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

    TEST_CASE("cast copies both components to raylib Vector2") {
        const Vector2 converted = Vector2D(1.5f, -2.5f).cast();

        CHECK(converted.x == doctest::Approx(1.5f));
        CHECK(converted.y == doctest::Approx(-2.5f));
    }
}
