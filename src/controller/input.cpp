/* input.cpp
 *
 * Controller: turns keys into input for the Model. May use raylib and the Model.
 *
 * Implementation of input.hpp.
 *
 * Placeholder from the skeleton PR. Target layout: sugestions.md in
 * jpach-cs/esof-322_miner_decription. Design: docs/architecture.md (to be written from
 * the Alpha and Beta documents). Move code here one class at a time, with its tests in
 * tests/test_<module>.cpp, and keep make test passing at every step. */
#include "player_input.hpp"
#include "input.hpp"
#include "raylib.h"
#include <vector>

uint8_t HandleInput(){
    uint8_t mask = INPUT_NONE;
    if(IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))                           mask |= LEFT_INPUT;
    if(IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))                          mask |= RIGHT_INPUT;
    if(IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP))  mask |= JUMP_INPUT;
    return mask;
}
