# Assignment 1 – Constants, Defines, and Player Placeholder

## What You Will Build

You will modify the original `main.c` so that the game window changes
from 800×450 to 480×270, the player sprite is replaced by a colored
rectangle, and all magic numbers are replaced by named constants.

When you are done the game must:

```
✓ open a 480×270 window
✓ show a green rectangle that moves left and right
✓ show a lime rectangle when facing left
✓ allow the player to jump with SPACE
✓ keep the player inside the screen boundaries
✓ keep the player above the temporary floor line
✓ compile without warnings
```

---

## Step 1 – Create Your Branch

Open your terminal in the project folder and run:

```bash
git checkout main
git pull origin main
git checkout -b feature/smith-constants-placeholder
```

You are now on your own branch. Your changes will not affect anyone else.

---

## Step 2 – What to Remove

Open `src/main.c` and remove the following blocks entirely:

```
- the isTextureValid() function
- const char *filename = ...
- Texture2D miner = LoadTexture(...)
- the if (!isTextureValid()) error block
- unsigned numFrames  = ...
- int      frameWidth = ...
- Rectangle frameRec  = ...
- unsigned frameDelay        = ...
- unsigned frameDelayCounter = ...
- unsigned frameIndex        = ...
- the entire animation section inside the loop
- the frameRec direction logic (dir, frameRec.x, frameRec.width)
- DrawTextureRec(...)
- UnloadTexture(...)
- the #include "raymath.h" line (no longer needed)
```

---

## Step 3 – What to Add at the Top of the File

Add these lines at the top, after `#include "raylib.h"`:

```c
#include <stdbool.h>
```

Then add the constants block below the include lines.
Read the tutorial section before writing this – it explains
why each constant is written the way it is.

```c
/* ── #define only where required by the compiler ────────────────────────────
   These values are used to declare array sizes inside a struct in Assignment 2.
   C requires array dimensions to be compile-time constants.
   static const int does NOT qualify as a compile-time constant in C.        */

#define TILE_SIZE    10
#define SCREEN_W    480
#define SCREEN_H    270
#define MAP_COLS    (SCREEN_W / TILE_SIZE)   /* 48 */
#define MAP_ROWS    (SCREEN_H / TILE_SIZE)   /* 27 */

/* ── static const for everything else ───────────────────────────────────────
   These values are only used in runtime calculations.
   static const gives them a type, a name visible in the debugger,
   and limits their scope to this file.                                       */

static const int   PLAYER_W   = TILE_SIZE;        /* 10 px – 1 tile wide   */
static const int   PLAYER_H   = TILE_SIZE * 3;    /* 30 px – 3 tiles tall  */
static const int   PLAYER_SPD = 2;                /* pixels per frame      */
static const float GRAVITY    = 0.4f;             /* px / frame²           */
static const float JUMP_FORCE = -7.5f;            /* px / frame, upward    */
static const float MAX_FALL   = 9.0f;             /* must stay < TILE_SIZE */
```

---

## Step 4 – What to Change Inside main()

### 4a – Window size

```c
/* remove this */
const int screenWidth  = 800;
const int screenHeight = 450;

/* add this */
const float groundY = (float)((MAP_ROWS * 4 / 5) * TILE_SIZE);   /* = 210 */
```

Change `InitWindow` to:

```c
InitWindow(SCREEN_W, SCREEN_H, "Montana Tech Miner");
```

### 4b – Player starting position

```c
/* remove this */
Vector2 minerPosition = {
    screenWidth / 2.0f,
    groundY - miner.height
};
Vector2 minerVelocity = { 0.0f, 0.0f };
bool onGround = true;

/* add this */
Vector2 pos = {
    (float)(SCREEN_W / 2 - PLAYER_W / 2),
    groundY - (float)PLAYER_H
};
Vector2 vel         = { 0.0f, 0.0f };
bool    onGround    = true;
bool    facingRight = true;
```

### 4c – Input section

```c
/* remove this */
if (IsKeyDown(KEY_RIGHT)) {
    minerVelocity.x = minerSpeed;
    if (frameRec.width < 0) frameRec.width = -frameRec.width;
} else if (IsKeyDown(KEY_LEFT)) {
    minerVelocity.x = -minerSpeed;
    if (frameRec.width > 0) frameRec.width = -frameRec.width;
} else {
    minerVelocity.x = 0;
}

if (IsKeyPressed(KEY_SPACE) && onGround) {
    minerVelocity.y = jumpForce;
    onGround = false;
}

/* add this */
if      (IsKeyDown(KEY_RIGHT)) { vel.x =  (float)PLAYER_SPD; facingRight = true;  }
else if (IsKeyDown(KEY_LEFT))  { vel.x = -(float)PLAYER_SPD; facingRight = false; }
else                             vel.x = 0.0f;

if (IsKeyPressed(KEY_SPACE) && onGround) {
    vel.y    = JUMP_FORCE;
    onGround = false;
}
```

### 4d – Physics section

```c
/* remove this */
minerVelocity.y += gravity;
minerPosition    = Vector2Add(minerPosition, minerVelocity);

/* add this */
vel.y += GRAVITY;
if (vel.y > MAX_FALL) vel.y = MAX_FALL;

pos.x += vel.x;
pos.y += vel.y;
```

### 4e – Screen boundaries

```c
/* remove this */
if (minerPosition.x < 0)
    minerPosition.x = 0;
if (minerPosition.x > screenWidth - frameWidth)
    minerPosition.x = (float)(screenWidth - frameWidth);

/* add this */
if (pos.x < 0)                                   pos.x = 0.0f;
if (pos.x + PLAYER_W > (float)SCREEN_W)          pos.x = (float)(SCREEN_W - PLAYER_W);
```

### 4f – Floor collision

```c
/* remove this */
if (minerPosition.y + miner.height >= groundY) {
    minerPosition.y = groundY - miner.height;
    minerVelocity.y = 0.0f;
    onGround        = true;
}

/* add this */
if (pos.y + PLAYER_H >= groundY) {
    pos.y    = groundY - (float)PLAYER_H;
    vel.y    = 0.0f;
    onGround = true;
}
```

### 4g – Draw section

```c
/* remove this */
BeginDrawing();
    ClearBackground((Color){ 30, 20, 10, 255 });
    DrawRectangle(0, (int)groundY,
                  screenWidth, screenHeight - (int)groundY,
                  DARKBROWN);
    DrawTextureRec(miner, frameRec, minerPosition, WHITE);
EndDrawing();

/* add this */
BeginDrawing();

    ClearBackground((Color){ 18, 10, 5, 255 });

    /* temporary floor */
    DrawRectangle(0, (int)groundY,
                  SCREEN_W, SCREEN_H - (int)groundY,
                  DARKBROWN);

    /* player placeholder – green = facing right, lime = facing left */
    DrawRectangle((int)pos.x, (int)pos.y,
                  PLAYER_W, PLAYER_H,
                  facingRight ? GREEN : LIME);

EndDrawing();
```

---

## Step 5 – Compile and Test

In your terminal:

```bash
make
bin/game.exe
```

Check that:

```
□ window opens at 480×270
□ green rectangle appears on the floor
□ LEFT arrow moves left, rectangle turns lime
□ RIGHT arrow moves right, rectangle turns green
□ SPACE makes the player jump
□ player cannot walk off screen edges
□ player lands back on the floor after jumping
```

---

## Step 6 – Commit and Push

```bash
git add src/main.c
git commit -m "feat: replace sprite with placeholder, add defines and constants"
git push origin feature/smith-constants-placeholder
```

---

## Acceptance Criteria

Your assignment is complete when all of the following are true:

```
□ window size is 480×270
□ TILE_SIZE, SCREEN_W, SCREEN_H, MAP_COLS, MAP_ROWS are #define
□ PLAYER_W, PLAYER_H, PLAYER_SPD, GRAVITY, JUMP_FORCE, MAX_FALL
  are static const with correct types
□ no Texture2D, no LoadTexture, no DrawTextureRec anywhere in the file
□ no magic numbers (800, 450, 5, 0.5f, -12.0f) anywhere in the file
□ facingRight bool exists and controls the rectangle color
□ MAX_FALL cap is applied every frame before movement
□ code compiles with no warnings
□ branch is pushed to GitHub
```

---

---

# Tutorial 1 – Why We Do This and How It Works

## Why Named Constants Instead of Numbers

Look at the original code:

```c
const int screenWidth  = 800;
const int screenHeight = 450;
const int minerSpeed   = 5;
const float gravity    = 0.5f;
const float jumpForce  = -12.0f;
const float groundY    = screenHeight - 80.0f;
```

Now imagine you want to change the window size from 800×450 to 480×270.
You change `screenWidth` and `screenHeight`. But `groundY` depends on
`screenHeight`. And the floor tiles in Assignment 2 will depend on
`SCREEN_H`. And the map array dimensions will depend on `SCREEN_W`
divided by `TILE_SIZE`.

One change should produce a correct result everywhere automatically.
Named constants make that possible. Magic numbers make it impossible.

```c
/* magic number – what does 80 mean? */
const float groundY = screenHeight - 80.0f;

/* named constant – the intention is clear */
const float groundY = (float)((MAP_ROWS * 4 / 5) * TILE_SIZE);
```

---

## Why Some Constants Are #define and Others Are static const

In C there are two ways to name a constant value:

```c
#define  TILE_SIZE   10          /* preprocessor – replaced before compilation */
static const int PLAYER_W = 10; /* compiler     – a real typed variable        */
```

The preprocessor runs before the compiler. It finds every occurrence
of `TILE_SIZE` in the file and replaces it with `10`. The compiler
never sees the word `TILE_SIZE` – only the number `10`.

`static const int` is a real variable with a type and an address
in memory. The compiler sees it. The debugger sees it.

**The rule in C:**

```
array dimensions inside a struct MUST be compile-time constants
static const int does NOT count as a compile-time constant in C

this fails in C:
static const int MAP_ROWS = 27;
TileType tiles[MAP_ROWS][MAP_COLS];   // error: variable length array

this works:
#define MAP_ROWS 27
TileType tiles[MAP_ROWS][MAP_COLS];   // ok
```

This is one of the most important differences between C and C++.
In C++ a `const int` is a compile-time constant. In C it is not.

**Practical rule for this project:**

```
used as an array size or struct field dimension → #define
used only in runtime calculations               → static const
```

---

## Why We Change the Resolution to 480×270

The original game runs at 800×450. We change it to 480×270 for one reason:

```
480 / 10 = 48   (MAP_COLS – exactly 48 tiles wide)
270 / 10 = 27   (MAP_ROWS – exactly 27 tiles tall)
```

A tile is 10×10 pixels. The map fills the entire screen with no
remainder. Every position in the game lines up cleanly with the grid.

480×270 is also exactly one quarter of 1920×1080 (full HD).
This will matter in a later assignment when we scale the game
up to fill a large monitor using a render target.

---

## How the Game Loop Works and Why It Gives You 60 FPS

Your game runs on a single thread. The CPU does one thing at a time:

```
[INPUT] → [PHYSICS] → [COLLISION] → [DRAW] → [SLEEP] → [INPUT] → ...
```

One complete pass = one frame.

You tell raylib you want 60 frames per second:

```c
SetTargetFPS(60);
```

This means one frame must take exactly:

```
1 / 60 = 0.01667 seconds = 16.67 milliseconds
```

`EndDrawing()` measures how long the frame took and sleeps
for whatever time remains:

```
your calculations took  2ms → EndDrawing sleeps 14.67ms → total 16.67ms ✓
your calculations took 10ms → EndDrawing sleeps  6.67ms → total 16.67ms ✓
your calculations took 20ms → EndDrawing sleeps  0ms    → total 20ms    ✗
```

If your calculations take longer than 16.67ms the frame rate drops.
`EndDrawing()` cannot sleep negative time.

---

## What Happens Between BeginDrawing and EndDrawing

```c
BeginDrawing();        // GPU: lock the back buffer for writing

    ClearBackground(); // paint the entire screen one solid color
                       // without this every frame draws ON TOP
                       // of the previous frame – you get a trail

    MapDraw(&map);     // draw all tiles

    DrawRectangle();   // draw the player

EndDrawing();          // GPU: swap buffers – show the finished frame
                       // then sleep until 16.67ms has passed
```

Raylib uses two buffers internally:

```
buffer A – shown on screen right now
buffer B – being drawn into right now

EndDrawing() swaps them:
buffer B becomes A  (user sees the finished frame)
buffer A becomes B  (next frame will be drawn here)
```

The user only ever sees a complete frame. They never see a frame
that is half drawn. This technique is called double buffering.

---

## Why MAX_FALL Must Be Less Than TILE_SIZE

A tile is 10 pixels tall. The collision check runs once per frame.

If the player falls 15 pixels in one frame they can pass completely
through a 10-pixel tile between two frames:

```
frame 1: player is 3px above the tile  → no collision detected
frame 2: player is 12px below the tile → no collision detected
                                          player has passed through
```

`MAX_FALL = 9.0f` guarantees the player can never move more than
9 pixels vertically in one frame. One tile is 10 pixels.
The player cannot skip over a tile.

```c
vel.y += GRAVITY;
if (vel.y > MAX_FALL) vel.y = MAX_FALL;   // cap before movement
pos.y += vel.y;                            // then move
```

This must happen in this order every frame without exception.

---

---

# Assignment 2 – Tile Map: EMPTY and EARTH

## What You Will Build

You will add a tile map to the game. The map is a two-dimensional
array of tiles. Each tile is either empty air or solid earth.
The floor will be drawn as brown tiles instead of one rectangle.
The old `groundY` collision stays active – tile collision comes
in Assignment 3.

When you are done the game must:

```
✓ display a floor made of brown tile rectangles
✓ display empty black space above the floor
✓ player still stands on the floor and can jump
✓ compile without warnings
```

---

## Step 1 – Create Your Branch From Assignment 1

You must complete Assignment 1 before starting this.
Do NOT merge yet. Do NOT touch main.

Create the new branch directly from your Assignment 1 branch:

```bash
git checkout feature/smith-constants-placeholder
git checkout -b feature/tilemap
```

Your `feature/tilemap` branch now starts exactly where
`feature/smith-constants-placeholder` ended.
All your Assignment 1 changes are already here.

```
main                                  ← untouched
  └── feature/smith-constants-placeholder   ← Assignment 1 complete
        └── feature/tilemap           ← you are here
```

You will open a Pull Request only after both assignments are complete.
The Pull Request will be reviewed in class before anything is merged.

---

## Step 2 – Add the Tile System Before main()

Add this block after the `static const` constants and before `main()`.
Add it in this exact order: enum first, then struct, then functions.

### 2a – The TileType enum

```c
typedef enum {
    TILE_EMPTY = 0,   /* air  – nothing drawn, player falls through */
    TILE_EARTH        /* dirt – drawn as rectangle, solid ground     */
} TileType;
```

### 2b – The GameMap struct

```c
typedef struct {
    TileType tiles[MAP_ROWS][MAP_COLS];
} GameMap;
```

`MAP_ROWS` and `MAP_COLS` must be `#define` values.
This is why they cannot be `static const int` – see Tutorial 1.

### 2c – TileSolid helper function

```c
static bool TileSolid(const GameMap *m, int col, int row)
{
    if (col < 0 || col >= MAP_COLS) return true;
    if (row < 0 || row >= MAP_ROWS) return true;
    return m->tiles[row][col] != TILE_EMPTY;
}
```

This function answers one question: is the tile at (col, row) solid?
It returns true if the coordinates are outside the map – the player
cannot leave the map boundaries.
You will not call this function in Assignment 2. It is prepared
for Assignment 3.

### 2d – MapInit function

```c
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
```

`MapInit` is called once before the game loop.
It must not be called inside the loop – see Tutorial 2.

### 2e – MapDraw function

```c
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
```

---

## Step 3 – What to Change Inside main()

### 3a – Declare and initialize the map before the game loop

Add these two lines after `InitWindow` and before `SetTargetFPS`:

```c
GameMap map;
MapInit(&map);
```

### 3b – Replace the temporary floor rectangle in the draw section

```c
/* remove this */
DrawRectangle(0, (int)groundY,
              SCREEN_W, SCREEN_H - (int)groundY,
              DARKBROWN);

/* add this */
MapDraw(&map);
```

The `groundY` variable and the floor collision block stay exactly
as they are. Do not remove them. That is Assignment 3.

---

## Step 4 – Compile and Test

```bash
make
bin/game.exe
```

Check that:

```
□ floor appears as brown tile rectangles
□ dark background above the floor
□ player rectangle sits on top of the tiles
□ player can still jump and land correctly
□ no visual difference in gameplay from Assignment 1
   (only the floor looks different – tiles instead of one rectangle)
```

---

## Step 5 – Commit and Push

```bash
git add src/main.c
git commit -m "feat: add tile map with EMPTY and EARTH types"
git push origin feature/smith-tilemap
```

Your branch is now on GitHub but nothing has been merged anywhere.

```
main                                        ← untouched
  └── feature/smith-constants-placeholder   ← Assignment 1 complete
        └── feature/smith-tilemap           ← Assignment 2 complete
```

**Do NOT open a Pull Request yet.**

Wait for the instructor to announce that the class review session
is starting. Pull Requests are opened together, in class, so that
everyone can see the process at the same time.

When the instructor gives the signal:

1. Go to github.com/jpach-cs/SE322MT_MINER
2. Click "Compare and pull request" next to your branch
3. Set the target like this:

```
base:    main
compare: feature/smith-tilemap
title:   "Assignment 1+2 – [your last name]"
```

4. In the description write:
   - what you changed in Assignment 1
   - what you changed in Assignment 2
   - anything that did not work as expected

5. Submit the Pull Request and wait for review.

Do NOT merge. The instructor and reviewer merge after the class review.

---

## Acceptance Criteria

```
□ TileType enum exists with TILE_EMPTY and TILE_EARTH
□ GameMap struct exists with tiles[MAP_ROWS][MAP_COLS]
□ TileSolid() function exists and handles out-of-bounds correctly
□ MapInit() fills rows 0-20 with TILE_EMPTY
□ MapInit() fills rows 21-26 with TILE_EARTH
□ MapDraw() draws only TILE_EARTH tiles, skips TILE_EMPTY
□ MapInit() is called ONCE before the game loop
□ MapDraw() is called ONCE inside the game loop inside BeginDrawing
□ groundY variable and floor collision are still present and unchanged
□ code compiles with no warnings
```

---

---

# Tutorial 2 – Why We Build the Map This Way

## Why a Two-Dimensional Array

The game world is a grid. Every position in the grid has a type.
A two-dimensional array maps directly onto this idea:

```
map.tiles[row][col]

map.tiles[0][0]   = top-left corner of the world
map.tiles[26][47] = bottom-right corner of the world
map.tiles[21][0]  = first tile of the floor, left edge
```

Row comes first, column second. This matches how memory is laid out
and how nested loops naturally traverse the grid:

```c
for (int r = 0; r < MAP_ROWS; r++)       /* rows: top to bottom   */
    for (int c = 0; c < MAP_COLS; c++)   /* columns: left to right */
```

---

## Why TileType Is an enum and Not an int

You could store tile types as plain integers:

```c
int tiles[MAP_ROWS][MAP_COLS];
/* 0 = empty, 1 = earth, 2 = ore, 3 = food */
```

This works but creates a maintenance problem. Every time you read
the code you need to remember what 0, 1, 2, and 3 mean.
A bug where you write `2` instead of `1` produces no compiler warning.

An enum names the values:

```c
typedef enum {
    TILE_EMPTY = 0,
    TILE_EARTH,
    TILE_ORE,
    TILE_FOOD
} TileType;
```

Now the compiler knows what values are valid.
The debugger shows the name not the number.
The code reads like English.

---

## Why MapInit Is Outside the Loop

`MapInit` fills 48 × 27 = 1296 tiles. It writes to memory.
It takes time.

You only need to do this once. The map does not change between frames
unless the player digs a tile – and even then you change one tile,
not the entire map.

```c
/* CORRECT – runs once */
GameMap map;
MapInit(&map);

while (!WindowShouldClose()) {
    MapDraw(&map);   /* reads the map – fast */
}

/* WRONG – runs 60 times per second for no reason */
while (!WindowShouldClose()) {
    MapInit(&map);   /* resets the entire map every frame */
    MapDraw(&map);
}
```

The wrong version also means any tile the player digs is immediately
restored on the next frame. The game would be unplayable.

---

## Why TileSolid Returns True for Out-of-Bounds Coordinates

```c
static bool TileSolid(const GameMap *m, int col, int row)
{
    if (col < 0 || col >= MAP_COLS) return true;
    if (row < 0 || row >= MAP_ROWS) return true;
    return m->tiles[row][col] != TILE_EMPTY;
}
```

If you ask "is tile at column -1 solid?" there is no tile there.
Accessing `m->tiles[row][-1]` is undefined behavior in C – the
program reads memory it does not own and may crash.

Returning `true` for out-of-bounds means the edges of the map
are treated as solid walls. The player is contained inside the map
automatically. No separate boundary check is needed in Assignment 3.

---

## Why groundY Stays in Assignment 2

Assignment 2 adds the tile map but does not change the collision.
This is intentional.

```
one change at a time:
Assignment 1 → new constants and player placeholder
Assignment 2 → tile map drawn correctly
Assignment 3 → collision switched to tiles

if Assignment 2 changed both drawing and collision at once:
a bug could be in either system
you would not know which one to fix
```

Keeping `groundY` as a fallback means if your `MapDraw` has a bug
the player still lands on the floor. You can see the visual result
of your map code without worrying about breaking the game.

---

## What You Should Be Able to Do After These Two Assignments

```
L1 – L2:
□ explain what a two-dimensional array is
□ explain why MAP_ROWS and MAP_COLS must be #define
□ explain why MapInit is called before the loop
□ run the game and confirm tiles are visible

L3:
□ add a platform – a row of TILE_EARTH tiles in the middle of the map
□ add a gap in the floor – a section of TILE_EMPTY in row 21
□ change the floor color without breaking anything else

L4:
□ add TILE_ORE to the enum and draw it in a different color
□ count how many TILE_EARTH tiles exist and print it before the loop
□ explain why TileSolid returns true for out-of-bounds
```
