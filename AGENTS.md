# AGENTS.md

Guidance for AI coding agents (and new humans) working in this repository.
Read this before touching `src/main.c`.

## What this is

A 2D platformer prototype for Montana Tech SE322, written in C99 against
[raylib](https://www.raylib.com). The whole game currently lives in
`src/main.c`. Headers and a prebuilt Windows raylib are vendored under
`include/` and `lib/`.

## Build, run, check

The Makefile detects the platform. Use these targets, not ad-hoc compiler
invocations:

| Target        | What it does                                                      |
|---------------|-------------------------------------------------------------------|
| `make`        | compile to `bin/game` (macOS/Linux) or `bin/game.exe` (Windows)   |
| `make run`    | compile and launch                                                |
| `make clean`  | remove the binary and macOS `.dSYM`                               |
| `make lint`   | cppcheck static analysis, non-zero exit on any finding            |
| `make docs`   | doxygen + graphviz UML-style diagrams into `docs/html/`           |
| `make test`   | placeholder, exits non-zero until a test harness exists           |

Dependencies per platform:

- **Windows:** [w64devkit](https://github.com/skeeto/w64devkit/releases). raylib is vendored.
- **macOS:** Xcode Command Line Tools, then `brew install raylib doxygen graphviz cppcheck`.
- **Linux:** a system raylib (`libraylib-dev`), plus doxygen, graphviz, cppcheck.

Before opening a PR, `make` and `make lint` must both pass with no new
warnings. The one accepted warning is the unused `tileSolid` function, which
is reserved for future tile collision.

## Code conventions

- **C99, 4-space indent, braces on the same line.** Match the surrounding code.
- **`#define` only for compile-time array sizes** (`TILE_SIZE`, `SCREEN_W`,
  `MAP_COLS`, ...). Everything else is `static const`. Never define the same
  name both ways; the preprocessor will mangle the `static const` line.
- **One `Player` struct, one stamina bar function.** Several past PRs each
  added their own. Extend the existing `Player` and `drawStaminaBar` rather
  than adding a parallel version with different casing.
- **Function naming is mixed** (`mapInit`, `UpdatePlayerStamina`). Keep the
  assignment-mandated names (`UpdatePlayerStamina`, `RegenerateStaminaFromFood`)
  exactly as they are. For new helpers, prefer lowerCamelCase to match the
  tile functions.
- **Stamina changes go through `UpdatePlayerStamina`.** It clamps to
  `[0, PLAYER_MAX_STAMINA]`. Do not subtract from `player.stamina` directly.

## Coordinate system (read this twice)

World `y` points **up**. `position.y` is the player's feet, `groundY` is
`4 * TILE_SIZE`, and map row 0 is the floor. Screen space is converted at
draw time with `SCREEN_H - y`. Use `tileScreenRect` and `playerScreenRect`
for anything that touches both the map and the player; they are the single
source of truth for where things are on screen.

Several abandoned branches flipped to y-down with the floor at row 21. Code
from those branches cannot be merged mechanically. Port the idea, not the diff.

## Gameplay rules currently implemented

- Jumping costs `JUMP_STAMINA_COST`. Below that threshold the player moves at
  `SPEED_PENALTY` speed and jumps at `JUMP_PENALTY` height. A low-stamina
  jump does not drain stamina further.
- Touching a `TILE_FOOD` tile restores `STAMINA_FROM_FOOD` and removes the
  tile. Food tiles are placed in `mapInit`.
- Controls: arrows or A/D to move, Space, Up or W to jump.

## Branching and PRs

- Feature work goes on `alpha/feature/<topic>/<name>` or
  `beta/feature/<topic>/<name>` and is PR'd into the matching
  `*/feature/<topic>/integration` branch.
- Integration branches are what get PR'd into `main`, once they build and
  lint clean. Individual feature branches should not target `main` directly.
- Keep PRs to source changes. Do **not** commit:
  - `bin/game.exe` or any binary (gitignored, but check `git status`)
  - personal tool paths in `Makefile` or `.vscode/*.json`
  - accidental files such as shell output redirected to a stray filename
  - course handouts or assignment text
- Commit messages: imperative subject under 72 characters, a body that says
  why. Reference the PR or issue being integrated.

## Things that have bitten people

- A merge that compiles on one branch and not the other usually means
  duplicate constants or a duplicate `typedef`. Grep before you merge.
- Passing `Player` by value to a function that is supposed to change
  stamina silently does nothing. Always pass `Player *`.
- Food regen without removing the tile makes stamina infinite.
- raylib's `DrawRectangle` takes `int`; the float constants in this file are
  implicitly truncated. Use `DrawRectangleRec` with a `Rectangle` when
  positions are floats.

## Testing

There is no unit test harness yet. `make test` exits non-zero on purpose so
CI cannot mistake "no tests" for "tests passed". When adding one, the
pure-logic functions (`UpdatePlayerStamina`, `RegenerateStaminaFromFood`,
`mapInit`) can be exercised by including `main.c` with `main` renamed via
`#define main game_main`; they do not need a window.
