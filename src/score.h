#ifndef SCORE_H
#define SCORE_H

#include <stdbool.h>

#define MAX_HIGH_SCORES 10

typedef struct {
    int scores[MAX_HIGH_SCORES];
    int count;
} ScoreTable;

void ScoreInit(ScoreTable *st);
int  ScoreLoad(ScoreTable *st, const char *path);
int  ScoreSave(ScoreTable *st, const char *path);
bool ScoreIsHighScore(ScoreTable *st, int score);
void ScoreAdd(ScoreTable *st, int score);

#endif
