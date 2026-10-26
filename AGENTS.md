# AGENTS.md

Guidance for AI coding agents (and new humans) working in this repository.
Read this before touching `src/main.c`.

## What this is

A 2D platformer prototype for Montana Tech SE322, written in C99 against
[raylib](https://www.raylib.com). The whole game currently lives in
`src/main.c`. Headers and a prebuilt Windows raylib are vendored under
`include/` and `lib/`.

## Layout (C++ restructure in progress)

The game is moving from C to C++ in a Model / View / Controller layout. The
target is `sugestions.md` in `jpach-cs/esof-322_miner_decription`; the design
will be `docs/architecture.md`. The files below exist as placeholders, each
with a comment saying which part of `main.c` moves into it.

```text
src/
  main.c                 the game today; still what `make` builds
  main.cpp               future entry point: window, render target, loop
  model/                 state and rules, no raylib (make lint enforces this)
    config.hpp           constexpr constants
    stamina.hpp/.cpp     jump cost, regeneration, clamping, penalties
    tilemap.hpp/.cpp     TileType, isSolidAt, spawnPoint
    player_input.hpp     what the player wants to do this frame
    player.hpp/.cpp      player state and per-frame update
  view/                  drawing, may read the Model
    player_sprite.hpp/.cpp   textures and animation
    world_view.hpp/.cpp      drawWorld, tile colors
    hud.hpp/.cpp             drawStaminaBar
  controller/            keys to PlayerInput
    input.hpp/.cpp       readPlayerInput
tests/                   doctest unit tests (see Testing); test_stamina, test_tilemap,
                         and test_player are placeholders for the model tests
third_party/doctest.h
docs/                    architecture documents; Doxygen output goes to docs/html/
```

Move one piece at a time, with its tests, and keep `make test` green at every
step: the restructure must not change behavior. Dependencies point one way:
View and Controller may use the Model, never the reverse.

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
| `make test`   | build and run the doctest unit tests in `tests/` (`bin/tests`)    |

Dependencies per platform:

- **Windows:** [w64devkit](https://github.com/skeeto/w64devkit/releases). raylib is vendored.
- **macOS:** Xcode Command Line Tools, then `brew install raylib doxygen graphviz cppcheck`.
- **Linux:** a system raylib (`libraylib-dev`), plus doxygen, graphviz, cppcheck.

Before opening a PR, `make`, `make lint` and `make test` must all pass with no
new warnings. CI runs `make lint` and `make test` on every push. The one accepted warning is the unused `tileSolid` function, which
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

Unit tests use [doctest](https://github.com/doctest/doctest) (single header,
vendored at `third_party/doctest.h`) and are C++20, ahead of the C++
migration. `make test` builds `bin/tests` and runs it.

- `tests/test_main.cpp` is only the doctest entry point.
- `tests/test_game.cpp` compiles `src/main.c` in with `main` renamed to
  `game_main`, so the rules can be called without opening a window. Only that
  one file may include `main.c`; a second copy would duplicate `game_main` at
  link time.
- Test only functions that do not need a window: the stamina, food, tilemap
  and coordinate helpers. Input handling and the movement penalties still live
  inside `main()` and cannot be unit tested until the restructure pulls them
  out.
- When a rule moves into `src/model/`, move its tests into the matching
  placeholder (`test_stamina.cpp`, `test_tilemap.cpp`, `test_player.cpp`).
  `make test` builds every `tests/*.cpp`, so new test files need no Makefile
  change.
- CI builds raylib 6.0 from source (matching `include/`) and caches it.
