#ifndef __RELIXLOG_H__
#define __RELIXLOG_H__

#include <stdio.h>

namespace rlx {
    #define LOG_FILE "debug_relix.log"

    // Log level ids
    // 0x0000ff00 bits reserved for verbose events
    //      Every frame or every few frames
    // 0x000000ff bits reserved for single-time events and errors
    // 0xffff0000 bits reserved for user log events
    #define LOG_LEVEL_DEFAULT   0x000000ff
    #define LOG_INIT            0x00000001
    #define LOG_INIT_V          0x00000100
    #define LOG_ObjectManager   0x00000002
    #define LOG_ObjectManager_V 0x00000200
    #define LOG_INPUT           0x00000004
    #define LOG_INPUT_V         0x00000400
    #define LOG_LOG             0x00000008
    #define LOG_GAME            0x00000010
    #define LOG_GAME_V          0x00001000
    #define LOG_SCREEN          0x00000020
    #define LOG_SCREEN_V        0x00002000
    #define LOG_RESOURCE        0x00000040
    #define LOG_RESOURCE_V      0x00004000
    #define LOG_UI              0x00000080
    #define LOG_UI_V            0x00008000

    void initLog();
    void closeLog();

    void setFlush(bool on);
    void setLogLevel(int level);
    void addLogLevel(int level);
    void removeLogLevel(int level);

    int writeLog(int level, const char *format, ...);
    // int nwriteLog(int level, unsigned int n, const char *format, ...);
    int nwriteLog(int level, const char *format, ...);
}

#endif

