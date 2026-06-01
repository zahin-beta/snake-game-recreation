#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "config.h"
#include <raylib.h>

#define MAX_OBSTACLES 60
#define FOOD_PER_LEVEL 10
#define MAX_LEVELS 5

typedef struct {
    Vector2 items[MAX_OBSTACLES];
    int count;
} ObstacleTable;

void ObstaclesGenerate(ObstacleTable *ot, int level);
void DrawObstacles(ObstacleTable *ot, Texture2D tex);

#endif
