/*******************************************************************************************
*
*   Montana Tech Miner - SE322 Prototype
*   Based on RayLib 2D Challenge by Hans de Ruiter
*
********************************************************************************************/
#define SUPPORT_CAMERA_SYSTEM       1
#include "raylib.h"
// #include "raymath.h"
#include <stdbool.h>

#define TILE_SIZE 10
#define SCREEN_W 480
#define SCREEN_H 270
#define MAP_COLS (SCREEN_W / TILE_SIZE)
#define MAP_ROWS (SCREEN_H / TILE_SIZE)

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
    const float groundY = (float)((MAP_ROWS * 4 / 5) * TILE_SIZE);   /* = 210 */

    const int   minerSpeed = 5;
    const float gravity    = 0.5f;
    const float jumpForce  = -12.0f;
    // const float groundY    = screenHeight - 80.0f;  // top edge of the floor replaced by step 4a
    // another thing here cuz I accidently commited to a poorly spelt branch :sob:

    Player player = {100.0f};           /* initializing player's starting stamina as the max */

    InitWindow(SCREEN_W, SCREEN_H, "Montana Tech Miner");

    // these two added here to initial map after the window is initialized and before anything else starts.
    GameMap map;
    MapInit(&map);

    Vector2 pos = {
        (SCREEN_W / 2.0f) - (player_W / 2),
        groundY
    Vector2 pos = {
        (float)(SCREEN_W / 2 - PLAYER_W / 2),
        groundY - (float)PLAYER_H
    };
    Vector2 vel = { 0.0f, 0.0f };
    bool onGround = true;
    bool facingRight = true;

    Vector2 vel         = { 0.0f, 0.0f };
    bool    onGround    = true;
    bool    facingRight = true;


    SetTargetFPS(60);

    // Player starting attributes
    Player player = {100.0f};


    // Main game loop
    while (!WindowShouldClose())
    {
        // ── INPUT ──────────────────────────────────────────────────────────
        // changed (float)PLAYER_SPD to effective_speed
        if      (IsKeyDown(KEY_RIGHT)) { vel.x =  effective_speed; facingRight = true;  }
        else if (IsKeyDown(KEY_LEFT))  { vel.x = -effective_speed; facingRight = false; }
        else                             vel.x = 0.0f;

        if (IsKeyPressed(KEY_SPACE) && onGround) {
            UpdatePlayerStamina(player); 
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

        // ── DRAW ───────────────────────────────────────────────────────────
        BeginDrawing();

            ClearBackground((Color){ 18, 10, 5, 255 });

            /* temporary floor */
            MapDraw(&map);

            /* temporary food */
            DrawRectangle(120, (int)groundY + 30, PLAYER_W / 2, PLAYER_H / 2, RED);

            /* player placeholder – green = facing right, lime = facing left */
            DrawRectangle((int)pos.x, (int)pos.y,
                        PLAYER_W, PLAYER_H,
                        facingRight ? GREEN : LIME);

            /* debug stamina bar */
            DrawText(TextFormat("%3.2f / 100.0 ", player.stamina), 15, 15, 18, WHITE);
            
        EndDrawing();

        // ── FOOD COLLISION  ────────────────────────────────────────────────
        if ((int)(pos.y + PLAYER_H ) == (int)groundY + 30 && ((int)(pos.x + PLAYER_W ) == 120 - (PLAYER_W / 2) || 120 + (PLAYER_W / 2))) {
            RegenerateStaminaFromFood(player);
        }
    }

    // De-Initialization
    CloseWindow();
    return 0;
}

