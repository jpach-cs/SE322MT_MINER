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
#include "model/player.hpp"
#include "controller/input.hpp"

int main()
{
    WorldView view(SCREEN_W, SCREEN_H, "Sample Game");
    TileMap map(MAP_COLS, MAP_ROWS, TILE_SIZE);
    map.LoadTestMap();

    Player player;

    StaminaBar staminaBar;
    PlayerSprite sprite;

    while (!view.ShouldClose())
    {
        HandleInput(player);
        player.UpdatePhysics(map);

        view.BeginFrame();
        view.DrawTileMap(map);
        staminaBar.draw(player.getStamina(), PLAYER_MAX_STAMINA);

        const Vector2D position = player.getPosition();
        sprite.draw(view,
                    Vector2{position.x, position.y},
                    player.IsFacingRight());
        view.EndFrame();
    }

    return 0;
}
