/* test_player.cpp */
#include "doctest.h"

#include "../src/model/player.hpp"

static const float FLOOR_TOP = GROUND_Y - PLAYER_H;

TEST_SUITE("player") {
    static void landPlayer(Player& player) {
        player.UpdatePhysics();
    }

    TEST_CASE("default constructor sets the starting state") {
        const Player player;

        CHECK(player.getPosition() == Vector2D(50.0f, GROUND_Y + PLAYER_H));
        CHECK(player.IsFacingRight());
        CHECK_FALSE(player.IsOnGround());
    }

    TEST_CASE("stamina constructor sets the same starting state") {
        const Player player(42.0f);

        CHECK(player.getPosition() == Vector2D(50.0f, GROUND_Y + PLAYER_H));
        CHECK(player.IsFacingRight());
        CHECK_FALSE(player.IsOnGround());
    }

    TEST_CASE("SetPosition replaces the position") {
        Player player;

        player.SetPosition(Vector2D(12.5f, -3.0f));

        CHECK(player.getPosition() == Vector2D(12.5f, -3.0f));
    }

    TEST_CASE("gravity is applied once per frame in the air") {
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));

        player.UpdatePhysics();
        CHECK(player.getPosition().x == doctest::Approx(10.0f));
        CHECK(player.getPosition().y == doctest::Approx(100.0f + GRAVITY));
        CHECK_FALSE(player.IsOnGround());

        player.UpdatePhysics();
        CHECK(player.getPosition().y == doctest::Approx(100.0f + 3.0f * GRAVITY));
    }

    TEST_CASE("falling speed is clamped to MAX_FALL") {
        Player player;
        player.SetPosition(Vector2D(0.0f, -1000.0f));

        for (int frame = 0; frame < 30; frame++) {
            player.UpdatePhysics();
        }
        const float before = player.getPosition().y;
        player.UpdatePhysics();

        CHECK(player.getPosition().y - before == doctest::Approx(MAX_FALL));
        CHECK_FALSE(player.IsOnGround());
    }

    TEST_CASE("player below the floor snaps onto it and lands") {
        Player player;

        player.UpdatePhysics();

        CHECK(player.getPosition().y == doctest::Approx(FLOOR_TOP));
        CHECK(player.IsOnGround());
    }

    TEST_CASE("landed player stays on the floor") {
        Player player;
        
        for (int i = 0; i < 10; i++) {
            player.UpdatePhysics();
        }

        CHECK(player.getPosition() == Vector2D(50.0f, FLOOR_TOP));
        CHECK(player.IsOnGround());
    }

    TEST_CASE("Move moves right and updates facing direction") {
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));

        player.Move(false, true, false);
        player.UpdatePhysics();

        CHECK(player.getPosition().x == doctest::Approx(12.0f));
        CHECK(player.IsFacingRight());
    }

    TEST_CASE("Move moves left and updates facing direction") {
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));

        player.Move(true, false, false);
        player.UpdatePhysics();

        CHECK(player.getPosition().x == doctest::Approx(8.0f));
        CHECK_FALSE(player.IsFacingRight());
    }

    TEST_CASE("Move with no direction stops horizontal movement") {
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));
        player.Move(false, true, false);

        player.Move(false, false, false);
        player.UpdatePhysics();

        CHECK(player.getPosition().x == doctest::Approx(10.0f));
    }

    TEST_CASE("Move without arguments stops horizontal movement") {
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));
        player.Move(false, true, false);

        player.Move();
        player.UpdatePhysics();

        CHECK(player.getPosition().x == doctest::Approx(10.0f));
    }

    TEST_CASE("Move prioritizes neither direction when both are pressed") {
        Player player;
        player.SetPosition(Vector2D(10.0f, 100.0f));

        player.Move(true, true, false);
        player.UpdatePhysics();

        CHECK(player.getPosition().x == doctest::Approx(10.0f));
        CHECK(player.IsFacingRight());
    }

    TEST_CASE("Jump takes a landed player off the ground") {
        Player player;
        landPlayer(player);

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
}
