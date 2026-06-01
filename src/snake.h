#ifndef SNAKE_H
#define SNAKE_H

#include <raylib.h>
#include "config.h"

#define DIR_QUEUE_CAP 2

typedef struct
{
    Vector2 body[MAX_SNAKE_LENGTH];
    int length;
    Vector2 direction;
    Vector2 dirQueue[DIR_QUEUE_CAP];
    int dirQueueLen;
} Snake;

void InitSnake(Snake *s);
void DrawSnake(Snake *s);
void HandleInput(Snake *s);
void MoveSnake(Snake *s, bool wrap);
void FlushInput(Snake *s);

#endif
