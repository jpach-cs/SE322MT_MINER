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

void HandleInput(){
    std::vector<int> inputs;
    int key = -1;
    while(key != 0){
        key = GetKeyPressed();
        std::string result = PlayerInput(key);
        
    }
}