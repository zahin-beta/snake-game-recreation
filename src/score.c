#include "score.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>

void ScoreInit(ScoreTable *st)
{
    st->count = 0;
    for (int i = 0; i < MAX_HIGH_SCORES; i++)
        st->scores[i] = 0;
}

int ScoreLoad(ScoreTable *st, const char *path)
{
    ScoreInit(st);
    FILE *f = fopen(path, "r");
    if (!f) return -1;

    char line[32];
    while (st->count < MAX_HIGH_SCORES && fgets(line, sizeof(line), f))
    {
        int val = atoi(line);
        if (val > 0) st->scores[st->count++] = val;
    }
    fclose(f);
    LOG_INFO("Loaded %d high scores from %s", st->count, path);
    return 0;
}

int ScoreSave(ScoreTable *st, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;

    for (int i = 0; i < st->count; i++)
        fprintf(f, "%d\n", st->scores[i]);

    fclose(f);
    LOG_INFO("Saved %d high scores", st->count);
    return 0;
}

bool ScoreIsHighScore(ScoreTable *st, int score)
{
    if (st->count < MAX_HIGH_SCORES) return true;
    for (int i = 0; i < st->count; i++)
        if (score > st->scores[i]) return true;
    return false;
}

void ScoreAdd(ScoreTable *st, int score)
{
    if (!ScoreIsHighScore(st, score)) return;

    int insertIdx = st->count;
    for (int i = 0; i < st->count; i++)
    {
        if (score > st->scores[i])
        {
            insertIdx = i;
            break;
        }
    }

    if (st->count < MAX_HIGH_SCORES) st->count++;

    for (int i = st->count - 1; i > insertIdx; i--)
        st->scores[i] = st->scores[i - 1];

    st->scores[insertIdx] = score;
}
