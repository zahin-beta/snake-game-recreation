// snake game remake in c
//@author: Zahin Bin Hasan
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define CELL_SIZE 30
#define CELL_COUNT 25

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

typedef struct Food
{
    Vector2 pos;
    Texture2D texture;
} Food;

void foodDraw(Food *food)
{
    DrawTexture(food->texture, food->pos.x*CELL_SIZE,food->pos.y*CELL_SIZE, WHITE);
}

Vector2 randomPos ()
{
    int x = GetRandomValue(0, CELL_COUNT - 1);
    int y = GetRandomValue(0, CELL_COUNT - 1);
    return (Vector2){x,y};
}

Food createFood ()
{
    Food f =
        {
            .pos = randomPos(),
            .texture = LoadTexture("../assets/food.png")
        };
    return f;
}

int main()
{
    InitWindow(CELL_SIZE * CELL_COUNT, CELL_SIZE * CELL_COUNT, "SNAKE GAME");
    SetTargetFPS(60);

    Food food = createFood();
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(green);
        foodDraw(&food);
        EndDrawing();
    }
    UnloadTexture(food.texture);
    CloseWindow();
}