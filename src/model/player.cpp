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

void Player::Move() {
    Move(false, false, false);
}

void Player::Move(bool moveLeft, bool moveRight, bool jump) {
    float speedMultiplier = stamina < JUMP_STAMINA_COST ? SPEED_PENALTY : 1.0f;

    if (moveRight && !moveLeft) {
        velocity.x = PLAYER_SPEED * speedMultiplier;
        facingRight = true;
    } else if (moveLeft && !moveRight) {
        velocity.x = -PLAYER_SPEED * speedMultiplier;
        facingRight = false;
    } else {
        velocity.x = 0.0f;
    }

    if (jump) {
        Jump();
    }
}

void Player::Jump() {
    if (!Onground) return;

    float jumpMultiplier = stamina < JUMP_STAMINA_COST ? JUMP_PENALTY : 1.0f;
    velocity.y = JUMP_FORCE * jumpMultiplier;
    Onground = false;
    UpdatePlayerStamina(-JUMP_STAMINA_COST);
}

void Player::UpdatePlayerStamina(float change) {
    if (change < 0.0f) {
        if (stamina >= -change) stamina += change;
    } else {
        stamina += change;
    }

    if (stamina > PLAYER_MAX_STAMINA) stamina = PLAYER_MAX_STAMINA;
    if (stamina < 0.0f) stamina = 0.0f;
}

float Player::getStamina() const {
    return stamina;
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