/*******************************************************************************************
*
*   Montana Tech Miner - SE322 Prototype
*   Based on RayLib 2D Challenge by Hans de Ruiter
*
********************************************************************************************/

#include "raylib.h"
#include <stdbool.h>

#define TILE_SIZE 10
#define SCREEN_W 480
#define SCREEN_H 270
#define MAP_COLS (SCREEN_W / TILE_SIZE)
#define MAP_ROWS (SCREEN_H / TILE_SIZE)

static const int PLAYER_W = TILE_SIZE;
static const int PLAYER_H = TILE_SIZE * 3;
static const int PLAYER_SPD = 2;
static const float GRAVITY = 0.4f;
static const float JUMP_FORCE = -7.5f;
static const float MAX_FALL = 9.0f;

typedef enum 
{
    TILE_EMPTY = 0,
    TILE_EARTH     
} TileType;

typedef struct 
{
    TileType tiles[MAP_ROWS][MAP_COLS];
} GameMap;

static bool TileSolid(const GameMap *m, int col, int row)
{
    if (col < 0 || col >= MAP_COLS) return true;
    if (row < 0 || row >= MAP_ROWS) return true;
    return m->tiles[row][col] != TILE_EMPTY;
}

static void MapInit(GameMap *m)
{
    for (int r = 0; r < MAP_ROWS; r++)
        for (int c = 0; c < MAP_COLS; c++)
            m->tiles[r][c] = TILE_EMPTY;

    int floorRow = (MAP_ROWS * 4) / 5;
    for (int r = floorRow; r < MAP_ROWS; r++)
        for (int c = 0; c < MAP_COLS; c++)
            m->tiles[r][c] = TILE_EARTH;
}

static void MapDraw(const GameMap *m)
{
    for (int r = 0; r < MAP_ROWS; r++)
        for (int c = 0; c < MAP_COLS; c++) {
            if (m->tiles[r][c] == TILE_EMPTY) continue;
            DrawRectangle(
                c * TILE_SIZE,
                r * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE,
                DARKBROWN
            );
        }
}

int main(void)
{
    // Initialization
    const float groundY = (float)((MAP_ROWS * 4 / 5) * TILE_SIZE); // ground is bottom 20% of screen

    InitWindow(SCREEN_W, SCREEN_H, "Montana Tech Miner");

    GameMap map;
    MapInit(&map);

    Vector2 pos = {SCREEN_W / 2 - SCREEN_H / 2, groundY - (float)(PLAYER_H)};
    Vector2 vel = { 0.0f, 0.0f };
    bool onGround = true;
    bool facingRight = true;

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // ── INPUT ──────────────────────────────────────────────────────────
        if (IsKeyDown(KEY_D) && !IsKeyDown(KEY_A))
        {
            vel.x = (float)PLAYER_SPD; facingRight = true;
        }
        else if (IsKeyDown(KEY_A) && !IsKeyDown(KEY_D))
        {
            vel.x = -(float)PLAYER_SPD; facingRight = false;
        }
        else
        {
            vel.x = 0.0f;
        }

        if (IsKeyDown(KEY_SPACE) && onGround)
        {
            vel.y += JUMP_FORCE;
            onGround = false;
        }
        // ── PHYSICS ────────────────────────────────────────────────────────
        vel.y += GRAVITY;
        if (vel.y > MAX_FALL)
        {
            vel.y = MAX_FALL;
        }
        pos.x += vel.x;
        pos.y += vel.y;

        // ── FLOOR COLLISION ────────────────────────────────────────────────
        if (pos.y + PLAYER_H >= groundY)
        {
            pos.y = groundY - (float)(PLAYER_H);
            vel.y = 0.0f;
            onGround = true;
        }

        // ── SCREEN BOUNDARIES ──────────────────────────────────────────────
        if (pos.x < 0)
        {
            pos.x = 0.0f;
        }
        if (pos.x + PLAYER_W > (float)SCREEN_W)
        {
            pos.x = (float)(SCREEN_W - PLAYER_W);
        }

        // ── ANIMATION ──────────────────────────────────────────────────────
        

        // ── DRAW ───────────────────────────────────────────────────────────
        BeginDrawing();
            ClearBackground((Color){ 30, 20, 10, 255 });

            MapDraw(&map);

            DrawRectangle((int)pos.x, (int)pos.y, PLAYER_W, PLAYER_H, facingRight ? GREEN : LIME);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();
    return 0;
}