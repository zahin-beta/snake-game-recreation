#ifndef COLLISION_H
#define COLLISION_H

#include <stdbool.h>
#include "snake.h"
#include "food.h"
#include "obstacle.h"

bool CheckSelfCollision(Snake *s);
bool CheckWallCollision(Snake *s);
bool CheckFoodCollision(Snake *s, Food *f);
bool CheckObstacleCollision(Snake *s, ObstacleTable *ot);

#endif
