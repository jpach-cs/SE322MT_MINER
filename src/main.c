/*******************************************************************************************
 *
 *   Montana Tech Miner - SE322 Prototype
 *   Based on RayLib 2D Challenge by Hans de Ruiter
 *
 ********************************************************************************************/

#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>

#define TILE_SIZE 10
#define SCREEN_W 480
#define SCREEN_H 270
#define MAP_COLS (SCREEN_W / TILE_SIZE) /* 48 */
#define MAP_ROWS (SCREEN_H / TILE_SIZE) /* 27 */

#define STAMINA_BAR_X 15
#define STAMINA_BAR_Y 35
#define STAMINA_BAR_HEIGHT 15
#define STAMINA_BAR_WIDTH 100

#define STAMINA_TEXT_X 130
#define STAMINA_TEXT_Y 35
#define STAMINA_TEXT_FONT 18

#define STAMINA_MOVE_DRAIN .75
#define STAMINA_JUMP_DRAIN .5

static const int PLAYER_W = TILE_SIZE;     /* 10 px – 1 tile wide   */
static const int PLAYER_H = TILE_SIZE * 3; /* 30 px – 3 tiles tall  */
static const int PLAYER_SPD = 2;           /* pixels per frame      */
static const float GRAVITY = 0.4f;         /* px / frame²           */
static const float JUMP_FORCE = -7.5f;     /* px / frame, upward    */
static const float MAX_FALL = 9.0f;        /* must stay < TILE_SIZE */
static const float PLAYER_MAX_STAMINA = 15.0f;
static const float JUMP_STAMINA_COST = 1.25f;
static const float STAMINA_FROM_FOOD = 40.0f;
static const float GROUND_Y = (float)((MAP_ROWS * 4 / 5) * TILE_SIZE); // 210

typedef enum
{
    TILE_EMPTY = 0,
    TILE_EARTH
} TileType;
typedef struct
{
    TileType tiles[MAP_ROWS][MAP_COLS];
} GameMap;
typedef struct
{
    float stamina;
    bool facingRight;
    bool onGround;
} Player;
typedef struct
{
    Color fillColor;
    Color backgroundColor;
    int borderSize;
    Color borderColor;

    float fillWidth;
} StaminaBar;

static void UpdatePlayerStamina(Player *player);
static void RegenerateStaminaFromFood(Player *player);
static void InputHandler(Vector2 *velocity, Player *player);
static void PhysicsHandler(Vector2 *position, Vector2 *velocity, Player *player);
static void DrawHandler(const GameMap *map, const Vector2 *position, const Player *player, const StaminaBar *staminaBar);
static void DrawStaminaBar(const StaminaBar *staminaBar);

// Tile Logic
static bool TileSolid(const GameMap *m, int col, int row);
static void MapInit(GameMap *m);
static void MapDraw(const GameMap *m);

int main(void)
{
    // Initialization
    Player player = {PLAYER_MAX_STAMINA, true, true};
    StaminaBar staminaBar = {BLUE, DARKGRAY, 1, WHITE, (player.stamina / PLAYER_MAX_STAMINA) * STAMINA_BAR_WIDTH};
    Vector2 pos = {(float)(SCREEN_W / 2 - PLAYER_W / 2), GROUND_Y - (float)PLAYER_H};
    Vector2 vel = {0.0f, 0.0f};
    GameMap map;

    InitWindow(SCREEN_W, SCREEN_H, "Montana Tech Miner");
    SetTargetFPS(60);
    MapInit(&map);

    // Main game loop
    while (!WindowShouldClose())
    {
        // ── INPUT ──────────────────────────────────────────────────────────
        InputHandler(&vel, &player);

        // ── PHYSICS ────────────────────────────────────────────────────────
        // ── FLOOR COLLISION ────────────────────────────────────────────────
        // ── SCREEN BOUNDARIES ──────────────────────────────────────────────
        PhysicsHandler(&pos, &vel, &player);

        // Food
        if (false) // TODO: check if player touches food tile
        {
            RegenerateStaminaFromFood(&player);
        }
        staminaBar.fillWidth = (player.stamina / PLAYER_MAX_STAMINA) * STAMINA_BAR_WIDTH;

        // ── DRAW ───────────────────────────────────────────────────────────
        DrawHandler(&map, &pos, &player, &staminaBar);
    }

    // De-Initialization
    CloseWindow();
    return 0;
}

static void DrawStaminaBar(const StaminaBar *staminaBar)
{
    DrawRectangle(STAMINA_BAR_X - staminaBar->borderSize, STAMINA_BAR_Y - staminaBar->borderSize, STAMINA_BAR_WIDTH + staminaBar->borderSize * 2, STAMINA_BAR_HEIGHT + staminaBar->borderSize * 2, staminaBar->borderColor);
    DrawRectangle(STAMINA_BAR_X, STAMINA_BAR_Y, STAMINA_BAR_WIDTH, STAMINA_BAR_HEIGHT, staminaBar->backgroundColor);
    DrawRectangle(STAMINA_BAR_X, STAMINA_BAR_Y, staminaBar->fillWidth, STAMINA_BAR_HEIGHT, staminaBar->fillColor);
}

static void DrawHandler(const GameMap *map, const Vector2 *position, const Player *player, const StaminaBar *staminaBar)
{
    BeginDrawing();
    ClearBackground((Color){30, 20, 10, 255});

    MapDraw(map);

    /* player placeholder – green = facing right, lime = facing left */
    DrawRectangle((int)position->x, (int)position->y, PLAYER_W, PLAYER_H, player->facingRight ? LIME : GREEN);

    DrawStaminaBar(staminaBar);
    // stamina is displayed as a percentage
    DrawText(TextFormat("Stamina: %.2f / 100.0", player->stamina / PLAYER_MAX_STAMINA * 100), STAMINA_TEXT_X, STAMINA_TEXT_Y, STAMINA_TEXT_FONT, WHITE);

    EndDrawing();
}

static void PhysicsHandler(Vector2 *position, Vector2 *velocity, Player *player)
{
    velocity->y += GRAVITY;
    if (velocity->y > MAX_FALL)
    {
        velocity->y = MAX_FALL;
    }

    position->x += velocity->x;
    position->y += velocity->y;

    // ── FLOOR COLLISION ────────────────────────────────────────────────
    if (position->y + PLAYER_H >= GROUND_Y)
    {
        position->y = GROUND_Y - (float)PLAYER_H;
        velocity->y = 0.0f;
        player->onGround = true;
    }

    // ── SCREEN BOUNDARIES ──────────────────────────────────────────────
    if (position->x < 0)
    {
        position->x = 0.0f;
    }
    if (position->x + PLAYER_W > (float)SCREEN_W)
    {
        position->x = (float)(SCREEN_W - PLAYER_W);
    }
}

static void InputHandler(Vector2 *velocity, Player *player)
{
    if (IsKeyDown(KEY_RIGHT))
    {
        velocity->x = (float)PLAYER_SPD;
        player->facingRight = true;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        velocity->x = -(float)PLAYER_SPD;
        player->facingRight = false;
    }
    else
    {
        velocity->x = 0.0f;
    }

    if (player->stamina < JUMP_STAMINA_COST)
    {
        velocity->x *= STAMINA_MOVE_DRAIN;
    }

    if (IsKeyPressed(KEY_SPACE) && player->onGround)
    {
        velocity->y = JUMP_FORCE;
        velocity->y *= player->stamina < JUMP_STAMINA_COST ? STAMINA_JUMP_DRAIN : 1; // Has to be done before stamina is decremented
        player->onGround = false;
        UpdatePlayerStamina(player);
    }
}

static void UpdatePlayerStamina(Player *player)
{
    player->stamina -= JUMP_STAMINA_COST;
    player->stamina = player->stamina > 0 ? player->stamina : 0.0f; // ensure stamina does not go below 0
}

static void RegenerateStaminaFromFood(Player *player)
{
    player->stamina += STAMINA_FROM_FOOD;
    player->stamina = player->stamina > PLAYER_MAX_STAMINA ? PLAYER_MAX_STAMINA : player->stamina; // ensure stamina does not excede max
}

static bool TileSolid(const GameMap *m, int col, int row)
{
    if (col < 0 || col >= MAP_COLS)
    {
        return true;
    }
    if (row < 0 || row >= MAP_ROWS)
    {
        return true;
    }
    return m->tiles[row][col] != TILE_EMPTY;
}

static void MapInit(GameMap *m)
{
    for (int r = 0; r < MAP_ROWS; r++)
    {
        for (int c = 0; c < MAP_COLS; c++)
        {
            m->tiles[r][c] = TILE_EMPTY;
        }
    }

    int floorRow = (MAP_ROWS * 4) / 5;
    for (int r = floorRow; r < MAP_ROWS; r++)
    {
        for (int c = 0; c < MAP_COLS; c++)
        {
            m->tiles[r][c] = TILE_EARTH;
        }
    }
}

static void MapDraw(const GameMap *m)
{
    for (int r = 0; r < MAP_ROWS; r++)
    {
        for (int c = 0; c < MAP_COLS; c++)
        {
            if (m->tiles[r][c] == TILE_EMPTY)
            {
                continue;
            }
            DrawRectangle(c * TILE_SIZE, r * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKBROWN);
        }
    }
}