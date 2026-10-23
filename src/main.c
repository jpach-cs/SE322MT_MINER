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

static const int player_W = TILE_SIZE;
static const int player_H = TILE_SIZE * 3;
static const int player_speed = 2;
static const float gravity = -0.4f;
static const float jump_force = 7.5f;
static const float max_fall = -9.0f;
static const float PLAYER_MAX_STAMINA = 100.0f;     /* maximum stamina value */
static const float JUMP_STAMINA_COST = 15.0f;       /* stamina cost per jump  */        
static const float STAMINA_FROM_FOOD = 40.0f;       /* stamina gained from food */
static const int   STAMINA_BAR_W = 100;             /* width of stamina bar in pixels */
static const int   STAMINA_BAR_H = 15;              /* height of stamina bar in pixels */
static const int   STAMINA_BAR_X = 15;              /* x position of stamina bar */
static const int   STAMINA_BAR_Y = 35;              /* y position of stamina bar */
static const int   STAMINA_BAR_TEXT_SIZE = 18;      /* border thickness of stamina bar */
static const float SPEED_PENALTY = 0.75f;           /* speed penalty when stamina is low */
static const float JUMP_PENALTY = 0.5f;             /* jump penalty when stamina is low */

typedef struct {
    Vector2 position;
    Vector2 velocity;
    bool onGround;
    bool facingRight;
    float stamina;
} Player;

typedef enum {
    TILE_EMPTY = 0,   /* air  - nothing drawn, player falls through */
    TILE_EARTH,       /* dirt - solid ground                        */
    TILE_FOOD         /* food - restores stamina when touched       */
} TileType;

typedef struct {
    TileType tiles[MAP_ROWS][MAP_COLS];
} GameMap;

static bool tileSolid(const GameMap* map, int col, int row) {
    if (col < 0  || col >= MAP_COLS || row < 0 || row >= MAP_ROWS) return true;
    return map->tiles[row][col] != TILE_EMPTY;
}

/* Map rows count upward from the bottom of the screen (row 0 is the floor). */
static Rectangle tileScreenRect(int col, int row) {
    return (Rectangle){ (float)(col * TILE_SIZE), (float)(SCREEN_H - (row * TILE_SIZE)),
                        (float)TILE_SIZE, (float)TILE_SIZE };
}

static Rectangle playerScreenRect(const Player* p) {
    return (Rectangle){ p->position.x, SCREEN_H - (p->position.y + player_H),
                        (float)player_W, (float)player_H };
}

static void mapInit(GameMap* map) {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            map->tiles[y][x] = (y < 5 ? TILE_EARTH : TILE_EMPTY);
        }
    }

    /* test food: two on the ground, one that needs a jump to reach */
    map->tiles[5][8]   = TILE_FOOD;
    map->tiles[5][40]  = TILE_FOOD;
    map->tiles[10][24] = TILE_FOOD;
}

static void drawMap(const GameMap* map) {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            if (map->tiles[y][x] == TILE_EMPTY) continue;
            DrawRectangleRec(tileScreenRect(x, y), map->tiles[y][x] == TILE_FOOD ? YELLOW : DARKBROWN);
        }
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

static void RegenerateStaminaFromFood(Player *p, GameMap *map) {
    Rectangle body = playerScreenRect(p);
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            if (map->tiles[y][x] != TILE_FOOD) continue;
            if (!CheckCollisionRecs(body, tileScreenRect(x, y))) continue;
            UpdatePlayerStamina(p, STAMINA_FROM_FOOD);
            map->tiles[y][x] = TILE_EMPTY;   /* food is consumed */
        }
    }
}

static void drawStaminaBar(const Player *player)
{
    float fill_width = (player->stamina / PLAYER_MAX_STAMINA) * STAMINA_BAR_W;

    //Create stamina bar background
    DrawRectangle(STAMINA_BAR_X, STAMINA_BAR_Y, STAMINA_BAR_W, STAMINA_BAR_H, DARKGRAY);
    // Create dynamic fill bar (on top of background)
    DrawRectangle(STAMINA_BAR_X, STAMINA_BAR_Y, fill_width, STAMINA_BAR_H, BLUE);
    // Create border around stamina bar (1px thick)
    DrawRectangleLines(STAMINA_BAR_X, STAMINA_BAR_Y, STAMINA_BAR_W, STAMINA_BAR_H, WHITE);

    DrawText(TextFormat("stamina: %.0f / %.0f", player->stamina, PLAYER_MAX_STAMINA), STAMINA_BAR_X, STAMINA_BAR_Y + 20, STAMINA_BAR_TEXT_SIZE, WHITE);
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
        .position = { (SCREEN_W / 2.0f) - (player_W / 2), groundY },
        .velocity = {0.0f, 0.0f},
        .onGround = true,
        .facingRight = true,
        .stamina = PLAYER_MAX_STAMINA
    };

    // Main game loop
    while (!WindowShouldClose())
    {
        // ── INPUT ──────────────────────────────────────────────────────────
        float speedMultiplier = 1.0f;          /*multiplier for speed */
        float jumpMultiplier  = 1.0f;          /*multiplier for jump height */

        if (player.stamina < JUMP_STAMINA_COST) {
            speedMultiplier = SPEED_PENALTY;  /* move slower when stamina is low */
            jumpMultiplier  = JUMP_PENALTY;   /* jump lower when stamina is low */
        }

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            player.velocity.x = player_speed * speedMultiplier;
            player.facingRight = true;
        } else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            player.velocity.x = -player_speed * speedMultiplier;
            player.facingRight = false;
        } else {
            player.velocity.x = 0;
        }

        // jump only when standing on the ground
        bool jumpPressed = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W);
        if (jumpPressed && player.onGround) {
            player.velocity.y = jump_force * jumpMultiplier;
            player.onGround = false;
            UpdatePlayerStamina(&player, -JUMP_STAMINA_COST);
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

        // ── FOOD ───────────────────────────────────────────────────────────
        RegenerateStaminaFromFood(&player, &map);

        // ── DRAW ───────────────────────────────────────────────────────────
        BeginDrawing();

        ClearBackground((Color){ 18, 10, 5, 255 });

        /* temporary floor */
        drawMap(&map);

        /* player placeholder – green = facing right, lime = facing left */
        DrawRectangleRec(playerScreenRect(&player), player.facingRight ? GREEN : LIME);

        drawStaminaBar(&player);

        EndDrawing();
    }

    // Deinit
    CloseWindow();
    return 0;
}
