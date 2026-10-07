#pragma once
#include "raylib.h"

#define PLAYER_MAX_STAMINA 150.0f
#define JUMP_STAMINA_COST 15.2f
#define STAMINA_FROM_FOOD 40.2f

typedef struct
{
    Vector2 position;
    Vector2 velocity;
    bool onGround;
    bool facingRight;
    float stamina;
} Character;

void useStamina(Character *player, float amountUsed);
void restoreStamina(Character *player, float amountRestored);