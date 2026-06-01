#include "collision.h"

bool CheckSelfCollision(Snake *s)
{
    for (int i = 1; i < s->length; i++)
    {
        if (s->body[0].x == s->body[i].x && s->body[0].y == s->body[i].y)
            return true;
    }
    return false;
}

bool CheckWallCollision(Snake *s)
{
    if (s->body[0].x < 0 || s->body[0].x >= CELL_COUNT ||
        s->body[0].y < 0 || s->body[0].y >= CELL_COUNT)
        return true;
    return false;
}

bool CheckFoodCollision(Snake *s, Food *f)
{
    return s->body[0].x == f->pos.x && s->body[0].y == f->pos.y;
}

bool CheckObstacleCollision(Snake *s, ObstacleTable *ot)
{
    for (int i = 0; i < ot->count; i++)
    {
        if (s->body[0].x == ot->items[i].x && s->body[0].y == ot->items[i].y)
            return true;
    }
    return false;
}
