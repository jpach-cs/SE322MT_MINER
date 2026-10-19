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
#define PLAYER_MAX_STAMINA  150.0f
#define JUMP_STAMINA_COST   10.0f
#define STAMINA_FROM_FOOD   40.0f
#define STAMINA_BAR_X       15
#define STAMINA_BAR_Y       35
#define STAMINA_BAR_WIDTH   100
#define STAMINA_BAR_HEIGHT  15


static const int player_W = TILE_SIZE;
static const int player_H = TILE_SIZE * 3;
static const int player_speed = 2;
static const float gravity = -0.4f;
static const float jump_force = 7.5f;
static const float max_fall = -9.0f;


typedef enum
{
    TILE_EMPTY = 0,
    TILE_EARTH,
    TILE_FOOD
} TileType;

typedef struct
{
    TileType tiles[MAP_ROWS][MAP_COLS];
} GameMap;

typedef struct
{
    float stamina;
}Player;

static bool tileSolid(const GameMap* map, int col, int row)
{
    if (col < 0  || col >= MAP_COLS || row < 0 || row >= MAP_ROWS) return true;
    return map->tiles[row][col] != TILE_EMPTY;
}

static void mapInit(GameMap* map)
{
    for (int y = 0; y < MAP_ROWS; y++)
    {
        for (int x = 0; x < MAP_COLS; x++)
        {
            map->tiles[y][x] = (y < 5 ? TILE_EARTH : TILE_EMPTY);
        }
    }
}

static void drawMap(const GameMap* map)
{
    for (int y = 0; y < MAP_ROWS; y++)
    {
        for (int x = 0; x < MAP_COLS; x++)
        {
            if (map->tiles[y][x] == TILE_EMPTY) continue;
            DrawRectangle(x * TILE_SIZE, SCREEN_H - (y * TILE_SIZE), TILE_SIZE, TILE_SIZE, DARKBROWN);
        }
    }
}

static void drawStaminaBar(const Player *player)
{
    float fill_width = (player->stamina / PLAYER_MAX_STAMINA) * STAMINA_BAR_WIDTH;

    //Create stamina bar background
    DrawRectangle(STAMINA_BAR_X, STAMINA_BAR_Y, STAMINA_BAR_WIDTH, STAMINA_BAR_HEIGHT, DARKGRAY);
    // Create dynamic fill bar (on top of background)
    DrawRectangle(STAMINA_BAR_X, STAMINA_BAR_Y, fill_width, STAMINA_BAR_HEIGHT, SKYBLUE);
    // Create border around stamina bar (1px thick)
    DrawRectangleLines(STAMINA_BAR_X, STAMINA_BAR_Y, STAMINA_BAR_WIDTH, STAMINA_BAR_HEIGHT, WHITE);
}

static void regenerateStaminaFromFood(Player *player)
{
    // Only add stamina if current stamina is less than the max
    if (player->stamina < PLAYER_MAX_STAMINA)
    {
        player->stamina += STAMINA_FROM_FOOD;
    }

    // Cap player's stamina at the max amount
    if (player->stamina > PLAYER_MAX_STAMINA)
    {
        player->stamina = PLAYER_MAX_STAMINA;
    }
}


static void findFood(const GameMap *map, Vector2 pos, Player *player)
{
    int col = (int)(pos.x / TILE_SIZE);
    int row = (int)(pos.y / TILE_SIZE);

    // Ensure food tile is within map bounds
    if (col < 0  || col >= MAP_COLS || row < 0 || row >= MAP_ROWS)
    {
        return;
    }

    if (map->tiles[row][col] == TILE_FOOD)
    {
        regenerateStaminaFromFood(player);
    }
}

static void updatePlayerStamina(Player *player)
{
    if (player->stamina >= JUMP_STAMINA_COST)
    {
        player->stamina -= JUMP_STAMINA_COST;
    }
}


int main(void)
{
    // Create instance of Player
    Player player;
    player.stamina = PLAYER_MAX_STAMINA;

    // Initialization
    const float groundY = 4 * TILE_SIZE;

    InitWindow(SCREEN_W, SCREEN_H, "Sample Game");
    SetTargetFPS(60);

    GameMap map;
    mapInit(&map);

    // Player's x and y coordinates
    Vector2 pos = {
        (SCREEN_W / 2.0f) - (player_W / 2),
        groundY
    };

    // Player's horizontal and vertical movement speed
    Vector2 vel = { 0.0f, 0.0f };
    bool onGround = true;
    bool facingRight = true;


    // Main game loop
    while (!WindowShouldClose())
    {
        // ── INPUT ──────────────────────────────────────────────────────────
        if (IsKeyDown(KEY_RIGHT))
        {
            vel.x = player_speed;
            if (player.stamina < JUMP_STAMINA_COST)
            {
                vel.x *= 0.75f;    // Decrease movement speed by 25%
            }
            facingRight = true;
        }
        
        else if (IsKeyDown(KEY_LEFT))
        {
            vel.x = -player_speed;
            if (player.stamina < JUMP_STAMINA_COST)
            {
                vel.x *= 0.75f;    // Decrease movement speed by 25%
            }
            facingRight = false;
        }

        else 
        {
            vel.x = 0;
        }

        // jump only when standing on the ground
        if (IsKeyPressed(KEY_SPACE) && onGround)
        {
            vel.y = jump_force;
            if (player.stamina < JUMP_STAMINA_COST)
            {
                vel.y *= 0.5f;     // Decrease jump height by 50%
            }
            onGround = false;
            updatePlayerStamina(&player);
        }

        if (player.stamina < 0.0f)
        {
            player.stamina = 0.0f;
        }

        // ── PHYSICS ────────────────────────────────────────────────────────
        vel.y += gravity;
        if (vel.y < max_fall) vel.y = max_fall;
        pos = Vector2Add(pos, vel);

        // ── FLOOR COLLISION ────────────────────────────────────────────────
        if (pos.y <= groundY)
        {
            pos.y = groundY;
            vel.y = 0.0f;
            onGround = true;
        }

        // ── SCREEN BOUNDARIES ──────────────────────────────────────────────
        pos.x = Clamp(pos.x, 0, SCREEN_W - player_W);

        // ── FIND FOOD ─────────────────────────────────────────────────
        findFood(&map, pos, &player);

        // ── DRAW ───────────────────────────────────────────────────────────
        BeginDrawing();

        ClearBackground((Color){ 18, 10, 5, 255 });

        // temporary floor
        drawMap(&map);

        // player placeholder – green = facing right, lime = facing left
        DrawRectangle((int)pos.x, (int)SCREEN_H - (pos.y + player_H), player_W, player_H, facingRight ? GREEN : LIME);

        // (text displayed, horizontal pixel, vertical pixel, font size, color)
        DrawText(TextFormat("Stamina: %.0f / %.0f", player.stamina, PLAYER_MAX_STAMINA), STAMINA_BAR_X, STAMINA_BAR_Y + 20, 18, WHITE);
        
        drawStaminaBar(&player);

        EndDrawing();
    }

    // Deinit
    CloseWindow();
    return 0;
}
