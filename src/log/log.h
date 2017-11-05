#ifndef __RELIXLOG_H__
#define __RELIXLOG_H__

#include <stdio.h>

#define LOG_FILE "debug_relix.log"

// Log level ids
// 0x0000ff00 bits reserved for verbose events
//      Every frame or every few frames
// 0x000000ff bits reserved for single-time events and errors
#define LOG_LEVEL_DEFAULT 0x000000ff
#define LOG_INIT 0x00000001
#define LOG_OBJECTMANAGER 0x00000002
#define LOG_OBJECTMANAGER_V 0x00000200
#define LOG_INPUT 0x00000004
#define LOG_INPUT_V 0x00000400
#define LOG_LOG 0x00000008

FILE *relixLogFile;
int flushLog;
int logLevel;

void initLog();
void closeLog();

void setFlush(unsigned int on);
void setLevel(int level);

int writeLog(int level, const char *format, ...);
int nwriteLog(int level, unsigned int n, const char *format, ...);

#endif

