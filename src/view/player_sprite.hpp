#pragma once

/* player_sprite.hpp
*
* View: drawing only. May read the Model; never changes it.
*
* Player textures and animation (resources/miner*.png). Today: the placeholder rectangle
* drawn in main().
*
* Placeholder from the skeleton PR. Target layout: sugestions.md in
* jpach-cs/esof-322_miner_decription. Design: docs/architecture.md (to be written from
* the Alpha and Beta documents). Move code here one class at a time, with its tests in
* tests/test_<module>.cpp, and keep make test passing at every step. */
#include "raylib.h"
#include "world_view.hpp"

struct PlayerSpriteLayout
{
	float width = 10.0f;
	float height = 30.0f;
	Color colorRight = GREEN;
	Color colorLeft = LIME;
};

class PlayerSprite {

	public:
		// explicit stops PlayerSpriteLayout into a PlayerSprite
		explicit PlayerSprite(const PlayerSpriteLayout &layout = PlayerSpriteLayout{});

		void draw(const WorldView &view, Vector2 worldPos, bool facingRight) const;

		const PlayerSpriteLayout &layout() const {return layout_;};

    private:
		PlayerSpriteLayout layout_;
         
};