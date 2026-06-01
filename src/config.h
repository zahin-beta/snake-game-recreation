#ifndef CONFIG_H
#define CONFIG_H

// ---- compile-time constants ----
#define CELL_SIZE          30
#define CELL_COUNT         25
#define MAX_SNAKE_LENGTH   100
#define DIFFICULTY_INTERVAL 5
#define MIN_FRAME_DELAY    2

// ---- runtime config ----

typedef struct {
    int   cellSize;
    int   cellCount;
    int   maxSnakeLength;
    int   difficultyInterval;
    int   minFrameDelay;
    int   windowWidth;
    int   windowHeight;
    bool  wrapMode;
    bool  muteOnStart;
    float masterVolume;
} RuntimeConfig;

extern RuntimeConfig g_config;

void ConfigSetDefaults(void);
int  ConfigLoad(const char *path);
int  ConfigSave(const char *path);
void ConfigApply(void);

#endif
