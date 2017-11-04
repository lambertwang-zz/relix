#ifndef __RELIXLOG_H__
#define __RELIXLOG_H__

#include <stdio.h>

#define LOG_FILE "debug_relix.log"
#define LOG_LEVEL_DEFAULT 1

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

