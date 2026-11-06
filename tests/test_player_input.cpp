/* test_player_input.cpp
 *
 * Unit tests for src/model/player_input.hpp. PlayerInput turns a key code into
 * an action name using INPUT_MAP. Model code has no raylib, so these tests
 * never need a window. */
#include "doctest.h"
#include "../src/model/player_input.hpp"

TEST_CASE("PlayerInput maps movement keys to left and right")
{
    CHECK(PlayerInput(int('A')).action == "left");
    CHECK(PlayerInput(263).action == "left");
    CHECK(PlayerInput(int('D')).action == "right");
    CHECK(PlayerInput(262).action == "right");
}

TEST_CASE("PlayerInput maps space, W and up arrow to jump")
{
    CHECK(PlayerInput(int(' ')).action == "jump");
    CHECK(PlayerInput(int('W')).action == "jump");
    CHECK(PlayerInput(265).action == "jump");
}

TEST_CASE("PlayerInput treats an unbound key as doing nothing")
{
    CHECK(PlayerInput(int('Q')).action.empty());
    CHECK(PlayerInput(0).action.empty());
    CHECK(PlayerInput(-1).action.empty());
}
