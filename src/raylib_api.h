#ifndef RAYLIB_API_H
#define RAYLIB_API_H

#include <stdbool.h>

// Define raylib types ourselves unless raylib.h was already included.
// This avoids including <raylib.h> in files that also need <windows.h>,
// which has conflicting name macros (CloseWindow, DrawText, etc.).
#ifndef RAYLIB_H
typedef struct { unsigned char r, g, b, a; } Color;
typedef struct { float x, y; } Vector2;
typedef struct { unsigned int id; int width, height, mipmaps, format; } Texture2D;
typedef struct { void *data; int width, height, mipmaps, format; } Image;
typedef struct { unsigned int sampleCount, sampleRate, sampleSize, channels; void *data; } Wave;
typedef struct { void *buffer; unsigned int sampleRate, sampleSize, channels; int format; } AudioStream;
typedef struct { AudioStream stream; unsigned int sampleCount; } Sound;
#endif

typedef void (*p_InitWindow_t)(int width, int height, const char *title);
typedef void (*p_SetTargetFPS_t)(int fps);
typedef void (*p_InitAudioDevice_t)(void);
typedef bool (*p_WindowShouldClose_t)(void);
typedef void (*p_BeginDrawing_t)(void);
typedef void (*p_EndDrawing_t)(void);
typedef void (*p_ClearBackground_t)(Color color);
typedef void (*p_CloseWindow_t)(void);
typedef int  (*p_GetRandomValue_t)(int min, int max);
typedef void (*p_DrawRectangle_t)(int posX, int posY, int width, int height, Color color);
typedef void (*p_DrawTexture_t)(Texture2D texture, int posX, int posY, Color tint);
typedef void (*p_DrawText_t)(const char *text, int posX, int posY, int fontSize, Color color);
typedef void (*p_DrawCircle_t)(int centerX, int centerY, float radius, Color color);
typedef const char *(*p_TextFormat_t)(const char *text, ...);
typedef int  (*p_MeasureText_t)(const char *text, int fontSize);
typedef bool (*p_IsKeyPressed_t)(int key);
typedef void (*p_PlaySound_t)(Sound sound);
typedef Image (*p_LoadImageFromMemory_t)(const char *fileType, const unsigned char *fileData, int dataSize);
typedef Texture2D (*p_LoadTextureFromImage_t)(Image image);
typedef void (*p_UnloadImage_t)(Image image);
typedef Wave (*p_LoadWaveFromMemory_t)(const char *fileType, const unsigned char *fileData, int dataSize);
typedef Sound (*p_LoadSoundFromWave_t)(Wave wave);
typedef void (*p_UnloadWave_t)(Wave wave);
typedef void (*p_UnloadTexture_t)(Texture2D texture);
typedef void (*p_UnloadSound_t)(Sound sound);

extern p_InitWindow_t p_InitWindow;
extern p_SetTargetFPS_t p_SetTargetFPS;
extern p_InitAudioDevice_t p_InitAudioDevice;
extern p_WindowShouldClose_t p_WindowShouldClose;
extern p_BeginDrawing_t p_BeginDrawing;
extern p_EndDrawing_t p_EndDrawing;
extern p_ClearBackground_t p_ClearBackground;
extern p_CloseWindow_t p_CloseWindow;
extern p_GetRandomValue_t p_GetRandomValue;
extern p_DrawRectangle_t p_DrawRectangle;
extern p_DrawTexture_t p_DrawTexture;
extern p_DrawText_t p_DrawText;
extern p_DrawCircle_t p_DrawCircle;
extern p_TextFormat_t p_TextFormat;
extern p_MeasureText_t p_MeasureText;
extern p_IsKeyPressed_t p_IsKeyPressed;
extern p_PlaySound_t p_PlaySound;
extern p_LoadImageFromMemory_t p_LoadImageFromMemory;
extern p_LoadTextureFromImage_t p_LoadTextureFromImage;
extern p_UnloadImage_t p_UnloadImage;
extern p_LoadWaveFromMemory_t p_LoadWaveFromMemory;
extern p_LoadSoundFromWave_t p_LoadSoundFromWave;
extern p_UnloadWave_t p_UnloadWave;
extern p_UnloadTexture_t p_UnloadTexture;
extern p_UnloadSound_t p_UnloadSound;

Texture2D LoadTextureFromMem(const unsigned char *data, int size, const char *ext);
Sound    LoadSoundFromMem(const unsigned char *data, int size, const char *ext);

bool LoadRaylib(void);
void UnloadRaylib(void);

#endif
