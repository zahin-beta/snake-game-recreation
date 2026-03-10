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
// food struct
typedef struct Food
{
    Vector2 pos;
    Texture2D texture;
    Sound sound;
} Food;

void foodDraw(Food *food)
{
    DrawTexture(food->texture, food->pos.x * CELL_SIZE, food->pos.y * CELL_SIZE, WHITE);
  //  PlaySound(food->sound);
}

Vector2 randomPos()
{
    int x = GetRandomValue(0, CELL_COUNT - 1);
    int y = GetRandomValue(0, CELL_COUNT - 1);
    return (Vector2){x, y};
}

Food createFood()
{
    Food f =
        {
            .pos = randomPos(),
            .texture = LoadTexture("../assets/food.png"),
            .sound = LoadSound("../assets/foodpopup.wav")};

    PlaySound(f.sound);
    return f;
}
// ending food struct

// snake struct

typedef struct Snake
{
    Vector2 body[100];
    int length;
    Vector2 direction;
} Snake;
void initialSnake(Snake *s)
{
    s->length = 3;
    s->body[0] = (Vector2){CELL_COUNT / 2, CELL_COUNT / 2};
    s->body[1] = (Vector2){CELL_COUNT / 2 - 1, CELL_COUNT / 2};
    s->body[2] = (Vector2){CELL_COUNT / 2 - 2, CELL_COUNT / 2};
    s->direction = (Vector2){1, 0};
}

void drawSnake(Snake *s)
{
    for (size_t i = 0; i < s->length; i++)
    {
        DrawRectangle(s->body[i].x * CELL_SIZE, s->body[i].y * CELL_SIZE, CELL_SIZE, CELL_SIZE, darkGreen);
    }
}
void input(Snake *s)
{
    if (IsKeyPressed(KEY_W) && s->direction.y != 1)
    {
        s->direction = (Vector2){0, -1};
    }
    if (IsKeyPressed(KEY_S) && s->direction.y != -1)
    {
        s->direction = (Vector2){0, 1};
    }
    if (IsKeyPressed(KEY_A) && s->direction.x != 1)
    {
        s->direction = (Vector2){-1, 0};
    }
    if (IsKeyPressed(KEY_D) && s->direction.x != -1)
    {
        s->direction = (Vector2){1, 0};
    }
}

void snakeMovement(Snake *s)
{
    for (size_t i = s->length - 1; i > 0; i--)
    {
        s->body[i] = s->body[i - 1];
    }
    {
        s->body[0].x += s->direction.x;
        s->body[0].y += s->direction.y;
    }
}

// ending snake struct

// collision

bool checkSelfCollision(Snake *s)
{
    for (size_t i = 1; i < s->length; i++)
    {
        if (s->body[0].x == s->body[i].x && s->body[0].y == s->body[i].y)
        {
            return true;
        }
    }
    return false;
}

bool wallCollision(Snake *s)
{
    if (s->body[0].x < 0 || s->body[0].x >= CELL_COUNT || s->body[0].y < 0 || s->body[0].y >= CELL_COUNT)
    {
        return true;
    }
    return false;
}

bool checkCollision(Snake *s, Food *f)
{
    if (s->body[0].x == f->pos.x && s->body[0].y == f->pos.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//
// text
void drawScore(int *score)
{
    DrawText(TextFormat("SCORE: %d", *score), 20, 20, 30, BLACK);
}

//

int main()
{
    InitWindow(CELL_SIZE * CELL_COUNT, CELL_SIZE * CELL_COUNT, "SNAKE GAME");
    SetTargetFPS(60);
    InitAudioDevice();
    Sound eat = LoadSound("../assets/eat.ogg");
    Sound die = LoadSound("../assets/die.ogg");
    int framecount = 0;
    int score = 0;
    Food food = createFood();
    Snake snake;
    initialSnake(&snake);
    bool gameOver = false;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        if (!gameOver)
        {
            framecount++;
            input(&snake);
            if (framecount == 10)
            {
                snakeMovement(&snake);
                framecount = 0;
            }
            ClearBackground(green);
            foodDraw(&food);
            drawSnake(&snake);
            drawScore(&score);
            if (checkCollision(&snake, &food))
            {
                score++;
                PlaySound(eat);
                food.pos = randomPos();
                PlaySound(food.sound);
                snake.body[snake.length] = snake.body[snake.length - 1];
                snake.length++;
            }
            if (checkSelfCollision(&snake) || wallCollision(&snake))
            {
                gameOver = true;
                PlaySound(die);
            }
        }
        else
        {
            ClearBackground(BLACK);
            DrawText("GAME OVER", 300, 350, 40, WHITE);
            DrawText(TextFormat("SCORE: %d", score), 300, 350 + 50, 40, WHITE);
            DrawText("PRESS R TO RESTART",300,450,35,YELLOW);
            if (IsKeyPressed(KEY_R))
            {
                gameOver = false;
                score = 0;
                initialSnake(&snake);
                food.pos = randomPos();
            }
            
        }
        

        EndDrawing();
    }
    UnloadTexture(food.texture);
    UnloadSound(eat);
    UnloadSound(die);
    UnloadSound(food.sound);
    CloseWindow();
}