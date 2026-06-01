#include "raylib_api.h"
#include "assets.h"
#include "log.h"
#include "result.h"

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOMINMAX
#include <windows.h>
#include <stdio.h>

// ---- function pointer definitions ----

p_InitWindow_t p_InitWindow;
p_SetTargetFPS_t p_SetTargetFPS;
p_InitAudioDevice_t p_InitAudioDevice;
p_WindowShouldClose_t p_WindowShouldClose;
p_BeginDrawing_t p_BeginDrawing;
p_EndDrawing_t p_EndDrawing;
p_ClearBackground_t p_ClearBackground;
p_CloseWindow_t p_CloseWindow;
p_GetRandomValue_t p_GetRandomValue;
p_DrawRectangle_t p_DrawRectangle;
p_DrawTexture_t p_DrawTexture;
p_DrawText_t p_DrawText;
p_DrawCircle_t p_DrawCircle;
p_TextFormat_t p_TextFormat;
p_MeasureText_t p_MeasureText;
p_IsKeyPressed_t p_IsKeyPressed;
p_PlaySound_t p_PlaySound;
p_LoadImageFromMemory_t p_LoadImageFromMemory;
p_LoadTextureFromImage_t p_LoadTextureFromImage;
p_UnloadImage_t p_UnloadImage;
p_LoadWaveFromMemory_t p_LoadWaveFromMemory;
p_LoadSoundFromWave_t p_LoadSoundFromWave;
p_UnloadWave_t p_UnloadWave;
p_UnloadTexture_t p_UnloadTexture;
p_UnloadSound_t p_UnloadSound;

// ---- DLL loading ----

static HMODULE raylibModule = NULL;
static char dllPathCache[MAX_PATH] = {0};

static bool ResolveAll(HMODULE mod)
{
#define RESOLVE(name) do { p_##name = (p_##name##_t)GetProcAddress(mod, #name); if (!p_##name) { LOG_ERROR("Missing symbol: %s", #name); return false; } } while(0)
    RESOLVE(InitWindow);
    RESOLVE(SetTargetFPS);
    RESOLVE(InitAudioDevice);
    RESOLVE(WindowShouldClose);
    RESOLVE(BeginDrawing);
    RESOLVE(EndDrawing);
    RESOLVE(ClearBackground);
    RESOLVE(CloseWindow);
    RESOLVE(GetRandomValue);
    RESOLVE(DrawRectangle);
    RESOLVE(DrawTexture);
    RESOLVE(DrawText);
    RESOLVE(DrawCircle);
    RESOLVE(TextFormat);
    RESOLVE(MeasureText);
    RESOLVE(IsKeyPressed);
    RESOLVE(PlaySound);
    RESOLVE(LoadImageFromMemory);
    RESOLVE(LoadTextureFromImage);
    RESOLVE(UnloadImage);
    RESOLVE(LoadWaveFromMemory);
    RESOLVE(LoadSoundFromWave);
    RESOLVE(UnloadWave);
    RESOLVE(UnloadTexture);
    RESOLVE(UnloadSound);
    return true;
#undef RESOLVE
}

bool LoadRaylib(void)
{
    char tempDir[MAX_PATH];
    if (!GetTempPathA(MAX_PATH, tempDir))
    {
        LOG_ERROR("GetTempPathA failed");
        return false;
    }

    snprintf(dllPathCache, MAX_PATH, "%ssnake_raylib_%lu.dll", tempDir, GetCurrentProcessId());
    LOG_DEBUG("Extracting raylib DLL to %s", dllPathCache);

    FILE *f = fopen(dllPathCache, "wb");
    if (!f)
    {
        LOG_ERROR("Failed to write %s", dllPathCache);
        return false;
    }
    size_t written = fwrite(RAYLIB_DLL_DATA, 1, RAYLIB_DLL_SIZE, f);
    fclose(f);

    if ((unsigned int)written != RAYLIB_DLL_SIZE)
    {
        LOG_ERROR("Wrote %zu bytes, expected %u", written, RAYLIB_DLL_SIZE);
        DeleteFileA(dllPathCache);
        return false;
    }

    raylibModule = LoadLibraryA(dllPathCache);
    if (!raylibModule)
    {
        LOG_ERROR("LoadLibraryA failed (error %lu)", GetLastError());
        DeleteFileA(dllPathCache);
        return false;
    }

    if (!ResolveAll(raylibModule))
    {
        LOG_ERROR("Function pointer resolution failed");
        FreeLibrary(raylibModule);
        raylibModule = NULL;
        DeleteFileA(dllPathCache);
        return false;
    }

    LOG_INFO("Raylib DLL loaded and all symbols resolved");
    return true;
}

void UnloadRaylib(void)
{
    if (raylibModule)
    {
        FreeLibrary(raylibModule);
        raylibModule = NULL;
        if (dllPathCache[0])
        {
            DeleteFileA(dllPathCache);
            LOG_DEBUG("Deleted temp DLL %s", dllPathCache);
        }
    }
}

// ---- helper implementations ----

Texture2D LoadTextureFromMem(const unsigned char *data, int size, const char *ext)
{
    Image img = p_LoadImageFromMemory(ext, data, size);
    Texture2D tex = p_LoadTextureFromImage(img);
    p_UnloadImage(img);
    return tex;
}

Sound LoadSoundFromMem(const unsigned char *data, int size, const char *ext)
{
    Wave wave = p_LoadWaveFromMemory(ext, data, size);
    Sound snd = p_LoadSoundFromWave(wave);
    p_UnloadWave(wave);
    return snd;
}
