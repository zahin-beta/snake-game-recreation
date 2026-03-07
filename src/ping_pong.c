// ping pong ball game
#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct ball
{
    Vector2 pos;
    float rad;
    Vector2 vel;
} ball;

typedef struct paddle
{
    Vector2 pos;
    Vector2 size;
    Vector2 vel;
} pad;

typedef struct line
{
    Vector2 startposition;
    Vector2 endposition;

} line;

void updateBall(ball *ball)
{
    ball->pos.x += ball->vel.x;
    ball->pos.y += ball->vel.y;
    if (ball->pos.y - ball->rad <= 0 || ball->pos.y + ball->rad >= GetScreenHeight())
    {
        ball->vel.y *= -1;
    }
    if (ball->pos.x - ball->rad <= 0 || ball->pos.x + ball->rad >= GetScreenWidth())
    {
        ball->vel.x *= -1;
    }
}

void updateScore(ball *ball, int *player1, int *player2)
{
    if (ball->pos.x - ball->rad < 0)
    {
        (*player2)++;
        ball->pos = (Vector2){GetScreenWidth() / 2, GetScreenHeight() / 2};
    }
    if (ball->pos.x > GetScreenWidth())
    {
        (*player1)++;
        ball->pos = (Vector2){GetScreenWidth() / 2, GetScreenHeight() / 2};
    }
}

void updatePaddle_left(pad *pad)
{
    if (IsKeyDown(KEY_W))
    {
        if (!(pad->pos.y <= 0))
        {
            pad->pos.y -= pad->vel.y;
        }
    }
    if (IsKeyDown(KEY_S))
    {
        if (!(pad->pos.y + pad->size.y >= GetScreenHeight()))
        {
            pad->pos.y += pad->vel.y;
        }
    }
}

void updatePaddle_right(pad *pad)
{
    if (IsKeyDown(KEY_UP))
    {
        if (!(pad->pos.y <= 0))
        {
            pad->pos.y -= pad->vel.y;
        }
    }
    if (IsKeyDown(KEY_DOWN))
    {
        if (!(pad->pos.y + pad->size.y >= GetScreenHeight()))
        {
            pad->pos.y += pad->vel.y;
        }
    }
}

void Ai(pad *pad, ball *ball)
{
    if (pad->pos.y + pad->size.y / 2 > ball->pos.y + ball->rad)
    {
        if (!(pad->pos.y <= 0))
        {
            pad->pos.y -= pad->vel.y;
        }
    }
    if (pad->pos.y + pad->size.y / 2 < ball->pos.y + ball->rad)
    {
        if (!(pad->pos.y + pad->size.y >= GetScreenHeight()))
        {
            pad->pos.y += pad->vel.y;
        }
    }
}

void checkCollision(ball *ball, pad *pad)
{
    Rectangle rect = {pad->pos.x, pad->pos.y, pad->size.x, pad->size.y};
    if (CheckCollisionCircleRec(ball->pos, ball->rad, rect))
    {
        ball->vel.x *= -1;
        // ball->vel.y *= -1;
    }
}

int main()
{
    int sw = 720, sh = 480;
    InitWindow(sw, sh, "ping pong");
    SetTargetFPS(60);

    int player1 = 0, player2 = 0;

    ball ball = {{sw / 2, sh / 2}, 10, {4, 4}};

    pad pad1 = {{10, sh / 2 - 50}, {15, 100}, {4, 4}};

    pad pad2 = {{sw - 30, sh / 2 - 50}, {15, 100}, {4, 4}};

    line line = {{sw / 2, 0}, {sw / 2, sh}};

    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        // ball
        DrawCircleV(ball.pos, ball.rad, WHITE);
        updateBall(&ball);
        DrawLineV(line.startposition, line.endposition, WHITE);

        // padlles
        DrawRectangleV(pad1.pos, pad1.size, GRAY);
        updatePaddle_left(&pad1);
        DrawRectangleV(pad2.pos, pad2.size, GRAY);
        // updatePaddle_right(&pad2);
        Ai(&pad2, &ball);

        checkCollision(&ball, &pad1);
        checkCollision(&ball, &pad2);

        updateScore(&ball, &player1, &player2);
        DrawText(TextFormat("SCORE : %d", player1), sw / 4, 10, 20, WHITE);
        DrawText(TextFormat("SCORE : %d", player2), sw / 4 * 3, 10, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
}