#ifndef FOOD_H
#define FOOD_H

#include <raylib.h>
#include "config.h"
#include "snake.h"
#include "obstacle.h"

typedef struct
{
    Vector2 pos;
    Texture2D texture;
    Sound sound;
} Food;

Food CreateFood();
void DrawFood(Food *f);
Vector2 RandomGridPosAvoidSnake(Snake *s, ObstacleTable *ot);

#endif
