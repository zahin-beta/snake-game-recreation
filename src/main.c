#include "game.h"
#include "raylib_api.h"
#include "config.h"
#include "log.h"
#include "result.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int GetAppDataPath(char *buf, int cap)
{
    const char *appdata = getenv("APPDATA");
    if (!appdata) return -1;
    snprintf(buf, cap, "%s/snake-game", appdata);
    return 0;
}

int main(void)
{
    // ---- paths ----
    char configDir[260];
    char configPath[260];
    char logPath[260];

    if (GetAppDataPath(configDir, sizeof(configDir)) == 0)
    {
        snprintf(configPath, sizeof(configPath), "%s/config.txt", configDir);
        snprintf(logPath,    sizeof(logPath),    "%s/log.txt",    configDir);
    }
    else
    {
        snprintf(configPath, sizeof(configPath), "config.txt");
        snprintf(logPath,    sizeof(logPath),    "log.txt");
    }

    // ---- init subsystems ----
    LogInit(logPath);
    LOG_INFO("Snake Game v2 starting");

    ConfigSetDefaults();
    ConfigLoad(configPath);
    ConfigApply();

    // ---- load raylib ----
    if (!LoadRaylib())
    {
        LOG_ERROR("Failed to load raylib DLL");
        LogShutdown();
        return 1;
    }
    LOG_INFO("Raylib loaded successfully");

    // ---- run game ----
    Game game;
    InitGame(&game);
    RunGame(&game);
    UnloadGame(&game);

    // ---- shutdown ----
    UnloadRaylib();
    ConfigSave(configPath);
    LOG_INFO("Shutdown complete");
    LogShutdown();
    return 0;
}
