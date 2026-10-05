/*******************************************************************************************
*
*   Montana Tech Miner - SE322 Prototype
*   Based on RayLib 2D Challenge by Hans de Ruiter
*
********************************************************************************************/
#define SUPPORT_CAMERA_SYSTEM       1
#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include <stdio.h>

#define TILE_SIZE 10
#define SCREEN_W 480
#define SCREEN_H 270
#define MAP_COLS (SCREEN_W / TILE_SIZE)
#define MAP_ROWS (SCREEN_H / TILE_SIZE)
#define PLAYER_MAX_STAMINA 100.f
#define JUMP_STAMINA_COST 10.f
#define STAMINA_FROM_FOOD 40.f

static const int player_W = TILE_SIZE;
static const int player_H = TILE_SIZE * 3;
static const int player_speed = 2;
static const float gravity = -0.4f;
static const float jump_force = 7.5f;
static const float max_fall = -9.0f;

typedef enum {
    TILE_EMPTY = 0,
    TILE_EARTH
} TileType;

typedef struct {
    TileType tiles[MAP_ROWS][MAP_COLS];
} GameMap;

typedef struct {
    float stamina;
    Vector2 pos;
} Player;

static bool tileSolid(const GameMap* map, int col, int row) {
    if (col < 0  || col >= MAP_COLS || row < 0 || row >= MAP_ROWS) return true;
    return map->tiles[row][col] != TILE_EMPTY;
}

static void mapInit(GameMap* map) {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            map->tiles[y][x] = (y < 5 ? TILE_EARTH : TILE_EMPTY);
        }
    }
}

static void drawMap(const GameMap* map) {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            if (map->tiles[y][x] == TILE_EMPTY) continue;
            DrawRectangle(x * TILE_SIZE, SCREEN_H - (y * TILE_SIZE), TILE_SIZE, TILE_SIZE, DARKBROWN);
        }
    }
}


int main(void)
{
    // Initialization
    const float groundY = 4 * TILE_SIZE;
    //const float groundY    = (float)(MAP_ROWS * 4 / 5) * TILE_SIZE;

    InitWindow(SCREEN_W, SCREEN_H, "Sample Game");
    SetTargetFPS(60);

    GameMap map;
    mapInit(&map);

    Player player= {};
    player.stamina = PLAYER_MAX_STAMINA;
    player.pos = (Vector2) {
        (SCREEN_W / 2.0f) - (player_W / 2),
        groundY
    };
    Vector2 vel = { 0.0f, 0.0f };
    bool onGround = true;
    bool facingRight = true;


    // Main game loop
    while (!WindowShouldClose())
    {
        // ── INPUT ──────────────────────────────────────────────────────────
        if (IsKeyDown(KEY_RIGHT)) {
            vel.x = player_speed * (player.stamina < JUMP_STAMINA_COST ? 0.5f : 1.0f);
            facingRight = true;
        } else if (IsKeyDown(KEY_LEFT)) {
            vel.x = -player_speed * (player.stamina < JUMP_STAMINA_COST ? 0.5f : 1.0f);
            facingRight = false;
        } else {
            vel.x = 0;
        }

        // jump only when standing on the ground
        if (IsKeyPressed(KEY_SPACE) && onGround) {
            vel.y = jump_force * (player.stamina < JUMP_STAMINA_COST ? 0.75 : 1.0);
            onGround = false;
            player.stamina -= JUMP_STAMINA_COST;
        }

        if (IsKeyPressed(KEY_F)) {
            player.stamina += STAMINA_FROM_FOOD;
        }

        // ── PHYSICS ────────────────────────────────────────────────────────
        vel.y += gravity;
        if (vel.y < max_fall) vel.y = max_fall;
        player.pos = Vector2Add(player.pos, vel);

        // ── FLOOR COLLISION ────────────────────────────────────────────────
        if (player.pos.y <= groundY) {
            player.pos.y = groundY;
            vel.y = 0.0f;
            onGround        = true;
        }

        // ── SCREEN BOUNDARIES ──────────────────────────────────────────────
        player.pos.x = Clamp(player.pos.x, 0, SCREEN_W - player_W);
        player.stamina = Clamp(player.stamina, 0.f, PLAYER_MAX_STAMINA);

        // ── DRAW ───────────────────────────────────────────────────────────
        BeginDrawing();

        ClearBackground((Color){ 18, 10, 5, 255 });

        /* temporary floor */
        drawMap(&map);

        /* player placeholder – green = facing right, lime = facing left */
        DrawRectangle((int)player.pos.x, (int)SCREEN_H - (player.pos.y + player_H), player_W, player_H, facingRight ? GREEN : LIME);

        char staminaText[21];
        snprintf(staminaText, 21, "Stamina: %3.1f/%3.1f", player.stamina, PLAYER_MAX_STAMINA);
        DrawText(staminaText, 15, 15, 18, WHITE);
        EndDrawing();
    }

    // Deinit
    CloseWindow();
    return 0;
}
