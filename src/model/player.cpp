/* player.cpp
 *
 * Model: game state and rules. Must not include raylib, so it can be unit tested without a window.
 *
 * Implementation of player.hpp.
 *
 * Placeholder from the skeleton PR. Target layout: sugestions.md in
 * jpach-cs/esof-322_miner_decription. Design: docs/architecture.md (to be written from
 * the Alpha and Beta documents). Move code here one class at a time, with its tests in
 * tests/test_<module>.cpp, and keep make test passing at every step. */

 // Madison Austin player.cpp

#include "player.hpp"

player::Node() {
    facingRight = true;
    Onground = false;
    position = { 50.0f, 50.0f }; // Starting position
    velocity = { 0.0f, 0.0f };
}

player::~Node() {}

// Handle horizontal movement and input
void player::Move() {
    // Left/Right input
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        position.x -= PLAYER_SPEED;
        facingRight = false;
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        position.x += PLAYER_SPEED;
        facingRight = true;
    }
}

// jumping action
void player::Jump() {
    if (Onground && (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))) {
        velocity.y = JUMP_FORCE;
        Onground = false;
    }
}

//  gravity and update vertical position
void player::UpdatePhysics() {
    if (!Onground) {
        velocity.y += GRAVITY;
        if (velocity.y < MAX_FALL) {
            velocity.y = MAX_FALL;
        }
    }

    // velocity to position 
    position.y -= velocity.y; 

    // Simple temporary floor collision check at screen bottom (SCREEN_H = 270)
    if (position.y >= 270 - PLAYER_H) {
        position.y = 270 - PLAYER_H;
        velocity.y = 0;
        Onground = true;
    }
}

Vector2D player::getPosition() const {
    return position;
}

void player::SetPosition(Vector2D pos) {
    position = pos;
}

bool player::IsOnGround() const {
    return Onground;
}

bool player::IsFacingRight() const {
    return facingRight;
}