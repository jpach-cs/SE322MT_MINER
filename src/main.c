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

#define TILE_SIZE 10
#define SCREEN_W 480
#define SCREEN_H 270
#define MAP_COLS (SCREEN_W / TILE_SIZE)
#define MAP_ROWS (SCREEN_H / TILE_SIZE)

#define JUMP_STAMINA_COST   15.0f
#define STAMINA_FROM_FOOD   40.0f

#define PLAYER_MAX_STAMINA  100.0f
#define STAMINA_BAR_X       15.0f
#define STAMINA_BAR_Y       15.0f
#define STAMINA_BAR_WIDTH   175.0f
#define STAMINA_BAR_HEIGHT  15.0f
#define STAMINA_FONT_SIZE   18.0f

static const int player_W = TILE_SIZE;
static const int player_H = TILE_SIZE * 3;
static const int player_speed = 2;
static const float gravity = -0.4f;
static const float jump_force = 7.5f;
static const float max_fall = -9.0f;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    bool onGround;
    bool facingRight;
    float stamina;
} Player;

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

static void DrawStaminaBar(const Player *player) {
    int fill_width = (int)((player->stamina / PLAYER_MAX_STAMINA) * STAMINA_BAR_WIDTH);

    // Empty Bar Background
    DrawRectangle(STAMINA_BAR_X, STAMINA_BAR_Y, STAMINA_BAR_WIDTH, STAMINA_BAR_HEIGHT, DARKGRAY);

    // Stamina Fill
    DrawRectangle(STAMINA_BAR_X, STAMINA_BAR_Y, fill_width, STAMINA_BAR_HEIGHT, BLUE);

    // Border
    DrawRectangleLines(STAMINA_BAR_X, STAMINA_BAR_Y, STAMINA_BAR_WIDTH, STAMINA_BAR_HEIGHT, WHITE);

    // Stamina Display
    DrawText(TextFormat("Stamina: %.1f / %.1f", player->stamina, PLAYER_MAX_STAMINA),
    STAMINA_BAR_X, STAMINA_BAR_Y + STAMINA_BAR_HEIGHT, STAMINA_FONT_SIZE, WHITE
    );
}

static void UsePlayerStamina(Player *player) {
    player->stamina -= JUMP_STAMINA_COST;
}

static void RegenerateStaminaFromFood(Player *player) {
    player->stamina += STAMINA_FROM_FOOD;
}

int main(void)
{
    // Initialization
    const float groundY = 4 * TILE_SIZE;

    InitWindow(SCREEN_W, SCREEN_H, "Sample Game");
    SetTargetFPS(60);

    GameMap map;
    mapInit(&map);

    Player player = {
        .position = (Vector2) {(SCREEN_W / 2.0f), groundY},
        .velocity = {0.0f, 0.0f},
        .onGround = true,
        .facingRight = true,
        .stamina = PLAYER_MAX_STAMINA
    };

    // Main game loop
    while (!WindowShouldClose())
    {
        // ── INPUT ──────────────────────────────────────────────────────────
        // If both direction keys are placed stand still
        if (IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)) {
            player.velocity.x = player_speed * (player.stamina < JUMP_STAMINA_COST ? 0.5f : 1.0f);
            player.facingRight = true;
        } else if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) {
            player.velocity.x = -player_speed * (player.stamina < JUMP_STAMINA_COST ? 0.5f : 1.0f);
            player.facingRight = false;
        } else {
            player.velocity.x = 0;
        }

        // jump only when standing on the ground
        if (IsKeyPressed(KEY_SPACE) && player.onGround) {
            player.velocity.y = jump_force * (player.stamina < JUMP_STAMINA_COST ? 0.75 : 1.0);
            player.onGround = false;
            UsePlayerStamina(&player);
        }

        if (IsKeyPressed(KEY_F)) {
            RegenerateStaminaFromFood(&player);
        }

        // ── PHYSICS ────────────────────────────────────────────────────────
        player.velocity.y += gravity;
        if (player.velocity.y < max_fall) player.velocity.y = max_fall;
        player.position = Vector2Add(player.position, player.velocity);

        // ── FLOOR COLLISION ────────────────────────────────────────────────
        if (player.position.y <= groundY) {
            player.position.y = groundY;
            player.velocity.y = 0.0f;
            player.onGround   = true;
        }

        // ── SCREEN BOUNDARIES ──────────────────────────────────────────────
        player.position.x = Clamp(player.position.x, 0, SCREEN_W - player_W);
        player.stamina = Clamp(player.stamina, 0.f, PLAYER_MAX_STAMINA);

        // ── DRAW ───────────────────────────────────────────────────────────
        BeginDrawing();

        ClearBackground((Color){ 18, 10, 5, 255 });

        /* temporary floor */
        drawMap(&map);

        DrawStaminaBar(&player);

        /* player placeholder – green = facing right, lime = facing left */
        DrawRectangle((int)player.position.x, (int)SCREEN_H - (player.position.y + player_H), player_W, player_H, player.facingRight ? GREEN : LIME);

        EndDrawing();
    }

    // Deinit
    CloseWindow();
    return 0;
}
