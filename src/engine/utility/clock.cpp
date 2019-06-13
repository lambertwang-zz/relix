// Library
#include <time.h>
#include <stdio.h>

// Engine
#include "utility.h"

#include "clock.h"

namespace rlx {

char *timeString() {
    static char time_str[30];

    time_t now;
    time(&now);

    struct tm *p_time = localtime(&now);

    // Format into human readable string
    sprintf(time_str, "%02d:%02d:%02d", p_time->tm_hour, p_time->tm_min, p_time->tm_sec);

    return time_str;
}

Clock::Clock() {
    // Initialize time to current time
    previous_time = getCurrentTime();
}

unsigned long Clock::split() const {
    // Calculate difference
    unsigned long elapsed_time = getCurrentTime() - previous_time;
    return elapsed_time;
}

unsigned long Clock::delta() {
    // Call split to get time difference
    unsigned long elapsed_time = split();
    // Reset previous time
    previous_time = getCurrentTime();
    return elapsed_time;
}

}
