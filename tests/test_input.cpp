/* test_input.cpp
 *
 * Placeholder for the unit tests of src/controller/input.hpp. Reading the
 * keyboard needs raylib and a window, but the mapping from key state to a
 * PlayerInput does not: if readPlayerInput is split into a raylib call plus a
 * pure mapping function, test the mapping here (right or D moves right, left or
 * A moves left, Space, Up or W jumps). */
#include "doctest.h"

#include "controller/input.hpp"
#include "model/config.hpp"
#include "model/player.hpp"

TEST_SUITE("input") {
    static TileMap makeTestMap() {
        TileMap map(MAP_COLS, MAP_ROWS, TILE_SIZE);
        map.LoadTestMap();
        return map;
    }

    static void landPlayer(Player& player, TileMap& map) {
        for (int frame = 0; frame < 100 && !player.IsOnGround(); frame++) {
            player.UpdatePhysics(map);
        }
    }

    TEST_CASE("ProcessInput moves the player right") {
        TileMap map = makeTestMap();
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));

        ProcessInput(RIGHT_INPUT, player);
        player.UpdatePhysics(map);

        CHECK(player.getPosition().x == doctest::Approx(12.0f));
        CHECK(player.IsFacingRight());
    }

    TEST_CASE("ProcessInput moves the player left") {
        TileMap map = makeTestMap();
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));

        ProcessInput(LEFT_INPUT, player);
        player.UpdatePhysics(map);

        CHECK(player.getPosition().x == doctest::Approx(8.0f));
        CHECK_FALSE(player.IsFacingRight());
    }

    TEST_CASE("ProcessInput jumps a landed player") {
        TileMap map = makeTestMap();
        Player player;
        landPlayer(player, map);
        REQUIRE(player.IsOnGround());

        ProcessInput(JUMP_INPUT, player);

        CHECK_FALSE(player.IsOnGround());
        CHECK(player.getStamina() == doctest::Approx(85.0f));
    }

    TEST_CASE("ProcessInput with no mask stops movement") {
        TileMap map = makeTestMap();
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));
        ProcessInput(RIGHT_INPUT, player);

        ProcessInput(INPUT_NONE, player);
        player.UpdatePhysics(map);

        CHECK(player.getPosition().x == doctest::Approx(10.0f));
    }
}
