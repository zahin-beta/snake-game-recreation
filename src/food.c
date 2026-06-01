#include "food.h"
#include "assets.h"
#include "raylib_api.h"

Vector2 RandomGridPosAvoidSnake(Snake *s, ObstacleTable *ot)
{
    Vector2 pos;
    bool ok;
    do
    {
        pos.x = (float)p_GetRandomValue(0, CELL_COUNT - 1);
        pos.y = (float)p_GetRandomValue(0, CELL_COUNT - 1);
        ok = true;
        for (int i = 0; i < s->length; i++)
        {
            if (s->body[i].x == pos.x && s->body[i].y == pos.y)
            {
                ok = false;
                break;
            }
        }
        if (ok && ot)
        {
            for (int i = 0; i < ot->count; i++)
            {
                if (pos.x == ot->items[i].x && pos.y == ot->items[i].y)
                {
                    ok = false;
                    break;
                }
            }
        }
    } while (!ok);
    return pos;
}

Food CreateFood()
{
    Food f =
        {
            .pos = {0, 0},
            .texture = LoadTextureFromMem(FOOD_PNG_DATA, FOOD_PNG_SIZE, ".png"),
            .sound = LoadSoundFromMem(FOODPOPUP_WAV_DATA, FOODPOPUP_WAV_SIZE, ".wav")};

    return f;
}

void DrawFood(Food *f)
{
    p_DrawTexture(f->texture, f->pos.x * CELL_SIZE, f->pos.y * CELL_SIZE, WHITE);
}
