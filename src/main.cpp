/* main.cpp
 *
 * Entry point: window, render target, and the game loop.
 *
 * Replaces src/main.c once the model, view, and controller code has moved. Until then
 * the game still builds from main.c and this file is not compiled.
 *
 * Placeholder from the skeleton PR. Target layout: sugestions.md in
 * jpach-cs/esof-322_miner_decription. Design: docs/architecture.md (to be written from
 * the Alpha and Beta documents). Move code here one class at a time, with its tests in
 * tests/test_<module>.cpp, and keep make test passing at every step. */
 
 #include "raylib.h"
 #include "raymath.h"
 #include <stdbool.h>

#define SCREEN_W  720
#define SCREEN_H 480



int main(){

    InitWindow(SCREEN_W, SCREEN_H, "SE332 Miner Game");
    SetTargetFPS(60);
    
    while(!WindowShouldClose()){
        




    }

    CloseWindow();

    return 0; // Successful return
}