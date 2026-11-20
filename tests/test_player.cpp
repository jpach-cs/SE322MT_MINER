/* test_player.cpp */
#include "doctest.h"

#include "model/player.hpp"

static const float FLOOR_TOP = 4.0f * TILE_SIZE;

TEST_SUITE("player") {
    static TileMap makeTestMap() {
        TileMap map(MAP_COLS, MAP_ROWS, TILE_SIZE);
        map.LoadTestMap();
        return map;
    }

    static void landPlayer(Player& player, const TileMap& map) {
        for (int frame = 0; frame < 100 && !player.IsOnGround(); frame++) {
            player.UpdatePhysics(map);
        }
    }

    TEST_CASE("default constructor sets the starting state") {
        const Player player;

        CHECK(player.getPosition() == Vector2D(50.0f, GROUND_Y + PLAYER_HEIGHT));
        CHECK(player.IsFacingRight());
        CHECK_FALSE(player.IsOnGround());
    }

    TEST_CASE("stamina constructor sets the same starting state") {
        const Player player(42.0f);

        CHECK(player.getPosition() == Vector2D(50.0f, GROUND_Y + PLAYER_HEIGHT));
        CHECK(player.IsFacingRight());
        CHECK_FALSE(player.IsOnGround());
    }

    TEST_CASE("SetPosition replaces the position") {
        Player player;

        player.SetPosition(Vector2D(12.5f, -3.0f));

        CHECK(player.getPosition() == Vector2D(12.5f, -3.0f));
    }

    TEST_CASE("gravity is applied once per frame in the air") {
        const TileMap map = makeTestMap();
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));

        player.UpdatePhysics(map);
        CHECK(player.getPosition().x == doctest::Approx(10.0f));
        CHECK(player.getPosition().y == doctest::Approx(100.0f + GRAVITY));
        CHECK_FALSE(player.IsOnGround());

        player.UpdatePhysics(map);
        CHECK(player.getPosition().y == doctest::Approx(100.0f + 3.0f * GRAVITY));
    }

    TEST_CASE("falling speed is clamped to MAX_FALL") {
        const TileMap map = makeTestMap();
        Player player;
        player.SetPosition(Vector2D(0.0f, -1000.0f));

        for (int frame = 0; frame < 30; frame++) {
            player.UpdatePhysics(map);
        }
        const float before = player.getPosition().y;
        player.UpdatePhysics(map);

        CHECK(player.getPosition().y - before == doctest::Approx(MAX_FALL));
        CHECK_FALSE(player.IsOnGround());
    }

    TEST_CASE("player lands on solid floor tiles") {
        const TileMap map = makeTestMap();
        Player player;

        landPlayer(player, map);

        CHECK(player.getPosition().y == doctest::Approx(FLOOR_TOP));
        CHECK(player.IsOnGround());
    }

    TEST_CASE("landed player stays on the floor") {
        const TileMap map = makeTestMap();
        Player player;

        landPlayer(player, map);
        for (int i = 0; i < 10; i++) {
            player.UpdatePhysics(map);
        }

        CHECK(player.getPosition() == Vector2D(50.0f, FLOOR_TOP));
        CHECK(player.IsOnGround());
    }

    TEST_CASE("Move moves right and updates facing direction") {
        const TileMap map = makeTestMap();
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));

        player.Move(false, true, false);
        player.UpdatePhysics(map);

        CHECK(player.getPosition().x == doctest::Approx(12.0f));
        CHECK(player.IsFacingRight());
    }

    TEST_CASE("Move moves left and updates facing direction") {
        const TileMap map = makeTestMap();
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));

        player.Move(true, false, false);
        player.UpdatePhysics(map);

        CHECK(player.getPosition().x == doctest::Approx(8.0f));
        CHECK_FALSE(player.IsFacingRight());
    }

    TEST_CASE("Move with no direction stops horizontal movement") {
        const TileMap map = makeTestMap();
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));
        player.Move(false, true, false);

        player.Move(false, false, false);
        player.UpdatePhysics(map);

        CHECK(player.getPosition().x == doctest::Approx(10.0f));
    }

    TEST_CASE("Move without arguments stops horizontal movement") {
        const TileMap map = makeTestMap();
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));
        player.Move(false, true, false);

        player.Move();
        player.UpdatePhysics(map);

        CHECK(player.getPosition().x == doctest::Approx(10.0f));
    }

    TEST_CASE("Move prioritizes neither direction when both are pressed") {
        const TileMap map = makeTestMap();
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));

        player.Move(true, true, false);
        player.UpdatePhysics(map);

        CHECK(player.getPosition().x == doctest::Approx(10.0f));
        CHECK(player.IsFacingRight());
    }

    TEST_CASE("Jump takes a landed player off the ground") {
        const TileMap map = makeTestMap();
        Player player;
        landPlayer(player, map);

        player.Move(false, false, true);

        CHECK_FALSE(player.IsOnGround());
    }

    TEST_CASE("Jump cannot be triggered while airborne") {
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));

        player.Jump();

        CHECK_FALSE(player.IsOnGround());
        CHECK(player.getStamina() == doctest::Approx(100.0f));
    }

    TEST_CASE("UpdatePhysics lands on a solid tile platform") {
        TileMap map(MAP_COLS, MAP_ROWS, TILE_SIZE);
        map.Set(3, 10, TileType::Earth);
        Player player;
        player.SetPosition(Vector2D(20.25f, 100.2f));

        player.UpdatePhysics(map);

        CHECK(player.getPosition().y == doctest::Approx(100.0f));
        CHECK(player.IsOnGround());
    }

    TEST_CASE("UpdatePhysics stops a jump at a solid ceiling tile") {
        TileMap map(MAP_COLS, MAP_ROWS, TILE_SIZE);
        map.Set(3, 10, TileType::Earth);
        map.Set(3, 15, TileType::Earth);
        Player player;
        player.SetPosition(Vector2D(20.25f, 100.2f));
        landPlayer(player, map);

        REQUIRE(player.getPosition().y == doctest::Approx(100.0f));
        player.Jump();
        player.UpdatePhysics(map);
        player.UpdatePhysics(map);

        CHECK(player.getPosition().y == doctest::Approx(110.0f));
        CHECK_FALSE(player.IsOnGround());
    }

    TEST_CASE("UpdatePhysics stops horizontal movement at solid tiles") {
        TileMap map(MAP_COLS, MAP_ROWS, TILE_SIZE);
        map.Set(3, 9, TileType::Earth);
        Player player;
        player.SetPosition(Vector2D(20.0f, 60.0f));

        player.Move(false, true, false);
        player.UpdatePhysics(map);

        CHECK(player.getPosition().x == doctest::Approx(20.0f));
    }

    TEST_CASE("diagonal movement cannot pass through a wall corner") {
        TileMap map = makeTestMap();
        map.Set(3, 7, TileType::Earth);
        Player player;
        player.SetPosition(Vector2D(20.0f, GROUND_Y + PLAYER_HEIGHT));
        landPlayer(player, map);

        player.Move(false, true, true);
        player.UpdatePhysics(map);

        CHECK(player.getPosition().x == doctest::Approx(20.0f));
        CHECK(player.getPosition().y > FLOOR_TOP);
    }
}
