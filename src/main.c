/*******************************************************************************************
*
*   Montana Tech Miner - SE322 Prototype
*   Based on RayLib 2D Challenge by Hans de Ruiter
*
********************************************************************************************/

#include "raylib.h"
#include <stdbool.h>

#define TILE_SIZE    10
#define SCREEN_W    480
#define SCREEN_H    270
#define MAP_COLS    (SCREEN_W / TILE_SIZE)   /* 48 */
#define MAP_ROWS    (SCREEN_H / TILE_SIZE)   /* 27 */

#define PLAYER_MAX_STAMINA  100.0f
#define JUMP_STAMINA_COST   15.0f
#define STAMINA_FROM_FOOD   40.0f
#define PLAYER_SPEED_PEN    0.75f
#define PLAYER_JUMP_PEN     0.5f
#define DEBUG_FONT_SIZE     18
#define DEBUG_X             15
#define DEBUG_Y             15


static const int   PLAYER_W   = TILE_SIZE;        /* 10 px – 1 tile wide   */
static const int   PLAYER_H   = TILE_SIZE * 3;    /* 30 px – 3 tiles tall  */
static const int   PLAYER_SPD = 2;                /* pixels per frame      */
static const float GRAVITY    = 0.4f;             /* px / frame²           */
static const float JUMP_FORCE = -7.5f;            /* px / frame, upward    */
static const float MAX_FALL   = 9.0f;             /* must stay < TILE_SIZE */

typedef enum {
    TILE_EMPTY = 0,   /* air  – nothing drawn, player falls through */
    TILE_EARTH        /* dirt – drawn as rectangle, solid ground     */
} TileType;

typedef struct {
    TileType tiles[MAP_ROWS][MAP_COLS];
} GameMap;

typedef struct {
    float stamina;
} Player;

static bool TileSolid(const GameMap *m, int col, int row)
{
    if (col < 0 || col >= MAP_COLS) return true;
    if (row < 0 || row >= MAP_ROWS) return true;
    return m->tiles[row][col] != TILE_EMPTY;
}

static void MapInit(GameMap *m)
{
    /* step 1: fill everything with air */
    for (int r = 0; r < MAP_ROWS; r++)
        for (int c = 0; c < MAP_COLS; c++)
            m->tiles[r][c] = TILE_EMPTY;

    /* step 2: solid floor – rows 21 to 26 */
    int floorRow = (MAP_ROWS * 4) / 5;   /* = 21 */
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

static void UpdatePlayerStamina(Player *player)
{
    player->stamina -= JUMP_STAMINA_COST;
}

static void RegenerateStaminaFromFood(Player *player)
{
    player->stamina += STAMINA_FROM_FOOD;

    // Set stamina cap to 100 if overfill
    if (player->stamina > PLAYER_MAX_STAMINA) {
        player->stamina = PLAYER_MAX_STAMINA;
    }
}

static void DrawDebugStamina(const Player *player) {
    DrawText(TextFormat("Stamina: %.1f / %.1f", player->stamina, PLAYER_MAX_STAMINA),
    DEBUG_X, DEBUG_Y, DEBUG_FONT_SIZE, WHITE
    );
}

int main(void)
{
    // Initialization
    const float groundY = (float) ((MAP_ROWS * 4 / 5) * TILE_SIZE);     /* = 210 */ 
    Player player;
    player.stamina = PLAYER_MAX_STAMINA;

    InitWindow(SCREEN_W, SCREEN_H, "Montana Tech Miner");

    GameMap map;
    MapInit(&map);

    Vector2 pos = {
        (float)(SCREEN_W / 2 - PLAYER_W / 2),
        groundY - (float)PLAYER_H
    };
    Vector2 vel         = { 0.0f, 0.0f };
    bool    onGround    = true;
    bool    facingRight = true;

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // ── INPUT ──────────────────────────────────────────────────────────
        if      (IsKeyDown(KEY_RIGHT)) { 
            vel.x =  (float)PLAYER_SPD;
            facingRight = true;  
        }
        else if (IsKeyDown(KEY_LEFT))  { 
            vel.x = -(float)PLAYER_SPD; 
            facingRight = false; 
        }
        else {
            vel.x = 0.0f;
        }

        // When low stamina, 25% reduction
        if (player.stamina < JUMP_STAMINA_COST) {
            vel.x *= PLAYER_SPEED_PEN;
        }

        if (IsKeyPressed(KEY_SPACE) && onGround && player.stamina >= JUMP_STAMINA_COST) {
            vel.y    = JUMP_FORCE;
            onGround = false;
            UpdatePlayerStamina(&player);
        }

        // ── PHYSICS ────────────────────────────────────────────────────────
        vel.y += GRAVITY;
        if (vel.y > MAX_FALL) vel.y = MAX_FALL;

        pos.x += vel.x;
        pos.y += vel.y;

        // ── FLOOR COLLISION ────────────────────────────────────────────────
        if (pos.y + PLAYER_H >= groundY) {
            pos.y    = groundY - (float)PLAYER_H;
            vel.y    = 0.0f;
            onGround = true;
        }

        // ── SCREEN BOUNDARIES ──────────────────────────────────────────────
        if (pos.x < 0)                                   pos.x = 0.0f;
        if (pos.x + PLAYER_W > (float)SCREEN_W)          pos.x = (float)(SCREEN_W - PLAYER_W);

        // ── DRAW ───────────────────────────────────────────────────────────
        BeginDrawing();

            ClearBackground((Color){ 18, 10, 5, 255 });

            MapDraw(&map);

            /* player placeholder – green = facing right, lime = facing left */
            DrawRectangle((int)pos.x, (int)pos.y,
                        PLAYER_W, PLAYER_H,
                        facingRight ? GREEN : LIME);

            DrawDebugStamina(&player);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();
    return 0;
}