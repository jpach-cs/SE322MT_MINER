#pragma once

/* stamina.hpp
 *
 * Model: game state and rules. Must not include raylib, so it can be unit tested without a window.
 *
 * The stamina rules: jump cost, regeneration, clamping to [0, max], and the low-stamina
 * speed and jump penalties. Today: UpdatePlayerStamina and the multiplier logic in main().
 *
 * Placeholder from the skeleton PR. Target layout: sugestions.md in
 * jpach-cs/esof-322_miner_decription. Design: docs/architecture.md (to be written from
 * the Alpha and Beta documents). Move code here one class at a time, with its tests in
 * tests/test_<module>.cpp, and keep make test passing at every step. */


static const float PLAYER_MAX_STAMINA = 100.0f;     /* maximum stamina value */

class Stamina {
    private:
        float stamina;
    public:
        Stamina();
        explicit Stamina(float initialStamina);

        void UpdatePlayerStamina(float change);
        float getStamina() const;
        float ValidateStamina(float stamina);
};