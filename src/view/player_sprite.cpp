/* player_sprite.cpp
*
* View: drawing only. May read the Model; never changes it.
*
* Implementation of player_sprite.hpp.
*
* Placeholder from the skeleton PR. Target layout: sugestions.md in
* jpach-cs/esof-322_miner_decription. Design: docs/architecture.md (to be written from
* the Alpha and Beta documents). Move code here one class at a time, with its tests in
* tests/test_<module>.cpp, and keep make test passing at every step. */
#include "player_sprite.hpp"


PlayerSprite::PlayerSprite(const PlayerSpriteLayout &layout) : layout_(layout) {}
 

void PlayerSprite::draw(const WorldView &view, Vector2 worldPos, bool facingRight) const 
{
    const Rectangle screen = view.ToScreen(worldPos, layout_.width, layout_.height);
    DrawRectangleRec(screen, facingRight ? layout_.colorRight : layout_.colorLeft);
}