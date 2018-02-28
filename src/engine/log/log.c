#include "log.h"

// Library
#include <stdarg.h>
#include <stdio.h>

#if defined __linux__
#include <pthread.h>
#elif defined _WIN32 || defined _WIN64
#include <windows.h>
#else
#error "unknown platform"
#endif

// Engine
#include "utility/clock.h"
#include "game/game.h"

#if defined __linux__
static pthread_mutex_t log_mutex;
#elif defined _WIN32 || defined _WIN64
static HANDLE log_mutex;
#endif

void initLog() {
    flushLog = 1;
    logLevel = LOG_LEVEL_DEFAULT;
    relixLogFile = fopen(LOG_FILE, "w");

#if defined __linux__
    pthread_mutex_init(&log_mutex, NULL);
#elif defined _WIN32 || defined _WIN64
    log_mutex = CreateMutex(NULL, FALSE, NULL);
#endif

    writeLog(LOG_INIT, "log::initLog(): Initialized logging");
}

void closeLog() {
    writeLog(LOG_INIT, "log::closeLog(): Closing logging");
    fclose(relixLogFile);

#if defined __linux__
    pthread_mutex_destroy(&log_mutex);
#elif defined _WIN32 || defined _WIN64
    CloseHandle(log_mutex);
#endif
}

void setFlush(unsigned int on) {
    writeLog(LOG_LOG, "log::setFlush(): Set flush %d", on);
    flushLog = on;
}

void setLogLevel(int level) {
    writeLog(LOG_LOG, "log::setLogLevel(): Set log level 0x%x", level);
    logLevel = level;
}

void addLogLevel(int level) {
    writeLog(LOG_LOG, "log::addLogLevel(): Adding log level 0x%x", level);
    logLevel |= level;
}

void removeLogLevel(int level) {
    writeLog(LOG_LOG, "log::addLogLevel(): Removing log level 0x%x", level);
    logLevel &= ~level;
}

int writeLog(int level, const char *format, ...) {
    char char_format[1024];
    
    va_list args;
    va_start(args, format);

    // int charsPrinted = vsprintf(char_format, format, args);
    vsprintf(char_format, format, args);
    fflush(stderr);

    va_end(args);
    return nwriteLog(level, char_format);
}

// int nwriteLog(int level, unsigned int n, const char *format, ...) {
int nwriteLog(int level, const char *format, ...) {
    if (!(logLevel & level)) {
        return -1;
    }
    if (relixLogFile == NULL) {
        return -1;
    }

#if defined __linux__
    pthread_mutex_lock(&log_mutex);
#elif defined _WIN32 || defined _WIN64
    WaitForSingleObject(log_mutex, INFINITE);
#endif

    fprintf(relixLogFile, "%8s  %6d : ", timeString(), frame_count);

    // Format arguments
    va_list args;
    va_start(args, format);
    int chars_printed = vfprintf(relixLogFile, format, args);
    va_end(args);

    // Add newline to log file
    chars_printed += fwrite("\n", sizeof(char), 1, relixLogFile);
    
    // Flush log file is specified to do so
    if (flushLog) {
        fflush(relixLogFile);
    }

#if defined __linux__
    pthread_mutex_unlock(&log_mutex);
#elif defined _WIN32 || defined _WIN64
    ReleaseMutex(log_mutex);
#endif

    return chars_printed;
}

