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

#include "Vector2D.hpp"

 static const float PLAYER_SPEED = 2.0f;
 static const float GRAVITY = -0.4f;
 static const float JUMP_FORCE = 7.5f;
 static const float MAX_FALL = -9.0f;
 static const float PLAYER_H = 30.0f;
 static const float FLOOR_Y = 270.0f;                          // screen bottom (SCREEN_H)

 class Player {
    private:
        float stamina;
        bool facingRight;
        bool Onground;
        Vector2D position;
        Vector2D velocity;

    public:
        Player();
        explicit Player(float initialStamina);
        ~Player();

        void UpdatePlayerStamina(float change);
        float getStamina() const;

        void Move();
        void Jump();
        void UpdatePhysics();

        Vector2D getPosition() const;
        void SetPosition(Vector2D pos);
        bool IsOnGround() const;
        bool IsFacingRight() const;
};