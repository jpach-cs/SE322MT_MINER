/*******************************************************************************************
 *
 *   Montana Tech Miner - SE322 Prototype
 *   Based on RayLib 2D Challenge by Hans de Ruiter
 *
 ********************************************************************************************/

#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>

#define TILE_SIZE 10
#define SCREEN_W 480
#define SCREEN_H 270
#define MAP_COLS (SCREEN_W / TILE_SIZE) /* 48 */
#define MAP_ROWS (SCREEN_H / TILE_SIZE) /* 27 */

#define STAMINA_TEXT_X 15
#define STAMINA_TEXT_Y 15
#define STAMINA_TEXT_FONT 18

static const int PLAYER_W = TILE_SIZE;     /* 10 px – 1 tile wide   */
static const int PLAYER_H = TILE_SIZE * 3; /* 30 px – 3 tiles tall  */
static const int PLAYER_SPD = 2;           /* pixels per frame      */
static const float GRAVITY = 0.4f;         /* px / frame²           */
static const float JUMP_FORCE = -7.5f;     /* px / frame, upward    */
static const float MAX_FALL = 9.0f;        /* must stay < TILE_SIZE */
static const float PLAYER_MAX_STAMINA = 15.0f;
static const float JUMP_STAMINA_COST = 15.0f;
static const float STAMINA_FROM_FOOD = 40.0f;

typedef enum
{
    TILE_EMPTY = 0,
    TILE_EARTH
} TileType;
typedef struct
{
    TileType tiles[MAP_ROWS][MAP_COLS];
} GameMap;
typedef struct
{
    float stamina;
} Player;

static void UpdatePlayerStamina(Player *player);
static void RegenerateStaminaFromFood(Player *player);

// Tile Logic
static bool TileSolid(const GameMap *m, int col, int row);
static void MapInit(GameMap *m);
static void MapDraw(const GameMap *m);

int main(void)
{
    // Initialization
    Player player;
    player.stamina = PLAYER_MAX_STAMINA;
    const float groundY = (float)((MAP_ROWS * 4 / 5) * TILE_SIZE); // 210

    InitWindow(SCREEN_W, SCREEN_H, "Montana Tech Miner");

    Vector2 pos = {(float)(SCREEN_W / 2 - PLAYER_W / 2), groundY - (float)PLAYER_H};
    Vector2 vel = {0.0f, 0.0f};
    bool onGround = true;
    bool facingRight = true;

    SetTargetFPS(60);

    GameMap map;
    MapInit(&map);

    // Main game loop
    while (!WindowShouldClose())
    {
        // ── INPUT ──────────────────────────────────────────────────────────
        if (IsKeyDown(KEY_RIGHT))
        {
            vel.x = (float)PLAYER_SPD;
            facingRight = true;
        }
        else if (IsKeyDown(KEY_LEFT))
        {
            vel.x = -(float)PLAYER_SPD;
            facingRight = false;
        }
        else
        {
            vel.x = 0.0f;
        }

        if (player.stamina < JUMP_STAMINA_COST)
        {
            vel.x *= .75;
        }

        if (IsKeyPressed(KEY_SPACE) && onGround)
        {
            vel.y = JUMP_FORCE;
            vel.y *= player.stamina < JUMP_STAMINA_COST ? .5 : 1; // Has to be done before stamina is decremented
            onGround = false;
            UpdatePlayerStamina(&player);
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
            pos.y = groundY - (float)PLAYER_H;
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

        // Food
        if (false) // TODO: check if player touches food tile
        {
            RegenerateStaminaFromFood(&player);
        }

        // ── DRAW ───────────────────────────────────────────────────────────
        BeginDrawing();
        ClearBackground((Color){30, 20, 10, 255});

        MapDraw(&map);

        /* player placeholder – green = facing right, lime = facing left */
        DrawRectangle((int)pos.x, (int)pos.y, PLAYER_W, PLAYER_H, facingRight ? LIME : GREEN);

        // stamina is displayed as a percentage
        DrawText(TextFormat("Stamina: %f / 100.0", player.stamina / PLAYER_MAX_STAMINA * 100), STAMINA_TEXT_X, STAMINA_TEXT_Y, STAMINA_TEXT_FONT, WHITE);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();
    return 0;
}

static void UpdatePlayerStamina(Player *player)
{
    player->stamina -= JUMP_STAMINA_COST;
    player->stamina += player->stamina < 0 ? JUMP_STAMINA_COST : 0.0f; // ensure stamina does not go below 0
}

static void RegenerateStaminaFromFood(Player *player)
{
    player->stamina += STAMINA_FROM_FOOD;
    player->stamina = player->stamina > PLAYER_MAX_STAMINA ? PLAYER_MAX_STAMINA : player->stamina; // ensure stamina does not excede max
}

static bool TileSolid(const GameMap *m, int col, int row)
{
    if (col < 0 || col >= MAP_COLS)
    {
        return true;
    }
    if (row < 0 || row >= MAP_ROWS)
    {
        return true;
    }
    return m->tiles[row][col] != TILE_EMPTY;
}

static void MapInit(GameMap *m)
{
    for (int r = 0; r < MAP_ROWS; r++)
    {
        for (int c = 0; c < MAP_COLS; c++)
        {
            m->tiles[r][c] = TILE_EMPTY;
        }
    }

    int floorRow = (MAP_ROWS * 4) / 5;
    for (int r = floorRow; r < MAP_ROWS; r++)
    {
        for (int c = 0; c < MAP_COLS; c++)
        {
            m->tiles[r][c] = TILE_EARTH;
        }
    }
}

static void MapDraw(const GameMap *m)
{
    for (int r = 0; r < MAP_ROWS; r++)
    {
        for (int c = 0; c < MAP_COLS; c++)
        {
            if (m->tiles[r][c] == TILE_EMPTY)
            {
                continue;
            }
            DrawRectangle(c * TILE_SIZE, r * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKBROWN);
        }
    }
}