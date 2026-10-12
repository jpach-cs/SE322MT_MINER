/*******************************************************************************************
*
*   Montana Tech Miner - SE322 Prototype
*   Based on RayLib 2D Challenge by Hans de Ruiter
*
********************************************************************************************/
#include "raylib.h"
#include <stdbool.h>

#define TILE_SIZE    10
#define SCREEN_W    480
#define SCREEN_H    270
#define MAP_COLS    (SCREEN_W / TILE_SIZE)   /* 48 */
#define MAP_ROWS    (SCREEN_H / TILE_SIZE)   /* 27 */

/* ── static const ────────────────────────────────────────────────────────*/
static const int   PLAYER_W   = TILE_SIZE;          /* 10 px – 1 tile wide   */
static const int   PLAYER_H   = TILE_SIZE * 3;      /* 30 px – 3 tiles tall  */
static const int   PLAYER_SPD = 2;                  /* pixels per frame      */
static const float GRAVITY    = 0.4f;               /* px / frame²           */
static const float JUMP_FORCE = -7.5f;              /* px / frame, upward    */
static const float MAX_FALL   = 9.0f;               /* must stay < TILE_SIZE */
static const float PLAYER_MAX_STAMINA = 100.0f;     /* maximum stamina value */
static const float JUMP_STAMINA_COST = 15.0f;       /* stamina cost per jump  */        
static const float STAMINA_FROM_FOOD = 40.0f;       /* stamina gained from food */
static const int   STAMINA_BAR_W = 100;             /* width of stamina bar in pixels */
static const int   STAMINA_BAR_H = 15;              /* height of stamina bar in pixels */
static const int   STAMINA_BAR_X = 15;              /* x position of stamina bar */
static const int   STAMINA_BAR_Y = 35;              /* y position of stamina bar */
static const int   STAMINA_BAR_TEXT_SIZE = 18;          /* border thickness of stamina bar */

typedef enum {
    TILE_EMPTY = 0,   /* air  – nothing drawn, player falls through */
    TILE_EARTH,        /* dirt – drawn as rectangle, solid ground     */
} TileType;

typedef struct {
    TileType tiles[MAP_ROWS][MAP_COLS];
} GameMap;

typedef struct {
    float stamina;   /* current stamina value */
} Player;

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

static void UpdatePlayerStamina(Player *p, float change) {
    if (change < 0.0f) {
        if (p->stamina > (change * -1.0f)) p->stamina += change;
    } else {
        p->stamina += change;
    }

    if (p->stamina > PLAYER_MAX_STAMINA) p->stamina = PLAYER_MAX_STAMINA;
}

static void RegenerateStaminaFromFood(Player *P, Vector2 pos, GameMap *M) {
    UpdatePlayerStamina(P, STAMINA_FROM_FOOD);
}

void DrawStaminaBar(const Player *player) {
    DrawRectangle(STAMINA_BAR_X, STAMINA_BAR_Y, STAMINA_BAR_W, STAMINA_BAR_H, DARKGRAY);
    DrawRectangle(STAMINA_BAR_X, STAMINA_BAR_Y, (int)(STAMINA_BAR_W * (player->stamina / PLAYER_MAX_STAMINA)), STAMINA_BAR_H, BLUE);
    DrawRectangleLines(STAMINA_BAR_X, STAMINA_BAR_Y, STAMINA_BAR_W, STAMINA_BAR_H, WHITE);

    DrawText(TextFormat("stamina: %.0f / %.0f", player->stamina, PLAYER_MAX_STAMINA), STAMINA_BAR_X, STAMINA_BAR_Y + 20, STAMINA_BAR_TEXT_SIZE, WHITE);
}

int main(void)
{
    // Initialization
    const float groundY = (float)((MAP_ROWS * 4 / 5) * TILE_SIZE); /* = 210*/

    InitWindow(SCREEN_W, SCREEN_H, "Montana Tech Miner");

    Vector2 pos = {
        (float)(SCREEN_W / 2 - PLAYER_W / 2),
        groundY - (float)PLAYER_H
    };
    Vector2 vel         = { 0.0f, 0.0f };
    bool    onGround    = true;
    bool    facingRight = true;

    Player player;
    player.stamina = PLAYER_MAX_STAMINA;

    GameMap map;
    MapInit(&map);

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // ── INPUT ──────────────────────────────────────────────────────────
        float speedMultiplier = 1.0f;          /*multiplier for speed */
        float jumpMultiplier  = 1.0f;          /*multiplier for jump height */

        if (player.stamina < JUMP_STAMINA_COST) {
            speedMultiplier = 0.75f;  /* move slower when stamina is low */
            jumpMultiplier  = 0.5f;   /* jump lower when stamina is low */
        }

        if      (IsKeyDown(KEY_RIGHT)) { vel.x =  (float)PLAYER_SPD * speedMultiplier; facingRight = true;  }
        else if (IsKeyDown(KEY_LEFT))  { vel.x = -(float)PLAYER_SPD * speedMultiplier; facingRight = false; }
        else                             vel.x = 0.0f;

        if (IsKeyPressed(KEY_SPACE) && onGround) {
            vel.y    = JUMP_FORCE * jumpMultiplier;
            onGround = false;
            UpdatePlayerStamina(&player, -JUMP_STAMINA_COST);
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


        // ── DRAW ───────────────────────────────────────────────────────────
        BeginDrawing();

            ClearBackground((Color){ 18, 10, 5, 255 });

            /* temporary floor */
            MapDraw(&map);

            /* player placeholder – green = facing right, lime = facing left */
            DrawRectangle((int)pos.x, (int)pos.y, PLAYER_W, PLAYER_H, facingRight ? GREEN : LIME);
            /* stamina bar */
            DrawStaminaBar(&player);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();
    return 0;
}