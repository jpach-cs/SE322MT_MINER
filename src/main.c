/*******************************************************************************************
*
*   Montana Tech Miner - SE322 Prototype
*   Based on RayLib 2D Challenge by Hans de Ruiter
*
********************************************************************************************/
#define SUPPORT_CAMERA_SYSTEM       1
#include "raylib.h"
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
    const float groundY = 4 * TILE_SIZE;
    //const float groundY    = (float)(MAP_ROWS * 4 / 5) * TILE_SIZE;

    InitWindow(SCREEN_W, SCREEN_H, "Sample Game");
    SetTargetFPS(60);

    GameMap map;
    mapInit(&map);

    Vector2 pos = {
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
            vel.x = player_speed;
            facingRight = true;
        } else if (IsKeyDown(KEY_LEFT)) {
            vel.x = -player_speed;
            facingRight = false;
        } else {
            vel.x = 0;
        }
        else vel.x = 0.0f;

        if (IsKeyPressed(KEY_SPACE) && onGround || IsKeyPressed(KEY_UP) && onGround) {
            if (stamina >= JUMP_STAMINA_COST) {
                stamina -= JUMP_STAMINA_COST;
                vel.y    = JUMP_FORCE;
                onGround = false;

            }
            else if (stamina < JUMP_STAMINA_COST) {
                vel.y = JUMP_FORCE / 2.0f;  /* jump with half force if stamina is low */
    
                onGround = false;
            }
            
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

        // ── FOOD COLLISION ─────────────────────────────────────────────
        // Convert player position to grid coordinates
        int playerCol = (int)(pos.x + PLAYER_W / 2.0f) / TILE_SIZE;
        int playerRow = (int)(pos.y + PLAYER_H - 1) / TILE_SIZE;

        if (playerCol >= 0 && playerCol < MAP_COLS && playerRow >= 0 && playerRow < MAP_ROWS) {
            if (map.tiles[playerRow][playerCol] == TILE_FOOD) {
                stamina += STAMINA_FROM_FOOD;
    
                // Cap stamina
                if (stamina >= PLAYER_MAX_STAMINA) stamina = PLAYER_MAX_STAMINA;
    
                // Remove food after being eaten
                map.tiles[playerRow][playerCol] = TILE_EMPTY;
            }
        }

        // ── ANIMATION ──────────────────────────────────────────────────────
        

        

        // ── DRAW ───────────────────────────────────────────────────────────
        BeginDrawing();

            ClearBackground((Color){ 18, 10, 5, 255 });
            
            /* add this */
            MapDraw(&map);

            /* player placeholder – green = facing right, lime = facing left */
            DrawRectangle((int)pos.x, (int)pos.y,
                        PLAYER_W, PLAYER_H,
                        facingRight ? GREEN : LIME);
            
            DrawRectangle(15, 15,
                stamina, 15, BLUE);
            DrawRectangleLines(15, 15, BAR_WIDTH, 15, WHITE);
            DrawText(TextFormat("Stamina: %d", stamina), 15, 30, 20, WHITE);

        EndDrawing();
    }

    // Deinit
    CloseWindow();
    return 0;
}
