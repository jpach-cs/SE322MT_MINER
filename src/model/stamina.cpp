/* stamina.cpp
 *
 * Model: game state and rules. Must not include raylib, so it can be unit tested without a window.
 *
 * Implementation of stamina.hpp.
 *
 * Placeholder from the skeleton PR. Target layout: sugestions.md in
 * jpach-cs/esof-322_miner_decription. Design: docs/architecture.md (to be written from
 * the Alpha and Beta documents). Move code here one class at a time, with its tests in
 * tests/test_<module>.cpp, and keep make test passing at every step. */

#include "stamina.hpp"

Stamina::Stamina()
    : stamina(PLAYER_MAX_STAMINA) {}

Stamina::Stamina(float initialStamina)
    : stamina(initialStamina) {
    stamina = ValidateStamina(initialStamina);
}

float Stamina::getStamina() const {
    return stamina;
}

void Stamina::UpdatePlayerStamina(float change) {
    if (change < 0.0f) {
        if (stamina >= (change * -1.0f)) {
            stamina += change;
        }
    } else {
        stamina += change;
    }

    stamina = ValidateStamina(stamina);
}

float Stamina::ValidateStamina(float staminaValue) {
    if (staminaValue > PLAYER_MAX_STAMINA) return PLAYER_MAX_STAMINA;
    if (staminaValue < 0.0f) return 0.0f;
    return staminaValue;
}
