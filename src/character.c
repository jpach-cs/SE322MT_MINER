#include "character.h"

void useStamina(Character *player, float amountUsed)
{
    if (player->stamina < amountUsed)
    {
        player->stamina = 0.0f;
    }
    else
    {
        player->stamina -= amountUsed;
    }
}

void restoreStamina(Character *player, float amountRestored)
{
    if ((player->stamina + amountRestored) > PLAYER_MAX_STAMINA)
    {
        player->stamina = PLAYER_MAX_STAMINA;
    }
    else
    {
        player->stamina += amountRestored;
    }
}