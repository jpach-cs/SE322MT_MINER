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
