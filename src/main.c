//snake game remake in c
//@author: Zahin Bin Hasan
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int main ()
{
    int sw = 480, sh = 480;
    InitWindow(sw,sh,"SNAKE GAME");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(green);
        EndDrawing();
    }
    CloseWindow();
}