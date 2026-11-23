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

#include "model/player.hpp"

#include <cmath>

namespace {

bool hasSolidTileOnLine(const TileMap& map, int col, int row, int colStep, int rowStep, int tileCount,
                        TileType& collisionTileType, int& collisionCol, int& collisionRow) {
    for (int i = 0; i < tileCount; i++) {
        if (map.IsSolid(col, row)) {
            collisionTileType = map.Get(col, row);
            collisionCol = col;
            collisionRow = row;
            return true;
        }
        col += colStep;
        row += rowStep;
    }
    return false;
}

}  // namespace

Player::Player() : Player(PLAYER_MAX_STAMINA) {}

Player::Player(float initialStamina)
    : stamina(initialStamina),
      facingRight(true),
      Onground(false),
      lastCollisionTileType(TileType::Empty),
      position(50.0f, GROUND_Y + PLAYER_HEIGHT),
      velocity() {}

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
void Player::UpdatePhysics(TileMap& map) {
    lastCollisionTileType = TileType::Empty;

    if (!Onground) {
        velocity.y += GRAVITY;
        if (velocity.y < MAX_FALL) {
            velocity.y = MAX_FALL;
        }
    }

    TileCollision(map, position);
}

void Player::TileCollision(TileMap& map, Vector2D previousPosition) {
    position = previousPosition;
    HorizontalCollision(map, previousPosition);
    position.y = previousPosition.y + velocity.y;
    VerticalCollision(map, previousPosition);
}

void Player::HorizontalCollision(TileMap& map, Vector2D previousPosition) {
    position.x = previousPosition.x + velocity.x;
    if (velocity.x == 0.0f) return;

    const bool movingRight = velocity.x > 0.0f;
    const int firstRow = static_cast<int>(std::floor(previousPosition.y / TILE_SIZE)) + 1;
    const int lastRow = static_cast<int>(
        std::ceil((previousPosition.y + PLAYER_HEIGHT) / TILE_SIZE));
    const int firstCol = movingRight
        ? static_cast<int>(std::floor((previousPosition.x + PLAYER_WIDTH) / TILE_SIZE))
        : static_cast<int>(std::ceil(previousPosition.x / TILE_SIZE)) - 1;
    const int lastCol = movingRight
        ? static_cast<int>(std::floor((position.x + PLAYER_WIDTH) / TILE_SIZE))
        : static_cast<int>(std::ceil(position.x / TILE_SIZE)) - 1;
    const int step = movingRight ? 1 : -1;

    for (int col = firstCol; movingRight ? col <= lastCol : col >= lastCol; col += step) {
        const float boundary = static_cast<float>(movingRight ? col : col + 1) * TILE_SIZE;
        const float previousEdge = movingRight
            ? previousPosition.x + PLAYER_WIDTH
            : previousPosition.x;
        const float currentEdge = movingRight
            ? position.x + PLAYER_WIDTH
            : position.x;
        const bool crossedBoundary = movingRight
            ? previousEdge <= boundary && currentEdge >= boundary
            : previousEdge >= boundary && currentEdge <= boundary;

        TileType collisionTileType = TileType::Empty;
        int collisionCol = col;
        int collisionRow = firstRow;
        if (crossedBoundary &&
            hasSolidTileOnLine(map, col, firstRow, 0, 1, lastRow - firstRow + 1,
                               collisionTileType, collisionCol, collisionRow)) {
            HandleTileCollision(map, collisionTileType, collisionCol, collisionRow);
            position.x = movingRight ? boundary - PLAYER_WIDTH : boundary;
            velocity.x = 0.0f;
            return;
        }
    }
}

void Player::VerticalCollision(TileMap& map, Vector2D previousPosition) {
    if (velocity.y == 0.0f) return;

    const int firstCol = static_cast<int>(std::floor(position.x / TILE_SIZE));
    const int lastCol = static_cast<int>(std::ceil((position.x + PLAYER_WIDTH) / TILE_SIZE)) - 1;
    const bool movingUp = velocity.y > 0.0f;
    const float previousEdge = movingUp
        ? previousPosition.y + PLAYER_HEIGHT
        : previousPosition.y;
    const float currentEdge = movingUp ? position.y + PLAYER_HEIGHT : position.y;
    const int firstRow = static_cast<int>(std::floor(previousEdge / TILE_SIZE)) + (movingUp ? 1 : 0);
    const int lastRow = static_cast<int>(std::floor(currentEdge / TILE_SIZE)) + (movingUp ? 1 : 0);
    const int rowStep = movingUp ? 1 : -1;

    for (int row = firstRow; movingUp ? row <= lastRow : row >= lastRow; row += rowStep) {
        const float boundary = static_cast<float>(movingUp ? row - 1 : row) * TILE_SIZE;
        const bool crossedBoundary = movingUp
            ? previousEdge <= boundary && currentEdge >= boundary
            : previousEdge >= boundary && currentEdge <= boundary;

        TileType collisionTileType = TileType::Empty;
        int collisionCol = firstCol;
        int collisionRow = row;
        if (row < 0 || row >= map.Rows() || !crossedBoundary ||
            !hasSolidTileOnLine(map, firstCol, row, 1, 0, lastCol - firstCol + 1,
                               collisionTileType, collisionCol, collisionRow)) continue;

        HandleTileCollision(map, collisionTileType, collisionCol, collisionRow);
        position.y = movingUp ? boundary - PLAYER_HEIGHT : boundary;
        velocity.y = 0.0f;
        Onground = !movingUp;
        return;
    }
}

void Player::HandleTileCollision(TileMap& map, TileType type, int col, int row) {
    lastCollisionTileType = type;
    if (type == TileType::Food) {
        UpdatePlayerStamina(STAMINA_FROM_FOOD);
        map.Set(col, row, TileType::Empty);
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

TileType Player::GetLastCollisionTileType() const {
    return lastCollisionTileType;
}