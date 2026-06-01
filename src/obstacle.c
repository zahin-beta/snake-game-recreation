#include <stddef.h>
#include "obstacle.h"
#include "raylib_api.h"

// ---- level definitions ----

// Level 1: none
#define L1_COUNT 0

// Level 2: two horizontal I-walls
static const Vector2 L2[] = {
    {8,5},{9,5},{10,5},{11,5},{12,5},
    {16,16},{17,16},{18,16},{19,16},{20,16}
};
#define L2_COUNT 10

// Level 3: two L-shapes
static const Vector2 L3[] = {
    {5,8},{6,8},{7,8},{8,8},{8,9},{8,10},{8,11},
    {16,16},{17,16},{18,16},{19,16},{16,17},{16,18},{16,19}
};
#define L3_COUNT 14

// Level 4: O-frame center + two I-walls
static const Vector2 L4[] = {
    {10,10},{11,10},{12,10},{13,10},{14,10},
    {14,11},{14,12},{14,13},
    {10,14},{11,14},{12,14},{13,14},{14,14},
    {10,11},{10,12},{10,13},
    {3,3},{3,4},{3,5},{3,6},{3,7},
    {21,17},{21,18},{21,19},{21,20},{21,21}
};
#define L4_COUNT 26

// Level 5: dense mix
static const Vector2 L5[] = {
    {2,2},{3,2},{4,2},{5,2},{6,2},{2,3},{2,4},{2,5},{2,6},
    {18,18},{19,18},{20,18},{21,18},{22,18},{22,19},{22,20},{22,21},{22,22},
    {12,5},{12,6},{12,7},{12,8},{12,9},{12,10},{12,11},{12,12},{12,13},{12,14},{12,15},{12,16},
    {6,18},{7,18},{8,18},{9,18},{6,21},{7,21},{8,21},{9,21},{6,19},{6,20},{9,19},{9,20},
    {3,15},{4,15},{20,5},{21,5}
};
#define L5_COUNT 47

void ObstaclesGenerate(ObstacleTable *ot, int level)
{
    ot->count = 0;

    const Vector2 *src = NULL;
    int srcCount = 0;

    switch (level)
    {
    case 1: srcCount = L1_COUNT; src = NULL; break;
    case 2: src = L2; srcCount = L2_COUNT; break;
    case 3: src = L3; srcCount = L3_COUNT; break;
    case 4: src = L4; srcCount = L4_COUNT; break;
    case 5: src = L5; srcCount = L5_COUNT; break;
    default:
        if (level >= MAX_LEVELS) { src = L5; srcCount = L5_COUNT; }
        break;
    }

    int copyCount = (srcCount < MAX_OBSTACLES) ? srcCount : MAX_OBSTACLES;
    for (int i = 0; i < copyCount; i++)
        ot->items[i] = src[i];
    ot->count = copyCount;
}

void DrawObstacles(ObstacleTable *ot, Texture2D tex)
{
    for (int i = 0; i < ot->count; i++)
        p_DrawTexture(tex, (int)ot->items[i].x * CELL_SIZE, (int)ot->items[i].y * CELL_SIZE, WHITE);
}
