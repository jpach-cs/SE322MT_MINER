/* test_tilemap.cpp
 *
 * Unit tests for src/model/tilemap.hpp: the TileMap grid, bounds, solidity,
 * the test map layout (mirrors mapInit in main.c), and world-space tile
 * rectangles. Model code has no raylib, so these tests never need a window. */
#include "doctest.h"

#include "../src/model/config.hpp"
#include "../src/model/tilemap.hpp"

namespace {

int countTiles(const TileMap& map, TileType type) {
    int count = 0;
    for (int row = 0; row < map.Rows(); row++) {
        for (int col = 0; col < map.Cols(); col++) {
            if (map.Get(col, row) == type) count++;
        }
    }
    return count;
}

TileMap testMap() {
    TileMap map(MAP_COLS, MAP_ROWS, TILE_SIZE);
    map.LoadTestMap();
    return map;
}

}  // namespace

TEST_SUITE("tilemap") {
    TEST_CASE("a new map is empty and reports its dimensions") {
        const TileMap map(MAP_COLS, MAP_ROWS, TILE_SIZE);

        CHECK(map.Cols() == MAP_COLS);
        CHECK(map.Rows() == MAP_ROWS);
        CHECK(map.TileSize() == TILE_SIZE);
        CHECK(countTiles(map, TileType::Empty) == MAP_COLS * MAP_ROWS);
    }

    TEST_CASE("InBounds covers exactly the grid") {
        const TileMap map(MAP_COLS, MAP_ROWS, TILE_SIZE);

        CHECK(map.InBounds(0, 0));
        CHECK(map.InBounds(MAP_COLS - 1, MAP_ROWS - 1));
        CHECK_FALSE(map.InBounds(-1, 0));
        CHECK_FALSE(map.InBounds(0, -1));
        CHECK_FALSE(map.InBounds(MAP_COLS, 0));
        CHECK_FALSE(map.InBounds(0, MAP_ROWS));
    }

    TEST_CASE("Set and Get round-trip inside the map and ignore the outside") {
        TileMap map(MAP_COLS, MAP_ROWS, TILE_SIZE);

        map.Set(3, 7, TileType::Food);
        CHECK(map.Get(3, 7) == TileType::Food);

        map.Set(3, 7, TileType::Empty);
        CHECK(map.Get(3, 7) == TileType::Empty);

        map.Set(-1, 7, TileType::Earth);
        map.Set(3, MAP_ROWS, TileType::Earth);
        CHECK(map.Get(-1, 7) == TileType::Empty);
        CHECK(map.Get(3, MAP_ROWS) == TileType::Empty);
        CHECK(countTiles(map, TileType::Earth) == 0);
    }

    TEST_CASE("LoadTestMap fills rows 0 to 4 with earth") {
        const TileMap map = testMap();

        for (int col = 0; col < MAP_COLS; col++) {
            for (int row = 0; row < 5; row++) CHECK(map.Get(col, row) == TileType::Earth);
            CHECK(map.Get(col, 6) == TileType::Empty);
        }
    }

    TEST_CASE("LoadTestMap places the three test food tiles and nothing else above ground") {
        const TileMap map = testMap();

        CHECK(map.Get(8, 5) == TileType::Food);
        CHECK(map.Get(40, 5) == TileType::Food);
        CHECK(map.Get(24, 10) == TileType::Food);
        CHECK(countTiles(map, TileType::Food) == 3);
        CHECK(countTiles(map, TileType::Earth) == 5 * MAP_COLS);
    }

    TEST_CASE("IsSolid treats everything outside the map as solid") {
        const TileMap map = testMap();

        CHECK(map.IsSolid(-1, 10));
        CHECK(map.IsSolid(MAP_COLS, 10));
        CHECK(map.IsSolid(10, -1));
        CHECK(map.IsSolid(10, MAP_ROWS));
    }

    TEST_CASE("IsSolid distinguishes earth and food from empty") {
        const TileMap map = testMap();

        CHECK(map.IsSolid(3, 3));
        CHECK(map.IsSolid(8, 5));
        CHECK_FALSE(map.IsSolid(4, 6));
    }

    TEST_CASE("tile rectangles are one tile square and count upward from the floor") {
        const TileMap map = testMap();
        const float t = static_cast<float>(TILE_SIZE);

        const WorldRect r = map.TileWorldRect(3, 4);
        CHECK(r.x == doctest::Approx(3.0f * t));
        CHECK(r.width == doctest::Approx(t));
        CHECK(r.height == doctest::Approx(t));
        CHECK(r.y + r.height == doctest::Approx(4.0f * t)); /* top of row 4 is groundY */
        CHECK(map.TileWorldRect(0, 2).y > map.TileWorldRect(0, 1).y);
    }
}
