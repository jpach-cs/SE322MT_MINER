/*******************************************************************************************
*
*   Montana Tech Miner - SE322 Prototype
*   Based on RayLib 2D Challenge by Hans de Ruiter
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>

#define TILE_SIZE 10
#define SCREEN_W 480
#define SCREEN_H 270
#define MAP_COLS (SCREEN_W / TILE_SIZE)
#define MAP_ROWS (SCREEN_H / TILE_SIZE)

static const int player_W = TILE_SIZE;
static const int player_H = TILE_SIZE * 3;
static const int player_speed = 2;
static const float gravity = 0.4f;
static const float jump_force = -7.5f;
static const float max_fall = 9.0f;


int main(void)
{
    // Initialization
    const float groundY    = (float)(MAP_ROWS * 4 / 5) * TILE_SIZE;  // top edge of the floor

    InitWindow(SCREEN_W, SCREEN_H, "Montana Tech Miner");

    Vector2 pos = {
        (SCREEN_W / 2.0f) - (player_W / 2),
        groundY - player_H
    };
    Vector2 vel = { 0.0f, 0.0f };
    bool onGround = true;
    bool facingRight = true;

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // ── INPUT ──────────────────────────────────────────────────────────
        if (IsKeyDown(KEY_RIGHT)) {
            vel.x = player_speed;
            facingRight = true;
        } else if (IsKeyDown(KEY_LEFT)) {
            vel.x = -player_speed;
            facingRight = false;
        } else {
            vel.x = 0;
        }

        // jump only when standing on the ground
        if (IsKeyPressed(KEY_SPACE) && onGround) {
            vel.y = jump_force;
            onGround = false;
        }

        // ── PHYSICS ────────────────────────────────────────────────────────
        vel.y += gravity;
        if (vel.y > max_fall) vel.y = max_fall;
        pos    = Vector2Add(pos, vel);

        // ── FLOOR COLLISION ────────────────────────────────────────────────
        if (pos.y + player_H >= groundY) {
            pos.y = groundY - player_H;
            vel.y = 0.0f;
            onGround        = true;
        }

        // ── SCREEN BOUNDARIES ──────────────────────────────────────────────
        if (pos.x < 0)
            pos.x = 0;
        if (pos.x + player_W > SCREEN_W )
            pos.x = (float)(SCREEN_W - player_W);

        // ── DRAW ───────────────────────────────────────────────────────────
        BeginDrawing();
        
        ClearBackground((Color){ 18, 10, 5, 255 });

        /* temporary floor */
        DrawRectangle(0, (int)groundY, SCREEN_W, SCREEN_H - (int)groundY, DARKBROWN);

        /* player placeholder – green = facing right, lime = facing left */
        DrawRectangle((int)pos.x, (int)pos.y, player_W, player_H, facingRight ? GREEN : LIME);

        EndDrawing();
    }

    // Deinit
    CloseWindow();
    return 0;
}