#include "doctest.h"

#include "../src/model/stamina.hpp"

TEST_SUITE("Stamina") {
    TEST_CASE("default construction starts at maximum stamina") {
        const Stamina stamina;

        CHECK(stamina.getStamina() == doctest::Approx(100.0f));
    }

    TEST_CASE("construction accepts an initial stamina value") {
        const Stamina stamina(42.5f);

        CHECK(stamina.getStamina() == doctest::Approx(42.5f));
    }

    TEST_CASE("stamina can be decreased") {
        Stamina stamina(50.0f);

        stamina.updatePlayerStamina(-15.0f);

        CHECK(stamina.getStamina() == doctest::Approx(35.0f));
    }

    TEST_CASE("stamina can be increased") {
        Stamina stamina(50.0f);

        stamina.updatePlayerStamina(20.0f);

        CHECK(stamina.getStamina() == doctest::Approx(70.0f));
    }

    TEST_CASE("stamina is capped at its maximum") {
        Stamina stamina(90.0f);

        stamina.updatePlayerStamina(20.0f);

        CHECK(stamina.getStamina() == doctest::Approx(100.0f));
    }

    TEST_CASE("stamina can be decreased to zero") {
        Stamina stamina(10.0f);

        stamina.updatePlayerStamina(-10.0f);

        CHECK(stamina.getStamina() == doctest::Approx(0.0f));
    }

    TEST_CASE("a decrease larger than current stamina is ignored") {
        Stamina stamina(10.0f);

        stamina.updatePlayerStamina(-15.0f);

        CHECK(stamina.getStamina() == doctest::Approx(10.0f));
    }

    TEST_CASE("zero change leaves stamina unchanged") {
        Stamina stamina(42.0f);

        stamina.updatePlayerStamina(0.0f);

        CHECK(stamina.getStamina() == doctest::Approx(42.0f));
    }
}
