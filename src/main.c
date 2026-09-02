/*******************************************************************************************
*
*   Montana Tech Miner - SE322 Prototype
*   Based on RayLib 2D Challenge by Hans de Ruiter
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>

bool isTextureValid(const Texture2D *texture) {
    return texture->id > 0;
}

int main(void)
{
    // Initialization
    const int screenWidth  = 800;
    const int screenHeight = 450;

    const int   minerSpeed = 5;
    const float gravity    = 0.5f;
    const float jumpForce  = -12.0f;
    const float groundY    = screenHeight - 80.0f;  // top edge of the floor

    InitWindow(screenWidth, screenHeight, "Montana Tech Miner");

    const char *filename = "resources\\miner_walk.PNG"; // check "resources\\thomas.PNG"
    Texture2D miner = LoadTexture(filename);
    if (!isTextureValid(&miner)) {
        while (!WindowShouldClose()) {
            BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText(TextFormat("ERROR: Couldn't load %s.", filename),
                         20, 20, 20, BLACK);
            EndDrawing();
        }
        return 10;
    }

    unsigned numFrames  = 4; // 8 for thomas - line 29
    int      frameWidth = miner.width / numFrames;
    Rectangle frameRec  = { 0.0f, 0.0f, (float)frameWidth, (float)miner.height };

    Vector2 minerPosition = {
        screenWidth / 2.0f,
        groundY - miner.height    // start standing on the ground
    };
    Vector2 minerVelocity = { 0.0f, 0.0f };
    bool onGround = true;

    unsigned frameDelay        = 5;
    unsigned frameDelayCounter = 0;
    unsigned frameIndex        = 0;

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // ── INPUT ──────────────────────────────────────────────────────────
        if (IsKeyDown(KEY_RIGHT)) {
            minerVelocity.x = minerSpeed;
            if (frameRec.width < 0) frameRec.width = -frameRec.width; // face right
        } else if (IsKeyDown(KEY_LEFT)) {
            minerVelocity.x = -minerSpeed;
            if (frameRec.width > 0) frameRec.width = -frameRec.width; // face left
        } else {
            minerVelocity.x = 0;
        }

        // jump only when standing on the ground
        if (IsKeyPressed(KEY_SPACE) && onGround) {
            minerVelocity.y = jumpForce;
            onGround = false;
        }

        // ── PHYSICS ────────────────────────────────────────────────────────
        minerVelocity.y += gravity;
        minerPosition    = Vector2Add(minerPosition, minerVelocity);

        // ── FLOOR COLLISION ────────────────────────────────────────────────
        if (minerPosition.y + miner.height >= groundY) {
            minerPosition.y = groundY - miner.height;
            minerVelocity.y = 0.0f;
            onGround        = true;
        }

        // ── SCREEN BOUNDARIES ──────────────────────────────────────────────
        if (minerPosition.x < 0)
            minerPosition.x = 0;
        if (minerPosition.x > screenWidth - frameWidth)
            minerPosition.x = (float)(screenWidth - frameWidth);

        // ── ANIMATION ──────────────────────────────────────────────────────
        bool minerMoving = (minerVelocity.x != 0.0f);

        if (!onGround) {
            frameIndex = 1;                    // in the air: frozen frame
        } else {
            ++frameDelayCounter;
            if (frameDelayCounter > frameDelay) {
                frameDelayCounter = 0;
                if (minerMoving) {
                    ++frameIndex;
                    frameIndex %= numFrames;   // 0 → 1 → 2 → 3 → 0 ...
                } else {
                    frameIndex = 0;            // standing: first frame
                }
            }
        }

        // preserve direction (sign of frameRec.width) when changing frame
        float dir      = (frameRec.width >= 0) ? 1.0f : -1.0f;
        frameRec.x     = (float)frameWidth * frameIndex;
        frameRec.width = dir * (float)frameWidth;

        // ── DRAW ───────────────────────────────────────────────────────────
        BeginDrawing();
            ClearBackground((Color){ 30, 20, 10, 255 });

            // floor
            DrawRectangle(0, (int)groundY,
                          screenWidth, screenHeight - (int)groundY,
                          DARKBROWN);

            // player
            DrawTextureRec(miner, frameRec, minerPosition, WHITE);

        EndDrawing();
    }

    // De-Initialization
    UnloadTexture(miner);
    CloseWindow();
    return 0;
}