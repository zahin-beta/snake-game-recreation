#ifndef LOG_H
#define LOG_H

typedef enum {
    LVL_DEBUG = 0,
    LVL_INFO,
    LVL_WARN,
    LVL_ERROR,
} LogLevel;

void LogInit(const char *filePath);
void LogSetLevel(LogLevel level);
void LogWrite(LogLevel level, const char *file, int line, const char *fmt, ...);
void LogShutdown(void);

#define LOG_DEBUG(...)  LogWrite(LVL_DEBUG,  __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)   LogWrite(LVL_INFO,   __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)   LogWrite(LVL_WARN,   __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...)  LogWrite(LVL_ERROR,  __FILE__, __LINE__, __VA_ARGS__)

#endif
