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
#include "controller/input.hpp"
#include "model/player.hpp"
#include <raylib.h>
#include <cstdint>

void HandleInput(Player& player){
    uint8_t mask = INPUT_NONE;
    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) mask |= LEFT_INPUT;
    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) mask |= RIGHT_INPUT;
    if(IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)) mask |= JUMP_INPUT;
    ProcessInput(mask, player);
}

void ProcessInput(uint8_t mask, Player& player)
{
    bool moveLeft  = (mask & LEFT_INPUT) != 0;
    bool moveRight = (mask & RIGHT_INPUT) != 0;
    bool jump      = (mask & JUMP_INPUT) != 0;

    player.Move(moveLeft, moveRight, jump);
}