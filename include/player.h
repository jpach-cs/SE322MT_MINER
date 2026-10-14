#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

#define MAX_STAMINA 100.0f
#define FOOD_VALUE 20.0f
#define JUMP_COST 10.0f

typedef struct{
    float stamina;
    Vector2 position;
    Vector2 velocity;
}Player;

#endif