/*******************************************************************************************
*
*   Montana Tech Miner - SE322 Prototype
*   Based on RayLib 2D Challenge by Hans de Ruiter
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>

#define TILE_SIZE    10
#define SCREEN_W    480
#define SCREEN_H    270
#define MAP_COLS    (SCREEN_W / TILE_SIZE)   /* 48 */
#define MAP_ROWS    (SCREEN_H / TILE_SIZE)   /* 27 */

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

bool isTextureValid(const Texture2D *texture) {
    return texture->id > 0;
}

int main(void)
{
    // Initialization
    const int   minerSpeed = 5;
    const float gravity    = 0.5f;
    const float jumpForce  = -12.0f;

    const float groundY = (float)((MAP_ROWS * 4 / 5) * TILE_SIZE);   /* = 210 */

    InitWindow(SCREEN_W, SCREEN_H, "Montana Tech Miner");

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

    Vector2 pos = {
        (float)(SCREEN_W / 2 - PLAYER_W / 2),
        groundY - (float)PLAYER_H
    };
    Vector2 vel         = { 0.0f, 0.0f };
    bool    onGround    = true;
    bool    facingRight = true;

    unsigned frameDelay        = 5;
    unsigned frameDelayCounter = 0;
    unsigned frameIndex        = 0;

    SetTargetFPS(60);

    GameMap map;
    MapInit(&map);
    // Main game loop
    while (!WindowShouldClose())
    {
        // ── INPUT ──────────────────────────────────────────────────────────
        if      (IsKeyDown(KEY_RIGHT)) { vel.x =  (float)PLAYER_SPD; facingRight = true;  }
        else if (IsKeyDown(KEY_LEFT))  { vel.x = -(float)PLAYER_SPD; facingRight = false; }
        else                             vel.x = 0.0f;

        if (IsKeyPressed(KEY_SPACE) && onGround) {
            vel.y    = JUMP_FORCE;
            onGround = false;
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

        // ── ANIMATION ──────────────────────────────────────────────────────
        bool minerMoving = (vel.x != 0.0f);

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

            ClearBackground((Color){ 18, 10, 5, 255 });

            /* temporary floor */
            MapDraw(&map);

            /* player placeholder – green = facing right, lime = facing left */
            DrawRectangle((int)pos.x, (int)pos.y,
                        PLAYER_W, PLAYER_H,
                        facingRight ? GREEN : LIME);

        EndDrawing();

    }

    // De-Initialization
    UnloadTexture(miner);
    CloseWindow();
    return 0;
}