/* tilemap.cpp
 *
 * Model: game state and rules. Must not include raylib, so it can be unit tested without a window.
 *
 * Implementation of tilemap.hpp.
 *
 * Placeholder from the skeleton PR. Target layout: sugestions.md in
 * jpach-cs/esof-322_miner_decription. Design: docs/architecture.md (to be written from
 * the Alpha and Beta documents). Move code here one class at a time, with its tests in
 * tests/test_<module>.cpp, and keep make test passing at every step. */
#include "tilemap.hpp"

TileMap::TileMap(int cols, int rows, int tileSize): cols_(cols), rows_(rows), tileSize_(tileSize),
                                                    tiles_(static_cast<size_t>(cols) * rows, TileType::Empty) {}

void TileMap::LoadTestMap() 
{
    for (int row = 0; row < rows_; row++) {
        for (int col = 0; col < cols_; col++) {
            Set(col, row, row < 5 ? TileType::Earth : TileType::Empty);
        }
    }

    // test food: two on the ground, one that needs a jump to reach
    Set(8, 5, TileType::Food);
    Set(40, 5, TileType::Food);
    Set(24, 10, TileType::Food);
}

bool TileMap::InBounds(int col, int row) const 
{
    return col >= 0 && col < cols_ && row >= 0 && row < rows_;
}

TileType TileMap::Get(int col, int row) const 
{
    return InBounds(col, row) ? tiles_[Index(col, row)] : TileType::Empty;
}

void TileMap::Set(int col, int row, TileType type) 
{
    if (InBounds(col, row)) tiles_[Index(col, row)] = type;
}

bool TileMap::IsSolid(int col, int row) const 
{
    if (!InBounds(col, row)) return true;
    return tiles_[Index(col, row)] != TileType::Empty;
}

WorldRect TileMap::TileWorldRect(int col, int row) const
{
    const float t = static_cast<float>(tileSize_);
    return WorldRect{ col * t, (row - 1) * t, t, t };
}
