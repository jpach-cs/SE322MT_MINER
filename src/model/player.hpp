#pragma once
#include "model/Vector2D.hpp"
#include "model/config.hpp"
#include "model/tilemap.hpp"
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

 static const float GROUND_Y = SCREEN_H - PLAYER_HEIGHT;

 class Player {
    private:
        float stamina;
        bool facingRight;
        bool Onground;
        Vector2D position;
        Vector2D velocity;
        void TileCollision(const TileMap& map, Vector2D previousPosition);
        void HorizontalCollision(const TileMap& map, Vector2D previousPosition);
        void VerticalCollision(const TileMap& map, Vector2D previousPosition);

    public:
        Player();
        explicit Player(float initialStamina);

        void UpdatePlayerStamina(float change);
        float getStamina() const;

        void Move();
        void Move(bool moveLeft, bool moveRight, bool jump);
        void Jump();
        void UpdatePhysics(const TileMap& map);

        Vector2D getPosition() const;
        void SetPosition(Vector2D pos);
        bool IsOnGround() const;
        bool IsFacingRight() const;
};