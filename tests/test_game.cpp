/* Unit tests for the game rules in src/main.c.
 *
 * Until the C++ restructure splits main.c into model/view/controller files,
 * the game is compiled straight into this test binary with main() renamed, so
 * doctest owns main(). Only functions that do not open a window are called.
 * When a rule moves into src/model/, move its tests into a matching
 * tests/test_<module>.cpp and drop it from here.
 *
 * Only this one file may include main.c: game_main() is not static, so a
 * second copy would be a duplicate symbol at link time. */
#include "doctest.h"

#define main game_main
#include "../src/main.c"
#undef main

namespace {

const float GROUND_Y = 4 * TILE_SIZE; /* same value main() uses */

Player playerAt(float x, float y, float stamina) {
    Player p{};
    p.position = Vector2{x, y};
    p.onGround = true;
    p.facingRight = true;
    p.stamina = stamina;
    return p;
}

GameMap emptyMap() {
    GameMap map{};
    for (int row = 0; row < MAP_ROWS; row++) {
        for (int col = 0; col < MAP_COLS; col++) {
            map.tiles[row][col] = TILE_EMPTY;
        }
    }
    return map;
}

int countTiles(const GameMap& map, TileType type) {
    int count = 0;
    for (int row = 0; row < MAP_ROWS; row++) {
        for (int col = 0; col < MAP_COLS; col++) {
            if (map.tiles[row][col] == type) count++;
        }
    }
    return count;
}

} // namespace

TEST_SUITE("stamina") {
    TEST_CASE("the stamina rules use the values from the spec") {
        CHECK(PLAYER_MAX_STAMINA == doctest::Approx(100.0f));
        CHECK(JUMP_STAMINA_COST == doctest::Approx(15.0f));
        CHECK(SPEED_PENALTY == doctest::Approx(0.75f));
        CHECK(JUMP_PENALTY == doctest::Approx(0.5f));
    }

    TEST_CASE("a jump costs JUMP_STAMINA_COST") {
        Player p = playerAt(0, GROUND_Y, PLAYER_MAX_STAMINA);
        UpdatePlayerStamina(&p, -JUMP_STAMINA_COST);
        CHECK(p.stamina == doctest::Approx(PLAYER_MAX_STAMINA - JUMP_STAMINA_COST));
    }

    TEST_CASE("a jump with exactly JUMP_STAMINA_COST left drains stamina to 0") {
        Player p = playerAt(0, GROUND_Y, JUMP_STAMINA_COST);
        UpdatePlayerStamina(&p, -JUMP_STAMINA_COST);
        CHECK(p.stamina == doctest::Approx(0.0f));
    }

    TEST_CASE("a low-stamina jump does not drain stamina further") {
        Player p = playerAt(0, GROUND_Y, JUMP_STAMINA_COST - 5.0f);
        UpdatePlayerStamina(&p, -JUMP_STAMINA_COST);
        CHECK(p.stamina == doctest::Approx(JUMP_STAMINA_COST - 5.0f));
    }

    TEST_CASE("stamina never drops below 0 over repeated jumps") {
        Player p = playerAt(0, GROUND_Y, PLAYER_MAX_STAMINA);
        for (int i = 0; i < 20; i++) {
            UpdatePlayerStamina(&p, -JUMP_STAMINA_COST);
            CHECK(p.stamina >= 0.0f);
        }
    }

    TEST_CASE("regeneration is capped at PLAYER_MAX_STAMINA") {
        Player p = playerAt(0, GROUND_Y, PLAYER_MAX_STAMINA - 10.0f);
        UpdatePlayerStamina(&p, STAMINA_FROM_FOOD);
        CHECK(p.stamina == doctest::Approx(PLAYER_MAX_STAMINA));
    }

    TEST_CASE("regeneration from empty adds the full amount") {
        Player p = playerAt(0, GROUND_Y, 0.0f);
        UpdatePlayerStamina(&p, STAMINA_FROM_FOOD);
        CHECK(p.stamina == doctest::Approx(STAMINA_FROM_FOOD));
    }
}

TEST_SUITE("food") {
    TEST_CASE("touching food restores STAMINA_FROM_FOOD and consumes the tile") {
        GameMap map = emptyMap();
        map.tiles[5][8] = TILE_FOOD;
        Player p = playerAt(8 * TILE_SIZE, GROUND_Y, 20.0f);

        RegenerateStaminaFromFood(&p, &map);

        CHECK(p.stamina == doctest::Approx(20.0f + STAMINA_FROM_FOOD));
        CHECK(map.tiles[5][8] == TILE_EMPTY);
    }

    TEST_CASE("food is only eaten once") {
        GameMap map = emptyMap();
        map.tiles[5][8] = TILE_FOOD;
        Player p = playerAt(8 * TILE_SIZE, GROUND_Y, 20.0f);

        RegenerateStaminaFromFood(&p, &map);
        RegenerateStaminaFromFood(&p, &map);

        CHECK(p.stamina == doctest::Approx(20.0f + STAMINA_FROM_FOOD));
    }

    TEST_CASE("food out of reach is left alone") {
        GameMap map = emptyMap();
        map.tiles[5][40] = TILE_FOOD;
        Player p = playerAt(8 * TILE_SIZE, GROUND_Y, 20.0f);

        RegenerateStaminaFromFood(&p, &map);

        CHECK(p.stamina == doctest::Approx(20.0f));
        CHECK(map.tiles[5][40] == TILE_FOOD);
    }

    TEST_CASE("eating food at full stamina still consumes it without exceeding the cap") {
        GameMap map = emptyMap();
        map.tiles[5][8] = TILE_FOOD;
        Player p = playerAt(8 * TILE_SIZE, GROUND_Y, PLAYER_MAX_STAMINA);

        RegenerateStaminaFromFood(&p, &map);

        CHECK(p.stamina == doctest::Approx(PLAYER_MAX_STAMINA));
        CHECK(map.tiles[5][8] == TILE_EMPTY);
    }
}

TEST_SUITE("tilemap") {
    TEST_CASE("mapInit fills rows 0 to 4 with earth") {
        GameMap map;
        mapInit(&map);
        for (int row = 0; row < 5; row++) {
            for (int col = 0; col < MAP_COLS; col++) {
                CHECK(map.tiles[row][col] == TILE_EARTH);
            }
        }
    }

    TEST_CASE("mapInit places the three test food tiles and nothing else above ground") {
        GameMap map;
        mapInit(&map);
        CHECK(map.tiles[5][8] == TILE_FOOD);
        CHECK(map.tiles[5][40] == TILE_FOOD);
        CHECK(map.tiles[10][24] == TILE_FOOD);
        CHECK(countTiles(map, TILE_FOOD) == 3);
        CHECK(countTiles(map, TILE_EARTH) == 5 * MAP_COLS);
    }

    TEST_CASE("tileSolid treats everything outside the map as solid") {
        GameMap map = emptyMap();
        CHECK(tileSolid(&map, -1, 10));
        CHECK(tileSolid(&map, MAP_COLS, 10));
        CHECK(tileSolid(&map, 10, -1));
        CHECK(tileSolid(&map, 10, MAP_ROWS));
    }

    TEST_CASE("tileSolid distinguishes earth from empty") {
        GameMap map = emptyMap();
        map.tiles[3][3] = TILE_EARTH;
        CHECK(tileSolid(&map, 3, 3));
        CHECK_FALSE(tileSolid(&map, 4, 3));
    }
}

TEST_SUITE("coordinates") {
    TEST_CASE("a player standing on the ground has its feet on top of row 4") {
        Player p = playerAt(0, GROUND_Y, PLAYER_MAX_STAMINA);
        Rectangle body = playerScreenRect(&p);
        Rectangle topEarth = tileScreenRect(0, 4);
        CHECK(body.y + body.height == doctest::Approx(topEarth.y));
    }

    TEST_CASE("map rows count upward from the bottom of the screen") {
        CHECK(tileScreenRect(0, 1).y > tileScreenRect(0, 2).y);
        CHECK(tileScreenRect(3, 0).x == doctest::Approx(3.0f * TILE_SIZE));
    }

    TEST_CASE("the player rectangle is one tile wide and three tiles tall") {
        Player p = playerAt(100, GROUND_Y, PLAYER_MAX_STAMINA);
        Rectangle body = playerScreenRect(&p);
        CHECK(body.width == doctest::Approx((float)TILE_SIZE));
        CHECK(body.height == doctest::Approx(3.0f * TILE_SIZE));
        CHECK(body.x == doctest::Approx(100.0f));
    }
}
