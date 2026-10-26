#pragma once

/* player.hpp
 *
 * Model: game state and rules. Must not include raylib, so it can be unit tested without a window.
 *
 * Player state and its per-frame update: movement, gravity, floor collision, screen
 * bounds, and food pickup. Today: struct Player and the INPUT, PHYSICS, FLOOR COLLISION,
 * SCREEN BOUNDARIES, and FOOD sections of main().
 *
 * Placeholder from the skeleton PR. Target layout: sugestions.md in
 * jpach-cs/esof-322_miner_decription. Design: docs/architecture.md (to be written from
 * the Alpha and Beta documents). Move code here one class at a time, with its tests in
 * tests/test_<module>.cpp, and keep make test passing at every step. */
