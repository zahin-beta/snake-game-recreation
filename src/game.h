#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "snake.h"
#include "food.h"
#include "score.h"
#include "obstacle.h"

// ---- state machine ----

typedef enum {
    STATE_MENU = 0,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAME_OVER,
    STATE_HIGH_SCORES,
    STATE_SETTINGS,
    STATE_EXIT,
} GameState;

typedef struct {
    GameState current;
    GameState previous;
} StateMachine;

void SM_Init(StateMachine *sm, GameState initial);
void SM_Transition(StateMachine *sm, GameState next);

// ---- game ----

typedef struct {
    StateMachine state;
    Snake snake;
    Food food;
    int score;
    int frameCount;
    int frameDelay;
    bool muted;
    bool wrapMode;
    Sound eatSound;
    Sound dieSound;

    // UI state
    int menuSelection;
    int settingsSelection;

    // level / obstacles
    int level;
    int foodEatenThisLevel;
    ObstacleTable obstacles;
    Texture2D obstacleTexture;

    // high scores
    ScoreTable highScores;
    char highScorePath[260];
    bool newHighScore;
} Game;

void InitGame(Game *g);
void RunGame(Game *g);
void UnloadGame(Game *g);

#endif
