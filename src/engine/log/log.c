#include "log.h"

// Library
#include <stdarg.h>
#include <stdio.h>

// Engine
#include "utility/clock.h"
#include "game/game.h"

void initLog() {
    flushLog = 1;
    logLevel = LOG_LEVEL_DEFAULT;
    relixLogFile = fopen(LOG_FILE, "w");

    writeLog(LOG_INIT, "log::initLog(): Initialized logging");
}

void closeLog() {
    writeLog(LOG_INIT, "log::closeLog(): Closing logging");
    fclose(relixLogFile);
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

int writeLog(int level, const char *format, ...) {
    char char_format[1024];
    
    va_list args;
    va_start(args, format);

    int charsPrinted = vsprintf(char_format, format, args);
    fflush(stderr);

    va_end(args);
    return nwriteLog(level, charsPrinted, char_format);
}

int nwriteLog(int level, unsigned int n, const char *format, ...) {
    if (!(logLevel & level)) {
        return -1;
    }
    if (relixLogFile == NULL) {
        return -1;
    }
                //if (this->p_f && this->isStarted()) {
    // Print timestring and frame number to log file
    // df::GameManager &game_manager = df::GameManager::getInstance();
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
    return chars_printed;
    // }
    // }
    // return -1;
}

