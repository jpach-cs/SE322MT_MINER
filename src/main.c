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
#include <stdio.h>

#define TILE_SIZE 10
#define SCREEN_W 1680
#define SCREEN_H 1050
#define MAP_COLS (SCREEN_W / TILE_SIZE)
#define MAP_ROWS (SCREEN_H / TILE_SIZE)

#define PLAYER_MAX_STAMINA 1000.f
#define JUMP_STAMINA_COST 50.f 
#define STAMINA_FROM_FOOD 100.f

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

// all in one player
typedef struct{
    //stamina
    float stamina;
    Vector2 position;
} Player;


//Update player stamina function
static void DrawPlayerStamina(Player * P){
    // Draws box around stamina text
    DrawRectangle(15, 15, 650, 50, GRAY);
    DrawRectangle(15, 15, 650*P->stamina/PLAYER_MAX_STAMINA,50, RED );
    // draws text for stamina 
    char buffer [25];
    snprintf(buffer, 25, "Stamina: %3.1f/%3.1f", P->stamina, PLAYER_MAX_STAMINA);
    DrawText(buffer, 15, 15, 40, WHITE);
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

    Vector2 vel = { 0.0f, 0.0f };
    bool onGround = true;
    bool facingRight = true;

    Player P;
    P.stamina = PLAYER_MAX_STAMINA;
    P.position =  (Vector2){(SCREEN_W / 2.0f) - (player_W / 2), groundY}; 

    

    // Main game loop
    while (!WindowShouldClose())
    {
        // ── INPUT ──────────────────────────────────────────────────────────
        if (IsKeyDown(KEY_RIGHT)) {
            vel.x = player_speed;
            facingRight = true;
            P.stamina -= 0.25;
        } else if (IsKeyDown(KEY_LEFT)) {
            vel.x = -player_speed;
            facingRight = false;
            P.stamina -= 0.5;
        } else {
            vel.x = 0;
        }

        // jump only when standing on the ground
        if (IsKeyPressed(KEY_SPACE) && onGround) {
            vel.y = jump_force;
            onGround = false;
            P.stamina -= JUMP_STAMINA_COST;
            if (P.stamina < JUMP_STAMINA_COST){
                vel.y = jump_force * 0.5;
                
            }

        }
        //Regain stamina 
        if (IsKeyPressed(KEY_H)){
            P.stamina += STAMINA_FROM_FOOD;
        }

        // ── PHYSICS ────────────────────────────────────────────────────────
        vel.y += gravity;
        if (vel.y < max_fall) vel.y = max_fall;
        P.position = Vector2Add(P.position, vel);

        // ── FLOOR COLLISION ────────────────────────────────────────────────
        if (P.position.y <= groundY) {
            P.position.y = groundY;
            vel.y = 0.0f;
            onGround        = true;
        }

        // ── SCREEN BOUNDARIES ──────────────────────────────────────────────
        P.position.x = Clamp(P.position.x, 0, SCREEN_W - player_W);
        P.stamina = Clamp(P.stamina, 0, PLAYER_MAX_STAMINA);

        // ── DRAW ───────────────────────────────────────────────────────────
        BeginDrawing();

        ClearBackground((Color){ 18, 10, 5, 255 });

        /* temporary floor */
        drawMap(&map);

        /* player placeholder – green = facing right, lime = facing left */
        DrawRectangle((int)P.position.x, (int)SCREEN_H - (P.position.y + player_H), player_W, player_H, facingRight ? GREEN : LIME);

        DrawPlayerStamina(&P);
        EndDrawing();
    }

    // Deinit
    CloseWindow();
    return 0;
}
