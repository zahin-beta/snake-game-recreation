#include "snake.h"
#include "raylib_api.h"

static Color darkGreen = {43, 51, 24, 255};

void InitSnake(Snake *s)
{
    s->length = 3;
    s->body[0] = (Vector2){CELL_COUNT / 2, CELL_COUNT / 2};
    s->body[1] = (Vector2){CELL_COUNT / 2 - 1, CELL_COUNT / 2};
    s->body[2] = (Vector2){CELL_COUNT / 2 - 2, CELL_COUNT / 2};
    s->direction = (Vector2){1, 0};
    s->dirQueueLen = 0;
}

void FlushInput(Snake *s)
{
    s->dirQueueLen = 0;
}

static Color headGreen = {70, 110, 40, 255};

void DrawSnake(Snake *s)
{
    for (int i = 0; i < s->length; i++)
    {
        Color c = (i == 0) ? headGreen : darkGreen;
        p_DrawRectangle(s->body[i].x * CELL_SIZE, s->body[i].y * CELL_SIZE, CELL_SIZE, CELL_SIZE, c);
    }
    if (s->length > 0)
    {
        float cx = s->body[0].x * CELL_SIZE + CELL_SIZE / 2;
        float cy = s->body[0].y * CELL_SIZE + CELL_SIZE / 2;
        float off = 4;
        p_DrawCircle(cx - off, cy - off, 3, WHITE);
        p_DrawCircle(cx + off, cy - off, 3, WHITE);
    }
}

void HandleInput(Snake *s)
{
    Vector2 candidates[4] =
    {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}
    };
    int keys[4] = {KEY_W, KEY_S, KEY_A, KEY_D};
    Vector2 last = (s->dirQueueLen > 0) ? s->dirQueue[s->dirQueueLen - 1] : s->direction;

    for (int i = 0; i < 4; i++)
    {
        if (p_IsKeyPressed(keys[i]) && candidates[i].x != -last.x && candidates[i].y != -last.y)
        {
            if (s->dirQueueLen < DIR_QUEUE_CAP)
            {
                s->dirQueue[s->dirQueueLen++] = candidates[i];
            }
            break;
        }
    }
}

void MoveSnake(Snake *s, bool wrap)
{
    if (s->dirQueueLen > 0)
    {
        s->direction = s->dirQueue[0];
        for (int i = 1; i < s->dirQueueLen; i++)
            s->dirQueue[i - 1] = s->dirQueue[i];
        s->dirQueueLen--;
    }

    for (int i = s->length - 1; i > 0; i--)
    {
        s->body[i] = s->body[i - 1];
    }
    s->body[0].x += s->direction.x;
    s->body[0].y += s->direction.y;

    if (wrap)
    {
        s->body[0].x = (float)(((int)s->body[0].x + CELL_COUNT) % CELL_COUNT);
        s->body[0].y = (float)(((int)s->body[0].y + CELL_COUNT) % CELL_COUNT);
    }
}
