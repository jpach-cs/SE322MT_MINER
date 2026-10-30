/* world_view.cpp
 *
 * View: drawing only. May read the Model; never changes it.
 *
 * Implementation of world_view.hpp.
 *
 * Placeholder from the skeleton PR. Target layout: sugestions.md in
 * jpach-cs/esof-322_miner_decription. Design: docs/architecture.md (to be written from
 * the Alpha and Beta documents). Move code here one class at a time, with its tests in
 * tests/test_<module>.cpp, and keep make test passing at every step. */

#include "world_view.hpp"

WorldView::WorldView()
{
    mapInit(&map);
}

WorldView::~WorldView()
{
    
}

void WorldView::mapInit(GameMap* map)
{
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            map->tiles[y][x] = (y < 5 ? TILE_EARTH : TILE_EMPTY);
        }
    }

    /* test food: two on the ground, one that needs a jump to reach */
    map->tiles[5][8]   = TILE_FOOD;
    map->tiles[5][40]  = TILE_FOOD;
    map->tiles[10][24] = TILE_FOOD;
}

void WorldView::drawMap(const GameMap* map)
{
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            if (map->tiles[y][x] == TILE_EMPTY) continue;
            DrawRectangleRec(tileScreenRect(x, y), map->tiles[y][x] == TILE_FOOD ? YELLOW : DARKBROWN);
        }
    }
}

Rectangle WorldView::tileScreenRect(int col, int row)
{
    return (Rectangle){ (float)(col * TILE_SIZE), (float)(SCREEN_H - (row * TILE_SIZE)),
                        (float)TILE_SIZE, (float)TILE_SIZE };
}