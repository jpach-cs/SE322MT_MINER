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
#include <stdlib.h> // needed for malloc
#include <stdio.h>  // needed for printf debugging

#define TILE_SIZE 10
#define SCREEN_W 480
#define SCREEN_H 270
#define MAP_COLS (SCREEN_W / TILE_SIZE)
#define MAP_ROWS (SCREEN_H / TILE_SIZE)
#define PLAYER_MAX_STAMINA 150.0f
#define JUMP_STAMINA_COST 15.2f
#define STAMINA_FROM_FOOD 40.2f

static const int PLAYER_W = TILE_SIZE;
static const int PLAYER_H = TILE_SIZE * 3;
static const int PLAYER_SPD = 2;
static const float GRAVITY = 0.4f;
static const float JUMP_FORCE = -7.5f;
static const float MAX_FALL = 9.0f;

static int barWidth = 100;
static int barHeight = 15;
static int barPosX = 15;
static int barPosY = 35;

typedef struct
{
    Vector2 position;
    Vector2 velocity;
    bool onGround;
    bool facingRight;
    float stamina;
} Character;

static void drawStaminaBar(Character *player, int width, int height, int posX, int posY, Color outlineColor, Color fillColor)
{
    // Draw outline
    DrawRectangle(posX, posY, width, height, outlineColor);

    // Draw health
    int fillWidth = (player->stamina / PLAYER_MAX_STAMINA) * width;
    DrawRectangle(posX, posY, fillWidth, height, fillColor);
}

static void useStamina(Character *player, float amountUsed)
{
    if (player->stamina < amountUsed)
    {
        player->stamina = 0.0f;
    }
    else
    {
        player->stamina -= amountUsed;
    }
}

static void restoreStamina(Character *player, float amountRestored)
{
    if ((player->stamina + amountRestored) > PLAYER_MAX_STAMINA)
    {
        player->stamina = PLAYER_MAX_STAMINA;
    }
    else
    {
        player->stamina += amountRestored;
    }
}

Vector2 ConvertPlayerPositionToTile(Character *player)
{
    Vector2 TilePos;
    TilePos.x = (int)(player->position.x / TILE_SIZE);
    TilePos.y = (int)(player->position.y / TILE_SIZE);
    return TilePos;
}

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

// static bool TileSolid(const GameMap *m, int col, int row)
// {
//     if (col < 0 || col >= MAP_COLS) return true;
//     if (row < 0 || row >= MAP_ROWS) return true;
//     return m->tiles[row][col] != TILE_EMPTY;
// }

static void MapInit(GameMap *m)
{
    for (int r = 0; r < MAP_ROWS; r++)
        for (int c = 0; c < MAP_COLS; c++)
            m->tiles[r][c] = TILE_EMPTY;

    int floorRow = (MAP_ROWS * 4) / 5;
    for (int r = floorRow; r < MAP_ROWS; r++)
        for (int c = 0; c < MAP_COLS; c++)
            m->tiles[r][c] = TILE_EARTH;
}

static void MapDraw(const GameMap *m)
{
    for (int r = 0; r < MAP_ROWS; r++)
        for (int c = 0; c < MAP_COLS; c++) {
            if (m->tiles[r][c] == TILE_EMPTY) continue;
            if (m->tiles[r][c] == TILE_EARTH)
            {
                DrawRectangle(
                    c * TILE_SIZE,
                    r * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE,
                    DARKBROWN
                );
            }
            if (m->tiles[r][c] == TILE_FOOD)
            {
                DrawRectangle(
                    c * TILE_SIZE,
                    r * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE,
                    YELLOW
                );
            }
        }
}

int main(void)
{
    // Initialization
    const float groundY = (float)((MAP_ROWS * 4 / 5) * TILE_SIZE); // ground is bottom 20% of screen

    InitWindow(SCREEN_W, SCREEN_H, "Montana Tech Miner");

    GameMap map;
    MapInit(&map);
    map.tiles[15][30] = TILE_FOOD;
    map.tiles[18][18] = TILE_FOOD;

    // Initialize Player Character
    Character *player = malloc(sizeof(Character));

    if (player != NULL)
    {
        player->position = (Vector2){SCREEN_W / 2 - SCREEN_H / 2, groundY - (float)(PLAYER_H)};
        player->velocity = (Vector2){0.0f, 0.0f};
        player->facingRight = true;
        player->onGround = true; 
        player->stamina = PLAYER_MAX_STAMINA;
    }

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // ── INPUT ──────────────────────────────────────────────────────────
        if (IsKeyDown(KEY_D) && !IsKeyDown(KEY_A))
        {
            if (player->stamina < JUMP_STAMINA_COST)
            {
                player->velocity.x = (float)(PLAYER_SPD) * 0.75f; // 75% movement speed if stamina below jump cost
                player->facingRight = true;
            }
            else
            {
                player->velocity.x = (float)PLAYER_SPD; 
                player->facingRight = true;
            }
        }
        else if (IsKeyDown(KEY_A) && !IsKeyDown(KEY_D))
        {
            if (player->stamina < JUMP_STAMINA_COST)
            {
                player->velocity.x = -(float)(PLAYER_SPD) * 0.75f; // 75% movement speed if stamina below jump cost
                player->facingRight = false;
            }
            else
            {
                player->velocity.x = -(float)PLAYER_SPD; 
                player->facingRight = false;
            }
        }
        else
        {
            player->velocity.x = 0.0f;
        }

        if (IsKeyDown(KEY_SPACE) && player->onGround)
        {
            if (player->stamina >= JUMP_STAMINA_COST)
            {
                player->velocity.y += JUMP_FORCE;
                useStamina(player, JUMP_STAMINA_COST);
                player->onGround = false;
            }
            else
            {
                player->velocity.y += JUMP_FORCE * 0.5f; // 50% jump height if stamina below required for jumping
                useStamina(player, JUMP_STAMINA_COST);
                player->onGround = false;
            }

        }

        // ── PHYSICS ────────────────────────────────────────────────────────
        player->velocity.y += GRAVITY;
        if (player->velocity.y > MAX_FALL)
        {
            player->velocity.y = MAX_FALL;
        }
        player->position.x += player->velocity.x;
        player->position.y += player->velocity.y;

        // ── FLOOR COLLISION ────────────────────────────────────────────────
        if (player->position.y + PLAYER_H >= groundY)
        {
            player->position.y = groundY - (float)(PLAYER_H);
            player->velocity.y = 0.0f;
            player->onGround = true;
        }

        // ── FOOD COLLISION ─────────────────────────────────────────────────
        Vector2 TilePos = ConvertPlayerPositionToTile(player);
        if (map.tiles[(int)TilePos.y][(int)TilePos.x] == TILE_FOOD)
        {
            restoreStamina(player, STAMINA_FROM_FOOD);
            map.tiles[(int)TilePos.y][(int)TilePos.x] = TILE_EMPTY;
        }

        // ── SCREEN BOUNDARIES ──────────────────────────────────────────────
        if (player->position.x < 0)
        {
            player->position.x = 0.0f;
        }
        if (player->position.x + PLAYER_W > (float)SCREEN_W)
        {
            player->position.x = (float)(SCREEN_W - PLAYER_W);
        }

        // ── ANIMATION ──────────────────────────────────────────────────────
        

        // ── DRAW ───────────────────────────────────────────────────────────
        BeginDrawing();
            ClearBackground((Color){ 30, 20, 10, 255 });

            MapDraw(&map);

            DrawRectangle((int)player->position.x, (int)player->position.y, PLAYER_W, PLAYER_H, player->facingRight ? GREEN : LIME);

            DrawText(TextFormat("Stamina: %.1f / %.1f", player->stamina, PLAYER_MAX_STAMINA), 15, 15, 18, WHITE);

            drawStaminaBar(player, barWidth, barHeight, barPosX, barPosY, GRAY, GREEN);

        EndDrawing();

        printf("x: %.2f y: %.2f || TileX: %.2f TileY: %.2f \r", player->position.x, player->position.y, TilePos.x, TilePos.y);
    }

    // De-Initialization
    free(player);
    CloseWindow();
    return 0;
}