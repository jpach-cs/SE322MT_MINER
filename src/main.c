/*******************************************************************************************
*
*   Montana Tech Miner - SE322 Prototype
*   Based on RayLib 2D Challenge by Hans de Ruiter
*
********************************************************************************************/

#include "raylib.h"
// deleted include raymath.h
#include <stdbool.h> // added 

/* ── #define only where required by the compiler ────────────────────────────
   These values are used to declare array sizes inside a struct in Assignment 2.
   C requires array dimensions to be compile-time constants.
   static const int does NOT qualify as a compile-time constant in C.        */

#define TILE_SIZE    10
#define SCREEN_W    480
#define SCREEN_H    270
#define MAP_COLS    (SCREEN_W / TILE_SIZE)   /* 48 */
#define MAP_ROWS    (SCREEN_H / TILE_SIZE)   /* 27 */

/* ── static const for everything else ───────────────────────────────────────
   These values are only used in runtime calculations.
   static const gives them a type, a name visible in the debugger,
   and limits their scope to this file.                                       */

static const int   PLAYER_W   = TILE_SIZE;        /* 10 px – 1 tile wide   */
static const int   PLAYER_H   = TILE_SIZE * 3;    /* 30 px – 3 tiles tall  */
static const int   PLAYER_SPD = 2;                /* pixels per frame      */
static const float GRAVITY    = 0.4f;             /* px / frame²           */
static const float JUMP_FORCE = -7.5f;            /* px / frame, upward    */
static const float MAX_FALL   = 9.0f;             /* must stay < TILE_SIZE */

// Deleted isTextureValid()

int main(void)
{
    // Initialization
    // removed const int screenWidth  =...
    // removed const int screenHeight =...
    
    const float groundY = (float)((MAP_ROWS * 4 / 5) * TILE_SIZE);   /* = 210 */  // added this

    const int   minerSpeed = 5;
    const float gravity    = 0.5f;
    const float jumpForce  = -12.0f;
    // const float groundY    = screenHeight - 80.0f;  // top edge of the floor

    InitWindow(SCREEN_W, SCREEN_H, "Montana Tech Miner");  // changed

    // deleted const char *filename = ...
    // deleted Texture2D miner = LoadTexture(...);
    // deleted if (!isTextureValid(&miner)) {...


    // deleted unsigned numFrames  = ...
    // deleted int      frameWidth = ...
    // deleted Rectangle frameRec  = ...

    Vector2 pos = { // changed
        (float)(SCREEN_W / 2 - PLAYER_W / 2),
        groundY - (float)PLAYER_H
    };
    Vector2 vel         = { 0.0f, 0.0f };
    bool    onGround    = true;
    bool    facingRight = true;

    // deleted unsigned frameDelay=...
    // deleted unsigned frameDelayCounter =...
    // deleted unsigned frameIndex=...

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // ── INPUT: changed ──────────────────────────────────────────────────────────
        if      (IsKeyDown(KEY_RIGHT)) { vel.x =  (float)PLAYER_SPD; facingRight = true;  }
        else if (IsKeyDown(KEY_LEFT))  { vel.x = -(float)PLAYER_SPD; facingRight = false; }
        else  vel.x = 0.0f;

        if (IsKeyPressed(KEY_SPACE) && onGround) {
            vel.y    = JUMP_FORCE;
            onGround = false;
        }

        // ── PHYSICS: changed ────────────────────────────────────────────────────────
        vel.y += GRAVITY; 
        if (vel.y > MAX_FALL) vel.y = MAX_FALL;

        pos.x += vel.x;
        pos.y += vel.y;

        // ── FLOOR COLLISION: changed ────────────────────────────────────────────────
        if (pos.y + PLAYER_H >= groundY) { 
            pos.y    = groundY - (float)PLAYER_H;
            vel.y    = 0.0f;
            onGround = true;
}

        // ── SCREEN BOUNDARIES: changed ──────────────────────────────────────────────
        if (pos.x < 0)                              pos.x = 0.0f;
        if (pos.x + PLAYER_W > (float)SCREEN_W)     pos.x = (float)(SCREEN_W - PLAYER_W);


        // ── ANIMATION ──────────────────────────────────────────────────────

        // deleted the entire animation section inside the loop

        // preserve direction (sign of frameRec.width) when changing frame
        // deleted float dir
        // deleted frameRec.x
        // deleted frameRec.width

        // ── DRAW: changed ───────────────────────────────────────────────────────────
        BeginDrawing();

            ClearBackground((Color){ 18, 10, 5, 255 });

            /* temporary floor */
            DrawRectangle(0, (int)groundY,
                        SCREEN_W, SCREEN_H - (int)groundY,
                        DARKBROWN);

            /* player placeholder – green = facing right, lime = facing left */
            DrawRectangle((int)pos.x, (int)pos.y,
                        PLAYER_W, PLAYER_H,
                        facingRight ? LIME : GREEN);

        EndDrawing();
    }

    // De-Initialization
    // deleted UnloadTexture(...);
    CloseWindow();
    return 0;
}