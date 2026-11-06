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

Player::Player() : Player(100.0f) {}

Player::Player(float initialStamina)
    : stamina(initialStamina),
      facingRight(true),
      Onground(false),
      position(50.0f, GROUND_Y + PLAYER_H),
      velocity() {}

Player::~Player() {}

// Handle horizontal movement and input
void Player::Move() {
    // Left/Right input
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        velocity.x -= PLAYER_SPEED; 
        facingRight = false;
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        velocity.x += PLAYER_SPEED;
        facingRight = true;
    }
}

// jumping action
void Player::Jump() {
    if (Onground && (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))) {
        velocity.y = JUMP_FORCE;
        Onground = false;
    }
}

//  gravity and update vertical position
void Player::UpdatePhysics() {
    if (!Onground) {
        velocity.y += GRAVITY;
        if (velocity.y < MAX_FALL) {
            velocity.y = MAX_FALL;
        }
    }

    // velocity to position 
    position += velocity; 

    // Simple temporary floor collision check at screen bottom
    if (position.y >= GROUND_Y - PLAYER_H) {
        position.y = GROUND_Y - PLAYER_H;
        velocity.y = 0;
        Onground = true;
    }
}

Vector2D Player::getPosition() const {
    return position;
}

void Player::SetPosition(Vector2D pos) {
    position = pos;
}

bool Player::IsOnGround() const {
    return Onground;
}

bool Player::IsFacingRight() const {
    return facingRight;
}