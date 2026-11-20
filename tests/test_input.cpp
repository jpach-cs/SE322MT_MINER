/* test_input.cpp
 *
 * Placeholder for the unit tests of src/controller/input.hpp. Reading the
 * keyboard needs raylib and a window, but the mapping from key state to a
 * PlayerInput does not: if readPlayerInput is split into a raylib call plus a
 * pure mapping function, test the mapping here (right or D moves right, left or
 * A moves left, Space, Up or W jumps). */
#include "doctest.h"

#include "../src/controller/input.hpp"
#include "../src/model/player.hpp"

TEST_SUITE("input") {
    TEST_CASE("ProcessInput moves the player right") {
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));

        ProcessInput(RIGHT_INPUT, player);
        player.UpdatePhysics();

        CHECK(player.getPosition().x == doctest::Approx(12.0f));
        CHECK(player.IsFacingRight());
    }

    TEST_CASE("ProcessInput moves the player left") {
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));

        ProcessInput(LEFT_INPUT, player);
        player.UpdatePhysics();

        CHECK(player.getPosition().x == doctest::Approx(8.0f));
        CHECK_FALSE(player.IsFacingRight());
    }

    TEST_CASE("ProcessInput jumps a landed player") {
        Player player;
        player.UpdatePhysics();
        REQUIRE(player.IsOnGround());

        ProcessInput(JUMP_INPUT, player);

        CHECK_FALSE(player.IsOnGround());
        CHECK(player.getStamina() == doctest::Approx(85.0f));
    }

    TEST_CASE("ProcessInput with no mask stops movement") {
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));
        ProcessInput(RIGHT_INPUT, player);

        ProcessInput(INPUT_NONE, player);
        player.UpdatePhysics();

        CHECK(player.getPosition().x == doctest::Approx(10.0f));
    }
}
