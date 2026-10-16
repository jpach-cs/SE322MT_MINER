/*******************************************************************************************
*
*   Montana Tech Miner - SE322 Prototype
*   Based on RayLib 2D Challenge by Hans de Ruiter
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include "player.h"
#define TILE_SIZE    10
#define SCREEN_W    480
#define SCREEN_H    270
#define MAP_COLS    (SCREEN_W / TILE_SIZE)   /* 48 */
#define MAP_ROWS    (SCREEN_H / TILE_SIZE)   /* 27 */

static const int   PLAYER_WIDTH   = TILE_SIZE;        /* 10 px – 1 tile wide   */
static const int   PLAYER_HEIGHT   = TILE_SIZE * 3;    /* 30 px – 3 tiles tall  */
static const int   PLAYER_SPD = 2;                /* pixels per frame      */
static const float GRAVITY    = 0.4f;             /* px / frame²           */
static const float JUMP_FORCE = -7.5f;            /* px / frame, upward    */
static const float MAX_FALL   = 9.0f;             /* must stay < TILE_SIZE */

static const int STAMINA_BAR_WIDTH = 100;
static const int STAMINA_BAR_HEIGHT = 10;
static const int STAMINA_BAR_X = 15;
static const int STAMINA_BAR_Y = 35;
static const int STAMINA_TEXT_X = 15;
static const int STAMINA_TEXT_Y = 5;

static void drawStaminaBar(Player player, int width, int height, int posX, int posY, Color outlineColor, Color fillColor){
    DrawRectangle(posX, posY, width, height, outlineColor);

    int fillWidth = (player.stamina / MAX_STAMINA) * width;
    DrawRectangle(posX, posY, fillWidth, height, fillColor);
}

typedef enum {
    TILE_EMPTY = 0,   /* air  – nothing drawn, player falls through */
    TILE_EARTH        /* dirt – drawn as rectangle, solid ground     */
} TileType;

typedef struct {
    TileType tiles[MAP_ROWS][MAP_COLS];
} GameMap;

// commented out unusedfunction
// static bool TileSolid(const GameMap *m, int col, int row)
// {
//     if (col < 0 || col >= MAP_COLS) return true;
//     if (row < 0 || row >= MAP_ROWS) return true;
//     return m->tiles[row][col] != TILE_EMPTY;
// }

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

bool isTextureValid(const Texture2D *texture) {
    return texture->id > 0;
}


int main(void)
{
    // Initialization -- commented out unsed variables
    // const int   minerSpeed = 5;
    // const float gravity    = 0.5f;
    // const float jumpForce  = -12.0f;

    const float groundY = (float)((MAP_ROWS * 4 / 5) * TILE_SIZE);   /* = 210 */

    InitWindow(SCREEN_W, SCREEN_H, "Montana Tech Miner");

    const char *filename = "resources\\miner_walk.PNG"; // check "resources\\thomas.PNG"
    Texture2D miner = LoadTexture(filename);
    if (!isTextureValid(&miner)) {
        while (!WindowShouldClose()) {
            BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText(TextFormat("ERROR: Couldn't load %s.", filename),
                         20, 20, 20, BLACK);
            EndDrawing();
        }
        return 10;
    }

    unsigned numFrames  = 4; // 8 for thomas - line 29
    int      frameWidth = miner.width / numFrames;
    Rectangle frameRec  = { 0.0f, 0.0f, (float)frameWidth, (float)miner.height };

    Player player = {
        .stamina = MAX_STAMINA,
        .position = {
            (float)(SCREEN_W / 2 - PLAYER_WIDTH / 2),
            groundY - (float)PLAYER_HEIGHT
        },
        .velocity = {0.0f, 0.0f}
    };
    bool    onGround    = true;
    bool    facingRight = true;

    unsigned frameDelay        = 5;
    unsigned frameDelayCounter = 0;
    unsigned frameIndex        = 0;

    SetTargetFPS(60);

    GameMap map;
    MapInit(&map);
    
    // Main game loop
    while (!WindowShouldClose())
    {   

        
        // ── INPUT ──────────────────────────────────────────────────────────
        if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)){
            if(player.stamina == 0){
                player.velocity.x =  (float)PLAYER_SPD * 0.75f; facingRight = true;
                
            }else{
                player.velocity.x =  (float)PLAYER_SPD; facingRight = true;
            }
        }
        else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)){
            if(player.stamina == 0){
                player.velocity.x = -(float)PLAYER_SPD * 0.75f;
                facingRight = false;
            }else{
                player.velocity.x = -(float)PLAYER_SPD;
                facingRight = false;
            }       
        }
        else{
            player.velocity.x = 0.0f;
        }
        
        if (((player.stamina > 0) && (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyDown(KEY_W)) && onGround)){
            player.velocity.y = JUMP_FORCE;
            onGround = false;
            if(player.stamina > 0){
                player.stamina -= JUMP_STAMINA_COST;
            }
        }
        if (((player.stamina <= 0) && (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyDown(KEY_W)) && onGround)){
            player.velocity.y = JUMP_FORCE * 0.5f;
            onGround = false;
            // player.stamina -= JUMP_STAMINA_COST;
        }
        // Testing cheat
        if(IsKeyPressed(KEY_R)){
            if(player.stamina + STAMINA_FROM_FOOD > 100){
                player.stamina = 100;
            }else{
                player.stamina += STAMINA_FROM_FOOD;
            }
        }


        // ── PHYSICS ────────────────────────────────────────────────────────
        player.velocity.y += GRAVITY;
        if (player.velocity.y > MAX_FALL){
            player.velocity.y = MAX_FALL;
        }

        player.position.x += player.velocity.x;
        player.position.y += player.velocity.y;

        // ── FLOOR COLLISION ────────────────────────────────────────────────
        if (player.position.y + PLAYER_HEIGHT >= groundY) {
            player.position.y    = groundY - (float)PLAYER_HEIGHT;
            player.velocity.y    = 0.0f;
            onGround = true;
        }

        // ── SCREEN BOUNDARIES ──────────────────────────────────────────────
        if (player.position.x < 0)                                   player.position.x = 0.0f;
        if (player.position.x + PLAYER_WIDTH > (float)SCREEN_W)          player.position.x = (float)(SCREEN_W - PLAYER_WIDTH);

        // ── ANIMATION ──────────────────────────────────────────────────────
        bool minerMoving = (player.velocity.x != 0.0f);

        if (!onGround) {
            frameIndex = 1;                    // in the air: frozen frame
        } else {
            ++frameDelayCounter;
            if (frameDelayCounter > frameDelay) {
                frameDelayCounter = 0;
                if (minerMoving) {
                    ++frameIndex;
                    frameIndex %= numFrames;   // 0 → 1 → 2 → 3 → 0 ...
                } else {
                    frameIndex = 0;            // standing: first frame
                }
            }
        }

        // preserve direction (sign of frameRec.width) when changing frame
        float dir      = (frameRec.width >= 0) ? 1.0f : -1.0f;
        frameRec.x     = (float)frameWidth * frameIndex;
        frameRec.width = dir * (float)frameWidth;

        // ── DRAW ───────────────────────────────────────────────────────────
        BeginDrawing();
            DrawRectangleLines(STAMINA_BAR_X - 1, STAMINA_BAR_Y - 1, STAMINA_BAR_WIDTH + 2, STAMINA_BAR_HEIGHT + 2, WHITE);
            drawStaminaBar(player, STAMINA_BAR_WIDTH,STAMINA_BAR_HEIGHT,STAMINA_BAR_X,STAMINA_BAR_Y,DARKGRAY,BLUE);
            ClearBackground((Color){ 18, 10, 5, 255 });
            //Syntax: DrawText(const char *text, int posX, int posY, int fontSize, Color color);
            DrawText(TextFormat("Stamina: %.1f / 100.0", player.stamina),STAMINA_TEXT_X,STAMINA_TEXT_Y, 18 ,WHITE);
            /* temporary floor */
            MapDraw(&map);

            /* player placeholder – green = facing right, lime = facing left */
            DrawRectangle((int)player.position.x, (int)player.position.y,
                        PLAYER_WIDTH, PLAYER_HEIGHT,
                        facingRight ? GREEN : LIME);

        EndDrawing();

    }

    // De-Initialization
    UnloadTexture(miner);
    CloseWindow();
    return 0;
}