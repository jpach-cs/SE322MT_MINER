#pragma once

/* config.hpp
 *
 * Model: game state and rules. Must not include raylib, so it can be unit tested without a window.
 *
 * Compile-time constants as constexpr: tile and screen sizes, map size, player size and
 * speed, physics, and the stamina numbers. Replaces the #define and static const block
 * at the top of main.c.
 *
 * Placeholder from the skeleton PR. Target layout: sugestions.md in
 * jpach-cs/esof-322_miner_decription. Design: docs/architecture.md (to be written from
 * the Alpha and Beta documents). Move code here one class at a time, with its tests in
 * tests/test_<module>.cpp, and keep make test passing at every step. */

#include <map>
#include <string>

// Screen Config
constexpr int SCREEN_W = 480;
constexpr int SCREEN_H = 270;

// Tile Config
constexpr int TILE_SIZE = 10;
constexpr int MAP_COLS = (SCREEN_W / TILE_SIZE);
constexpr int MAP_ROWS = (SCREEN_H / TILE_SIZE);
constexpr int PLAYER_WIDTH = TILE_SIZE;
constexpr int PLAYER_HEIGHT = TILE_SIZE * 3;

// Physics Config
constexpr int PLAYER_SPEED = 2;
constexpr float GRAVITY = -0.4f;
constexpr float JUMP_FORCE = 7.5f;
constexpr float MAX_FALL = -9.0f;

// Stamina Bar Config
constexpr int   STAMINA_BAR_W = 100;             /* width of stamina bar in pixels */
constexpr int   STAMINA_BAR_H = 15;              /* height of stamina bar in pixels */
constexpr int   STAMINA_BAR_X = 15;              /* x position of stamina bar */
constexpr int   STAMINA_BAR_Y = 35;              /* y position of stamina bar */
constexpr int   STAMINA_BAR_TEXT_SIZE = 18;      /* border thickness of stamina bar */

// Stamina Config
constexpr float PLAYER_MAX_STAMINA = 100.0f;     /* maximum stamina value */
constexpr float JUMP_STAMINA_COST = 15.0f;       /* stamina cost per jump  */
constexpr float STAMINA_FROM_FOOD = 40.0f;       /* stamina gained from food */
constexpr float SPEED_PENALTY = 0.75f;           /* speed penalty when stamina is low */
constexpr float JUMP_PENALTY = 0.5f;             /* jump penalty when stamina is low */

// Input Config
// Key => Action
std::map<int, std::string> INPUT_MAP = {
    {int('A'), "left"},
    {263, "left"}, // Left arrow
    {int('D'), "right"},
    {262, "right"}, // Right arrow
    {int(' '), "jump"},
    {int('W'), "jump"},
    {265, "jump"} // Up arrow
};