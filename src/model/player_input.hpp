#pragma once

/* player_input.hpp
 *
 * Model: game state and rules. Must not include raylib, so it can be unit tested without a window.
 *
 * What the player wants to do this frame (move left or right, jump), as plain data. The
 * Controller fills it in; the Model reads it. No key codes here.
 *
 * Placeholder from the skeleton PR. Target layout: sugestions.md in
 * jpach-cs/esof-322_miner_decription. Design: docs/architecture.md (to be written from
 * the Alpha and Beta documents). Move code here one class at a time, with its tests in
 * tests/test_<module>.cpp, and keep make test passing at every step. */

#include <string>
#include "config.hpp"

class PlayerInput {
    private:
        int keyPressed;
        std::string action;

    public:
        PlayerInput(int keyPressed) {
            if (!INPUT_MAP.contains(keyPressed)) {
                action = "";
            }
            action = INPUT_MAP[keyPressed];
        };
};