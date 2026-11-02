/* test_config.cpp
 *
 * Unit tests for src/model/config.hpp. Checks the constants match the spec
 * (max stamina 100, jump cost 15, the 0.75 and 0.5 penalties) and that derived
 * sizes agree with the screen and tile sizes. Model code has no raylib, so
 * these tests never need a window. */
#include "doctest.h"
#include "../src/model/config.hpp"

TEST_CASE("map dimensions agree with the screen and tile sizes")
{
    CHECK(MAP_COLS * TILE_SIZE == SCREEN_W);
    CHECK(MAP_ROWS * TILE_SIZE == SCREEN_H);
    CHECK(PLAYER_WIDTH == TILE_SIZE);
    CHECK(PLAYER_HEIGHT == 3 * TILE_SIZE);
}

TEST_CASE("stamina constants match the spec")
{
    CHECK(PLAYER_MAX_STAMINA == doctest::Approx(100.0f));
    CHECK(JUMP_STAMINA_COST == doctest::Approx(15.0f));
    CHECK(STAMINA_FROM_FOOD == doctest::Approx(40.0f));
    CHECK(SPEED_PENALTY == doctest::Approx(0.75f));
    CHECK(JUMP_PENALTY == doctest::Approx(0.5f));
}

TEST_CASE("every input binding names a known action")
{
    for (const auto &binding : INPUT_MAP)
    {
        const std::string &action = binding.second;
        CHECK((action == "left" || action == "right" || action == "jump"));
    }
}
