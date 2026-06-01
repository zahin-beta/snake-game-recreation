#include "game.h"
#include "collision.h"
#include "config.h"
#include "assets.h"
#include "raylib_api.h"
#include "log.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static Color green  = {173, 204, 96, 255};
static Color bgDark = {30, 40, 20, 255};
static Color accent = {100, 200, 50, 255};
static Color dim    = {0, 0, 0, 120};

#define MENU_ITEM_COUNT 4
#define SETTINGS_ITEM_COUNT 3

// ---- state machine ----

void SM_Init(StateMachine *sm, GameState initial)
{
    sm->current  = initial;
    sm->previous = initial;
}

void SM_Transition(StateMachine *sm, GameState next)
{
    if (next != sm->current)
    {
        sm->previous = sm->current;
        sm->current  = next;
        LOG_DEBUG("state: %d -> %d", sm->previous, sm->current);
    }
}

// ---- helpers ----

static int CenterX(const char *text, int fontSize)
{
    return (g_config.windowWidth - p_MeasureText(text, fontSize)) / 2;
}

static void PlayIfNotMuted(Sound sound, bool muted)
{
    if (!muted) p_PlaySound(sound);
}

static void InitializeGameFood(Game *g)
{
    g->food = CreateFood();
    g->food.pos = RandomGridPosAvoidSnake(&g->snake, &g->obstacles);
    PlayIfNotMuted(g->food.sound, g->muted);
}

static void ResetForNewGame(Game *g)
{
    g->score = 0;
    g->frameDelay = 10;
    g->frameCount = 0;
    g->newHighScore = false;
    g->level = 1;
    g->foodEatenThisLevel = 0;
    ObstaclesGenerate(&g->obstacles, g->level);
    InitSnake(&g->snake);
    g->food.pos = RandomGridPosAvoidSnake(&g->snake, &g->obstacles);
    SM_Transition(&g->state, STATE_PLAYING);
    LOG_INFO("Starting level %d", g->level);
}

// ---- draw helpers ----

static void DrawSelectionList(const char *title, const char **items, int count, int selection, int baseY, int titleSize, int itemSize)
{
    p_DrawText(title, CenterX(title, titleSize), baseY, titleSize, accent);

    for (int i = 0; i < count; i++)
    {
        int y = baseY + 80 + i * (itemSize + 20);
        Color c = (i == selection) ? YELLOW : WHITE;
        const char *prefix = (i == selection) ? "> " : "  ";
        char buf[128];
        snprintf(buf, sizeof(buf), "%s%s", prefix, items[i]);
        p_DrawText(buf, CenterX(buf, itemSize), y, itemSize, c);
    }
}

// ---- state: MENU ----

static const char *menuItems[MENU_ITEM_COUNT] = {
    "Start Game", "High Scores", "Settings", "Quit"
};

static void DrawMenu(Game *g)
{
    p_ClearBackground(bgDark);
    p_DrawText("SNAKE", CenterX("SNAKE", 70), 180, 70, accent);
    p_DrawText("A Classic", CenterX("A Classic", 16), 260, 16, (Color){180, 200, 150, 255});
    DrawSelectionList("", menuItems, MENU_ITEM_COUNT, g->menuSelection, 310, 0, 30);
}

static void RunMenu(Game *g)
{
    if (p_IsKeyPressed(KEY_UP))    g->menuSelection = (g->menuSelection - 1 + MENU_ITEM_COUNT) % MENU_ITEM_COUNT;
    if (p_IsKeyPressed(KEY_DOWN))  g->menuSelection = (g->menuSelection + 1) % MENU_ITEM_COUNT;

    if (p_IsKeyPressed(KEY_ENTER))
    {
        LOG_INFO("Enter pressed, selection=%d", g->menuSelection);
        switch (g->menuSelection)
        {
        case 0: ResetForNewGame(g); break;
        case 1: SM_Transition(&g->state, STATE_HIGH_SCORES); break;
        case 2: g->settingsSelection = 0; SM_Transition(&g->state, STATE_SETTINGS); break;
        case 3: SM_Transition(&g->state, STATE_EXIT); break;
        }
    }

    if (g->state.current == STATE_MENU)
        DrawMenu(g);
}

// ---- state: PLAYING ----

static void DrawHUD(Game *g)
{
    p_DrawText(p_TextFormat("SOUND: %s", g->muted ? "OFF" : "ON"), 10, g_config.windowHeight - 30, 18, BLACK);
    p_DrawText(p_TextFormat("WRAP: %s", g->wrapMode ? "ON" : "OFF"), 140, g_config.windowHeight - 30, 18, BLACK);
    p_DrawText(p_TextFormat("LVL: %d", g->level), 290, g_config.windowHeight - 30, 18, BLACK);
    p_DrawText("P Pause", g_config.windowWidth - 100, g_config.windowHeight - 30, 18, BLACK);
    p_DrawText(p_TextFormat("SCORE: %d", g->score), 10, 10, 25, BLACK);
}

static void RunPlaying(Game *g)
{
    g->frameCount++;
    HandleInput(&g->snake);

    if (g->frameCount >= g->frameDelay)
    {
        MoveSnake(&g->snake, g->wrapMode);
        g->frameCount = 0;

        if (!g->wrapMode && CheckWallCollision(&g->snake))
        {
            PlayIfNotMuted(g->dieSound, g->muted);
            if (ScoreIsHighScore(&g->highScores, g->score)) { ScoreAdd(&g->highScores, g->score); g->newHighScore = true; }
            SM_Transition(&g->state, STATE_GAME_OVER);
            return;
        }
        if (CheckObstacleCollision(&g->snake, &g->obstacles))
        {
            PlayIfNotMuted(g->dieSound, g->muted);
            if (ScoreIsHighScore(&g->highScores, g->score)) { ScoreAdd(&g->highScores, g->score); g->newHighScore = true; }
            SM_Transition(&g->state, STATE_GAME_OVER);
            return;
        }
    }

    p_ClearBackground(green);
    DrawObstacles(&g->obstacles, g->obstacleTexture);
    DrawFood(&g->food);
    DrawSnake(&g->snake);
    DrawHUD(g);

    if (CheckFoodCollision(&g->snake, &g->food))
    {
        g->score++;
        g->foodEatenThisLevel++;
        PlayIfNotMuted(g->eatSound, g->muted);

        if (g->score % DIFFICULTY_INTERVAL == 0 && g->frameDelay > MIN_FRAME_DELAY)
            g->frameDelay--;

        // Level advancement
        if (g->foodEatenThisLevel >= FOOD_PER_LEVEL && g->level < MAX_LEVELS)
        {
            g->level++;
            g->foodEatenThisLevel = 0;
            ObstaclesGenerate(&g->obstacles, g->level);
            if (g->frameDelay > MIN_FRAME_DELAY)
                g->frameDelay--;
            LOG_INFO("Advanced to level %d", g->level);
            // Ensure snake doesn't overlap new obstacles
            bool overlaps = false;
            for (int i = 0; i < g->snake.length && !overlaps; i++)
                for (int j = 0; j < g->obstacles.count && !overlaps; j++)
                    if (g->snake.body[i].x == g->obstacles.items[j].x && g->snake.body[i].y == g->obstacles.items[j].y)
                        overlaps = true;
            if (overlaps)
            {
                InitSnake(&g->snake);
                g->food.pos = RandomGridPosAvoidSnake(&g->snake, &g->obstacles);
                LOG_INFO("Snake repositioned after level up");
            }
        }

        g->food.pos = RandomGridPosAvoidSnake(&g->snake, &g->obstacles);
        PlayIfNotMuted(g->food.sound, g->muted);

        if (g->snake.length < MAX_SNAKE_LENGTH)
        {
            g->snake.body[g->snake.length] = g->snake.body[g->snake.length - 1];
            g->snake.length++;
        }
    }

    if (CheckSelfCollision(&g->snake))
    {
        PlayIfNotMuted(g->dieSound, g->muted);
        if (ScoreIsHighScore(&g->highScores, g->score)) { ScoreAdd(&g->highScores, g->score); g->newHighScore = true; }
        SM_Transition(&g->state, STATE_GAME_OVER);
    }
}

// ---- state: PAUSED ----

static const char *pauseItems[] = { "Resume", "Restart", "Settings", "Quit to Menu" };
#define PAUSE_ITEM_COUNT 4

static void DrawPauseOverlay(Game *g)
{
    p_DrawRectangle(0, 0, g_config.windowWidth, g_config.windowHeight, dim);
    p_DrawText("PAUSED", CenterX("PAUSED", 50), 180, 50, WHITE);

    for (int i = 0; i < PAUSE_ITEM_COUNT; i++)
    {
        int y = 260 + i * 45;
        Color c = (i == g->menuSelection) ? YELLOW : WHITE;
        const char *pfx = (i == g->menuSelection) ? "> " : "  ";
        char buf[64];
        snprintf(buf, sizeof(buf), "%s%s", pfx, pauseItems[i]);
        p_DrawText(buf, CenterX(buf, 28), y, 28, c);
    }
}

static void RunPaused(Game *g)
{
    p_ClearBackground(green);
    DrawFood(&g->food);
    DrawSnake(&g->snake);
    DrawHUD(g);
    DrawPauseOverlay(g);

    if (p_IsKeyPressed(KEY_UP))   g->menuSelection = (g->menuSelection - 1 + PAUSE_ITEM_COUNT) % PAUSE_ITEM_COUNT;
    if (p_IsKeyPressed(KEY_DOWN)) g->menuSelection = (g->menuSelection + 1) % PAUSE_ITEM_COUNT;

    if (p_IsKeyPressed(KEY_ENTER))
    {
        switch (g->menuSelection)
        {
        case 0: SM_Transition(&g->state, STATE_PLAYING); break;
        case 1: ResetForNewGame(g); break;
        case 2: g->settingsSelection = 0; SM_Transition(&g->state, STATE_SETTINGS); break;
        case 3:
            FlushInput(&g->snake);
            SM_Transition(&g->state, STATE_MENU);
            break;
        }
    }

    if (p_IsKeyPressed(KEY_P))
        SM_Transition(&g->state, STATE_PLAYING);
}

// ---- state: GAME_OVER ----

static void DrawGameOverSplash(Game *g)
{
    p_ClearBackground(BLACK);

    if (g->newHighScore)
    {
        p_DrawText("NEW HIGH SCORE!", CenterX("NEW HIGH SCORE!", 30), 180, 30, GOLD);
    }

    p_DrawText("GAME OVER", CenterX("GAME OVER", 50), 230, 50, WHITE);
    p_DrawText(p_TextFormat("SCORE: %d", g->score), CenterX(p_TextFormat("SCORE: %d", g->score), 36), 300, 36, WHITE);

    char bestStr[64];
    if (g->highScores.count > 0)
        snprintf(bestStr, sizeof(bestStr), "BEST: %d", g->highScores.scores[0]);
    else
        snprintf(bestStr, sizeof(bestStr), "BEST: --");
    p_DrawText(bestStr, CenterX(bestStr, 22), 350, 22, LIGHTGRAY);

    p_DrawText("R  Restart     Q  Menu", CenterX("R  Restart     Q  Menu", 24), 420, 24, YELLOW);
}

static void RunGameOver(Game *g)
{
    DrawGameOverSplash(g);

    if (p_IsKeyPressed(KEY_R)) ResetForNewGame(g);
    if (p_IsKeyPressed(KEY_Q))
    {
        FlushInput(&g->snake);
        SM_Transition(&g->state, STATE_MENU);
    }
}

// ---- state: HIGH_SCORES ----

static void DrawHighScores(Game *g)
{
    p_ClearBackground(bgDark);
    p_DrawText("HIGH SCORES", CenterX("HIGH SCORES", 40), 100, 40, accent);

    for (int i = 0; i < g->highScores.count && i < MAX_HIGH_SCORES; i++)
    {
        char buf[32];
        snprintf(buf, sizeof(buf), "%2d.  %d", i + 1, g->highScores.scores[i]);
        Color c = (i == 0) ? GOLD : ((i < 3) ? (Color){200, 200, 150, 255} : WHITE);
        p_DrawText(buf, CenterX(buf, 28), 180 + i * 34, 28, c);
    }

    if (g->highScores.count == 0)
        p_DrawText("No scores yet", CenterX("No scores yet", 24), 220, 24, LIGHTGRAY);

    p_DrawText("Press ESC to go back", CenterX("Press ESC to go back", 16), 560, 16, LIGHTGRAY);
}

static void RunHighScores(Game *g)
{
    DrawHighScores(g);
    if (p_IsKeyPressed(KEY_ESCAPE))
        SM_Transition(&g->state, STATE_MENU);
}

// ---- state: SETTINGS ----

static const char *settingLabels[SETTINGS_ITEM_COUNT] = {
    "Sound", "Wrap Mode", "Back"
};

static void DrawSettings(Game *g)
{
    p_ClearBackground(bgDark);
    p_DrawText("SETTINGS", CenterX("SETTINGS", 40), 100, 40, accent);

    char buf[128];
    for (int i = 0; i < SETTINGS_ITEM_COUNT; i++)
    {
        int y = 180 + i * 50;
        Color c = (i == g->settingsSelection) ? YELLOW : WHITE;
        const char *pfx = (i == g->settingsSelection) ? "> " : "  ";

        switch (i)
        {
        case 0:
            snprintf(buf, sizeof(buf), "%sSound: %s", pfx, g->muted ? "OFF" : "ON");
            break;
        case 1:
            snprintf(buf, sizeof(buf), "%sWrap Mode: %s", pfx, g->wrapMode ? "ON" : "OFF");
            break;
        case 2:
            snprintf(buf, sizeof(buf), "%s%s", pfx, settingLabels[i]);
            break;
        }
        p_DrawText(buf, CenterX(buf, 24), y, 24, c);
    }
    p_DrawText("Enter toggle, Esc back", CenterX("Enter toggle, Esc back", 14), 520, 14, LIGHTGRAY);
}

static void RunSettings(Game *g)
{
    DrawSettings(g);

    if (p_IsKeyPressed(KEY_UP))    g->settingsSelection = (g->settingsSelection - 1 + SETTINGS_ITEM_COUNT) % SETTINGS_ITEM_COUNT;
    if (p_IsKeyPressed(KEY_DOWN))  g->settingsSelection = (g->settingsSelection + 1) % SETTINGS_ITEM_COUNT;

    if (p_IsKeyPressed(KEY_ESCAPE))
    {
        GameState prev = g->state.previous;
        if (prev == STATE_PAUSED) g->menuSelection = 0;
        SM_Transition(&g->state, prev == STATE_PAUSED ? STATE_PAUSED : STATE_MENU);
        return;
    }

    switch (g->settingsSelection)
    {
    case 0:
        if (p_IsKeyPressed(KEY_ENTER)) g->muted = !g->muted;
        break;
    case 1:
        if (p_IsKeyPressed(KEY_ENTER)) g->wrapMode = !g->wrapMode;
        break;
    case 2:
        if (p_IsKeyPressed(KEY_ENTER))
        {
            GameState prev = g->state.previous;
            if (prev == STATE_PAUSED) g->menuSelection = 0;
            SM_Transition(&g->state, prev == STATE_PAUSED ? STATE_PAUSED : STATE_MENU);
        }
        break;
    }
}

// ---- lifecycle ----

void InitGame(Game *g)
{
    memset(g, 0, sizeof(*g));
    SM_Init(&g->state, STATE_MENU);

    p_InitWindow(g_config.windowWidth, g_config.windowHeight, "SNAKE GAME");
    p_SetTargetFPS(60);
    p_InitAudioDevice();

    g->eatSound = LoadSoundFromMem(EAT_OGG_DATA, EAT_OGG_SIZE, ".ogg");
    g->dieSound = LoadSoundFromMem(DIE_OGG_DATA, DIE_OGG_SIZE, ".ogg");
    g->frameDelay = 10;
    g->muted = g_config.muteOnStart;
    g->wrapMode = g_config.wrapMode;
    g->menuSelection = 0;

    // high score path
    const char *appdata = getenv("APPDATA");
    if (appdata)
        snprintf(g->highScorePath, sizeof(g->highScorePath), "%s/snake-game/highscores.txt", appdata);
    else
        snprintf(g->highScorePath, sizeof(g->highScorePath), "highscores.txt");

    ScoreInit(&g->highScores);
    ScoreLoad(&g->highScores, g->highScorePath);

    // obstacles
    g->level = 1;
    g->foodEatenThisLevel = 0;
    g->obstacleTexture = LoadTextureFromMem(OBSTACLE_PNG_DATA, OBSTACLE_PNG_SIZE, ".png");
    ObstaclesGenerate(&g->obstacles, g->level);

    InitializeGameFood(g);
    InitSnake(&g->snake);

    LOG_INFO("Game initialized (window %dx%d) level=%d", g_config.windowWidth, g_config.windowHeight, g->level);
}

void RunGame(Game *g)
{
    LOG_INFO("Entering game loop");

    while (g->state.current != STATE_EXIT && !p_WindowShouldClose())
    {
        p_BeginDrawing();

        // Global keys
        if (p_IsKeyPressed(KEY_M)) g->muted = !g->muted;
        if (p_IsKeyPressed(KEY_W)) g->wrapMode = !g->wrapMode;

        // Pause from PLAYING
        if (p_IsKeyPressed(KEY_P) && g->state.current == STATE_PLAYING)
        {
            FlushInput(&g->snake);
            g->menuSelection = 0;
            SM_Transition(&g->state, STATE_PAUSED);
        }

        switch (g->state.current)
        {
        case STATE_MENU:       RunMenu(g);        break;
        case STATE_PLAYING:    RunPlaying(g);     break;
        case STATE_PAUSED:     RunPaused(g);      break;
        case STATE_GAME_OVER:  RunGameOver(g);    break;
        case STATE_HIGH_SCORES: RunHighScores(g); break;
        case STATE_SETTINGS:   RunSettings(g);    break;
        default: break;
        }

        p_EndDrawing();
    }

    LOG_INFO("Exited game loop (state=%d)", g->state.current);
}

void UnloadGame(Game *g)
{
    // Save high score if game-over produced a new one
    if (g->newHighScore)
        ScoreSave(&g->highScores, g->highScorePath);

    p_UnloadTexture(g->food.texture);
    p_UnloadTexture(g->obstacleTexture);
    p_UnloadSound(g->eatSound);
    p_UnloadSound(g->dieSound);
    p_UnloadSound(g->food.sound);
    p_CloseWindow();
    LOG_INFO("Game resources unloaded");
}
