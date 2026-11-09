/* world_view.cpp
 *
 * View: drawing only. May read the Model; never changes it.
 *
 * Implementation of world_view.hpp.
 *
 * Placeholder from the skeleton PR. Target layout: sugestions.md in
 * jpach-cs/esof-322_miner_decription. Design: docs/architecture.md (to be written from
 * the Alpha and Beta documents). Move code here one class at a time, with its tests in
 * tests/test_<module>.cpp, and keep make test passing at every step. */

#include "world_view.hpp"

WorldView::WorldView(int screenW, int screenH, const char* title, int targetFps)
    : screenW_(screenW), screenH_(screenH),
      background_{ 18, 10, 5, 255 },
      earthColor_(DARKBROWN),
      foodColor_(YELLOW) 
{
    InitWindow(screenW_, screenH_, title);
    SetTargetFPS(targetFps);
}

WorldView::~WorldView() 
{
    CloseWindow();
}

bool WorldView::ShouldClose()
{
    return WindowShouldClose();
}

void WorldView::BeginFrame() const 
{
    BeginDrawing();
    ClearBackground(background_);
}

void WorldView::DrawTileMap(const TileMap& map) const 
{
    for (int row = 0; row < map.Rows(); row++) {
        for (int col = 0; col < map.Cols(); col++) {
            const TileType type = map.Get(col, row);
            if (type == TileType::Empty) continue;
            DrawRectangleRec(ToScreen(map.TileWorldRect(col, row)),
                             type == TileType::Food ? foodColor_ : earthColor_);
        }
    }
}

void WorldView::EndFrame()
{
    EndDrawing();
}

Rectangle WorldView::ToScreen(const WorldRect& r) const
{
    return Rectangle{ r.x, screenH_ - (r.y + r.height), r.width, r.height };
}

Rectangle WorldView::ToScreen(Vector2 worldPos, float w, float h) const
{
    return ToScreen(WorldRect{ worldPos.x, worldPos.y, w, h });
}
