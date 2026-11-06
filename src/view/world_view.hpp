#pragma once

/* world_view.hpp
 *
 * View: drawing only. May read the Model; never changes it.
 *
 * drawWorld and the tile colors. Today: drawMap and tileScreenRect.
 *
 * Placeholder from the skeleton PR. Target layout: sugestions.md in
 * jpach-cs/esof-322_miner_decription. Design: docs/architecture.md (to be written from
 * the Alpha and Beta documents). Move code here one class at a time, with its tests in
 * tests/test_<module>.cpp, and keep make test passing at every step. */
#include "../model/tilemap.hpp"
#include "raylib.h"

class WorldView
{
   public:
      WorldView(int screenW, int screenH, const char* title, int targetFps = 60);
      ~WorldView();
   
      WorldView(const WorldView&) = delete;
      WorldView& operator=(const WorldView&) = delete;
   
      bool ShouldClose() const;
   
      void BeginFrame() const;                    // BeginDrawing + clear background
      void DrawTileMap(const TileMap& map) const; // earth / food tiles
      void EndFrame() const;                      // EndDrawing
   
      Rectangle ToScreen(const Rectangle& worldRect) const; // World-space rect (y up) -> screen-space rect (y down).
      Rectangle ToScreen(Vector2 worldPos, float w, float h) const; // Convenience for sprites: position is the bottom-left corner in world space.
   
      int ScreenW() const { return screenW_; }
      int ScreenH() const { return screenH_; }

   private:
      int screenW_;
      int screenH_;
      Color background_;
      Color earthColor_;
      Color foodColor_;

};