#pragma once

/* world_view.hpp
 *
 * View: drawing only. May read the Model; never changes it.
 *
 * drawWorld and the tile colors. Today: drawMap and tileScreenRect.
 *
 * Placeholder from the skeleton PR. Target layout: sugestions.md in
 * jpach-cs/esof-322_miner_decription. Design: docs/architecture.md (to be written from
 * the Alpha and Beta documents). Move code here one class at a time, with its tests in
 * tests/test_<module>.cpp, and keep make test passing at every step. */

#include "raylib.h"
#include "raymath.h"

#define TILE_SIZE 10
#define SCREEN_W 480
#define SCREEN_H 270
#define MAP_COLS (SCREEN_W / TILE_SIZE)
#define MAP_ROWS (SCREEN_H / TILE_SIZE)

 typedef enum {
    TILE_EMPTY = 0,   /* air  - nothing drawn, player falls through */
    TILE_EARTH,       /* dirt - solid ground                        */
    TILE_FOOD         /* food - restores stamina when touched       */
} TileType;

typedef struct {
    TileType tiles[MAP_ROWS][MAP_COLS];
} GameMap;

class WorldView
{
   public:
      WorldView();
      ~WorldView();

      void mapInit(GameMap* map);
      void drawMap(const GameMap* map);
      Rectangle tileScreenRect(int col, int row);
      
   private:
      GameMap map;

 };