// Library
#include <time.h>
#include <stdio.h>

// Engine
#include "utility.h"

#include "clock.h"

char *timeString() {
    static char time_str[30];

    time_t now;
    time(&now);

    struct tm *p_time = localtime(&now);

    // Format into human readable string
    sprintf(time_str, "%02d:%02d:%02d", p_time->tm_hour, p_time->tm_min, p_time->tm_sec);

    return time_str;
}

int initClock(Clock *c) {
    // Initialize time to current time
    c->previous_time = getCurrentTime();
    return 0;
}

unsigned long split(Clock *c) {
    // Calculate difference
    unsigned long elapsed_time = getCurrentTime() - c->previous_time;
    return elapsed_time;
}

unsigned long delta(Clock *c) {
    // Call split to get time difference
    unsigned long elapsed_time = split(c);
    // Reset previous time
    c->previous_time = getCurrentTime();
    return elapsed_time;
}

