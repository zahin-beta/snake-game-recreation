#include "config.h"
#include "log.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

RuntimeConfig g_config;

void ConfigSetDefaults(void)
{
    g_config.cellSize          = CELL_SIZE;
    g_config.cellCount         = CELL_COUNT;
    g_config.maxSnakeLength    = MAX_SNAKE_LENGTH;
    g_config.difficultyInterval = DIFFICULTY_INTERVAL;
    g_config.minFrameDelay     = MIN_FRAME_DELAY;
    g_config.windowWidth       = CELL_SIZE * CELL_COUNT;
    g_config.windowHeight      = CELL_SIZE * CELL_COUNT;
    g_config.wrapMode          = true;
    g_config.muteOnStart       = false;
    g_config.masterVolume      = 1.0f;
}

static char *trim(char *s)
{
    while (*s && isspace((unsigned char)*s)) s++;
    if (*s == '\0') return s;
    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
    return s;
}

int ConfigLoad(const char *path)
{
    ConfigSetDefaults();

    FILE *f = fopen(path, "r");
    if (!f)
    {
        LOG_INFO("No config file at %s, using defaults", path);
        return 0;
    }

    char line[256];
    int lineNum = 0;
    while (fgets(line, sizeof(line), f))
    {
        lineNum++;
        char *p = trim(line);
        if (*p == '\0' || *p == '#') continue;

        char *eq = strchr(p, '=');
        if (!eq)
        {
            LOG_WARN("config:%d: no '=' found, skipping", lineNum);
            continue;
        }

        *eq = '\0';
        char *key = trim(p);
        char *val = trim(eq + 1);

        if      (strcmp(key, "wrap_mode")       == 0) g_config.wrapMode          = (strcmp(val, "true") == 0);
        else if (strcmp(key, "mute_on_start")   == 0) g_config.muteOnStart       = (strcmp(val, "true") == 0);
        else if (strcmp(key, "master_volume")   == 0) g_config.masterVolume      = (float)atof(val);
        else if (strcmp(key, "window_width")    == 0) g_config.windowWidth       = atoi(val);
        else if (strcmp(key, "window_height")   == 0) g_config.windowHeight      = atoi(val);
        else if (strcmp(key, "max_snake_length") == 0) g_config.maxSnakeLength   = atoi(val);
        else LOG_WARN("config:%d: unknown key '%s'", lineNum, key);
    }

    fclose(f);
    LOG_INFO("Loaded config from %s", path);
    return 0;
}

int ConfigSave(const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;

    fprintf(f, "# snake-game configuration\n");
    fprintf(f, "wrap_mode        = %s\n",      g_config.wrapMode       ? "true" : "false");
    fprintf(f, "mute_on_start    = %s\n",      g_config.muteOnStart    ? "true" : "false");
    fprintf(f, "master_volume    = %.2f\n",    g_config.masterVolume);
    fprintf(f, "window_width     = %d\n",      g_config.windowWidth);
    fprintf(f, "window_height    = %d\n",      g_config.windowHeight);
    fprintf(f, "max_snake_length = %d\n",      g_config.maxSnakeLength);

    fclose(f);
    LOG_INFO("Saved config to %s", path);
    return 0;
}

void ConfigApply(void)
{
    LOG_INFO("Config applied: wrap=%s volume=%.2f size=%dx%d",
             g_config.wrapMode ? "ON" : "OFF",
             g_config.masterVolume,
             g_config.windowWidth,
             g_config.windowHeight);
}
