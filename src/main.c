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
#define MAP_COLS    (SCREEN_W / TILE_SIZE)
#define MAP_ROWS    (SCREEN_H / TILE_SIZE)
#define PLAYER_MAX_STAMINA  100.0f
#define JUMP_STAMINA_COST    15.0f
#define STAMINA_FROM_FOOD    40.0f

#define PLAYER_SPEED_PENALTY      0.75f        
#define PLAYER_JUMP_PENALTY      0.5f       
#define DEBUG_FONT_SIZE      20
#define DEBUG_TEXT_X         15
#define DEBUG_TEXT_Y         15

#define FOOD_TEST_COL 1

static const int   PLAYER_W   = TILE_SIZE;     
static const int   PLAYER_H   = TILE_SIZE * 3;  
static const int   PLAYER_SPD = 2;           
static const float GRAVITY    = 0.4f;         
static const float JUMP_FORCE = -7.5f;
static const float MAX_FALL   = 9.0f;  

/* ── Tile system ─────────────────────────────────────────────────────────── */

typedef enum {
    TILE_EMPTY = 0,   /* air  – nothing drawn, player falls through */
    TILE_EARTH,        /* dirt – drawn as rectangle, solid ground     */
    TILE_FOOD           /*regenerates player stamina*/
} TileType;

typedef struct {
    TileType tiles[MAP_ROWS][MAP_COLS];
} GameMap;

typedef struct {
    Vector2  pos;
    Vector2  vel;
    bool onGround;
    bool facingRight;
    float stamina;
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

    /* step 3: Tests food tile */
     m->tiles[floorRow - 1][FOOD_TEST_COL] = TILE_FOOD;
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

static void UpdatePlayerStamina(Player *p)
{
    if (p->stamina >= JUMP_STAMINA_COST) {
        /* enough stamina: full jump, pay the cost */
        p->vel.y    = JUMP_FORCE;
        p->stamina -= JUMP_STAMINA_COST;
    } else {
        /* low stamina: weak jump, sets to 0 */
        p->vel.y    = JUMP_FORCE * PLAYER_JUMP_PENALTY;
        p->stamina  = 0.0f;
    }
}

static void RegenerateStaminaFromFood(Player *p, const GameMap *m)
{
    int col = (int)((p->pos.x + PLAYER_W / 2) / TILE_SIZE);   /* middle of the player */
    int row = (int)((p->pos.y + PLAYER_H - 1) / TILE_SIZE);   /* the player's feet    */

    if (m->tiles[row][col] == TILE_FOOD) {
        p->stamina += STAMINA_FROM_FOOD;
        if (p->stamina > PLAYER_MAX_STAMINA) {
            p->stamina = PLAYER_MAX_STAMINA;
        }
    }
}

static void DrawDebugStamina(const Player *p)
{
    DrawText(TextFormat("Stamina: %.1f / %.1f", p->stamina, PLAYER_MAX_STAMINA),
             DEBUG_TEXT_X, DEBUG_TEXT_Y, DEBUG_FONT_SIZE, WHITE);
}

int main(void)
{

    const float groundY = (float)((MAP_ROWS * 4 / 5) * TILE_SIZE);

    InitWindow(SCREEN_W, SCREEN_H, "Montana Tech Miner");

    GameMap map;
    MapInit(&map);

    Player player = {
        .pos         = { (float)(SCREEN_W / 2 - PLAYER_W / 2), groundY - (float)PLAYER_H },
        .vel         = { 0.0f, 0.0f },
        .onGround    = true,
        .facingRight = true,
        .stamina     = PLAYER_MAX_STAMINA
    };

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // ── INPUT ──────────────────────────────────────────────────────────
        if      (IsKeyDown(KEY_RIGHT)) { player.vel.x =  (float)PLAYER_SPD; player.facingRight = true;  }
        else if (IsKeyDown(KEY_LEFT))  { player.vel.x = -(float)PLAYER_SPD; player.facingRight = false; }
        else                             player.vel.x = 0.0f;
 
        if (player.stamina < JUMP_STAMINA_COST) {
            player.vel.x *= PLAYER_SPEED_PENALTY;
        }

        if ((IsKeyPressed(KEY_SPACE) && player.onGround) || (IsKeyPressed(KEY_UP) && player.onGround)) {
            UpdatePlayerStamina(&player);
            player.onGround = false;
        }
 
        // ── PHYSICS ────────────────────────────────────────────────────────
        player.vel.y += GRAVITY;
        if (player.vel.y > MAX_FALL) player.vel.y = MAX_FALL;
 
        player.pos.x += player.vel.x;
        player.pos.y += player.vel.y;
 
        // ── SCREEN BOUNDARIES ──────────────────────────────────────────────
        if (player.pos.x < 0)                                   player.pos.x = 0.0f;
        if (player.pos.x + PLAYER_W > (float)SCREEN_W)          player.pos.x = (float)(SCREEN_W - PLAYER_W);
 
        // ── FLOOR COLLISION ────────────────────────────────────────────────
        if (player.pos.y + PLAYER_H >= groundY) {
            player.pos.y    = groundY - (float)PLAYER_H;
            player.vel.y    = 0.0f;
            player.onGround = true;
        }
        
        RegenerateStaminaFromFood(&player, &map);

        // ── DRAW ───────────────────────────────────────────────────────────
        BeginDrawing();
 
            ClearBackground((Color){ 18, 10, 5, 255 });
 
            /* tile map floor */
            MapDraw(&map);
 
            /* player placeholder – green = facing right, lime = facing left */
            DrawRectangle((int)player.pos.x, (int)player.pos.y,
                          PLAYER_W, PLAYER_H,
                          player.facingRight ? GREEN : LIME);

             DrawDebugStamina(&player);
 
        EndDrawing();
    }

    // De-Initialization
    CloseWindow();
    return 0;
}

// Brock Harman
// ESOF 322 Fall 2026
// Programming Assignment #3
// I declare that I am the author of this work, take full responsibility for it, and have disclosed any material external assistance.
// I used Claude to walkthrough the assignment while I wrote, tested, and verified the submitted implementation myself.