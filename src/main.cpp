/* main.cpp
*
* Entry point: window, render target, and the game loop.
*
* Replaces src/main.c once the model, view, and controller code has moved. Until then
* the game still builds from main.c and this file is not compiled.
*
* Placeholder from the skeleton PR. Target layout: sugestions.md in
* jpach-cs/esof-322_miner_decription. Design: docs/architecture.md (to be written from
* the Alpha and Beta documents). Move code here one class at a time, with its tests in
* tests/test_<module>.cpp, and keep make test passing at every step. */

// Example use of worldview and hud
///////////////////////////////
#include "model/config.hpp"
#include "model/tilemap.hpp"
#include "view/world_view.hpp"
#include "view/hud.hpp"
#include "view/player_sprite.hpp"

static const float groundY = 4 * TILE_SIZE;

int main()
{
    WorldView view(SCREEN_W, SCREEN_H, "Sample Game");
    TileMap map(MAP_COLS, MAP_ROWS, TILE_SIZE);
    map.LoadTestMap();

    StaminaBar staminaBar;
    const float stamina = PLAYER_MAX_STAMINA;
    PlayerSprite sprite;

    const Vector2 playerStart = {(SCREEN_W / 2.0f) - (sprite.layout().width / 2), groundY};

    while (!view.ShouldClose())
    {
        view.BeginFrame();
        view.DrawTileMap(map);
        staminaBar.draw(stamina, PLAYER_MAX_STAMINA);
        sprite.draw(view, playerStart, true);
        view.EndFrame();
    }

    return 0;
}
