/*
 * Utilty functions
 */

// Libraries
#include <stdio.h>
#include <time.h>

#ifdef TARGET_OS_MAC
#include <sys/time.h>
#elif defined __linux__
//    #include <time.h>
#elif defined _WIN32 || defined _WIN64
#include <Windows.h>
#else
#error "unknown platform"
#endif

#include "utility.h"

namespace rlx {

// Returns a human-readable timestring
// This may only work on linux
char *getTimeString() {
    // Return string made static
    static char time_str[30];

    // System calls for current time
    time_t now;
    time(&now);

    struct tm *p_time = localtime(&now);

    // String formatting
    sprintf(time_str, "%02d:%02d:%02d", p_time->tm_hour, p_time->tm_min,
            p_time->tm_sec);

    return time_str;
}

unsigned long getCurrentTime() {
    // Calculates time in microseconds from seconds and nanoseconds
    #ifdef TARGET_OS_MAC
        struct timeval current_tv;
        gettimeofday(&current_tv, NULL);

        return current_tv.tv_sec * 1000 * 1000 + current_tv.tv_nsec / 1000;
    #elif defined __linux__
        struct timespec current_time;
        clock_gettime(CLOCK_REALTIME, &current_time);

        return current_time.tv_sec * 1000 * 1000 + current_time.tv_nsec / 1000;
    #elif defined _WIN32 || defined _WIN64
        SYSTEMTIME current_st;
        GetSystemTime(&current_st);

        // Calculates time in microseconds from minutes, seconds, and milliseconds
        return (current_st.wMinute * 60 * 1000 * 1000) + (current_st.wSecond * 1000 * 1000) +
               (current_st.wMilliseconds * 1000);
    #else
    #error "unknown platform"
    #endif
    return 0;
}

void uSleep(long microseconds) {
    #if defined TARGET_OS_MAC || defined __linux__
        // Construct a timespec with the specified nanoseconds
        struct timespec sleep_time;
        sleep_time.tv_sec = 0;
        sleep_time.tv_nsec = microseconds* 1000;
        nanosleep(&sleep_time, NULL);
    #elif defined _WIN32 || defined _WIN64
        long sleep_time = microseconds / 1000;
        if (sleep_time > 0) {
            Sleep(sleep_time);
        }
    #else
    #error "unknown platform"
    #endif
}

}
