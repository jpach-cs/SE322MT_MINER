#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

#define MAX_STAMINA 100.0f
#define STAMINA_FROM_FOOD 20.0f
#define JUMP_STAMINA_COST 10.0f

typedef struct{
    float stamina;
    Vector2 position;
    Vector2 velocity;
}Player;

#endif