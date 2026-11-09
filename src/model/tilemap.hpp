#pragma once

/* tilemap.hpp
*
* Model: game state and rules. Must not include raylib, so it can be unit tested without a window.
*
* TileType, the map grid, isSolidAt, and spawnPoint. Today: TileType, GameMap, mapInit,
* and tileSolid.
*
* Placeholder from the skeleton PR. Target layout: sugestions.md in
* jpach-cs/esof-322_miner_decription. Design: docs/architecture.md (to be written from
* the Alpha and Beta documents). Move code here one class at a time, with its tests in
* tests/test_<module>.cpp, and keep make test passing at every step. */

#include <vector>

enum class TileType
{
    Empty,
    Earth,
    Food
};

/** Axis-aligned rectangle in world space (y up). The View converts it to screen space. */
struct WorldRect
{
    float x;
    float y;
    float width;
    float height;
};

class TileMap
{
    public:
        TileMap(int cols, int rows, int tileSize);

        void LoadTestMap();

        int Cols() const { return cols_; }
        int Rows() const { return rows_; }
        int TileSize() const { return tileSize_; }

        bool InBounds(int col, int row) const;
        TileType Get(int col, int row) const;
        void Set(int col, int row, TileType type);

        bool IsSolid(int col, int row) const; // anything not Empty is solid
        WorldRect TileWorldRect(int col, int row) const; // World space rectangle covered by tile

    private:
        int Index(int col, int row) const { return row * cols_ + col; }

        int cols_;
        int rows_;
        int tileSize_;
        std::vector<TileType> tiles_;
};
