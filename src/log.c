#include "log.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#define MAX_LOG_SIZE (1024 * 1024)

static FILE *logFile = NULL;
static LogLevel minLevel = LVL_DEBUG;

static const char *LevelStr(LogLevel l)
{
    switch (l)
    {
    case LVL_DEBUG: return "DEBUG";
    case LVL_INFO:  return "INFO";
    case LVL_WARN:  return "WARN";
    case LVL_ERROR: return "ERROR";
    default:        return "?";
    }
}

void LogInit(const char *filePath)
{
    if (filePath)
    {
        long size = 0;
        FILE *f = fopen(filePath, "rb");
        if (f)
        {
            fseek(f, 0, SEEK_END);
            size = ftell(f);
            fclose(f);
        }
        const char *mode = (size > MAX_LOG_SIZE) ? "w" : "a";
        logFile = fopen(filePath, mode);
    }
}

void LogSetLevel(LogLevel level)
{
    minLevel = level;
}

void LogWrite(LogLevel level, const char *file, int line, const char *fmt, ...)
{
    if (level < minLevel) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char msg[4096];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);

    const char *shortFile = file;
    const char *slash = strrchr(file, '/');
    if (slash) shortFile = slash + 1;
    slash = strrchr(shortFile, '\\');
    if (slash) shortFile = slash + 1;

    fprintf(stdout, "[%02d:%02d:%02d] %-5s %s:%d  %s\n",
            t->tm_hour, t->tm_min, t->tm_sec,
            LevelStr(level), shortFile, line, msg);

    if (logFile)
    {
        fprintf(logFile, "[%04d-%02d-%02d %02d:%02d:%02d] %-5s %s:%d  %s\n",
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min, t->tm_sec,
                LevelStr(level), shortFile, line, msg);
        fflush(logFile);
    }
}

void LogShutdown(void)
{
    if (logFile)
    {
        fclose(logFile);
        logFile = NULL;
    }
}
