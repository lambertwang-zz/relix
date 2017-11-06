#include "clock.h"
#include "utility.h"

#include "time.h"
#include "stdio.h"

char *timeString() {
    static char time_str[30];

    time_t now;
    time(&now);

    struct tm *p_time = localtime(&now);

    // Format into human readable string
    sprintf(time_str, "%02d:%02d:%02d", p_time->tm_hour, p_time->tm_min, p_time->tm_sec);

    return time_str;
}

int initClock(struct Clock *c) {
    // Initialize time to current time
    c->previous_time = getCurrentTime();
    return 0;
}

unsigned long split(struct Clock *c) {
    // Calculate difference
    unsigned long elapsed_time = getCurrentTime() - c->previous_time;
    // If elapsed time is zero or positive, return elapsed time
    return elapsed_time >= 0 ? elapsed_time : -1;
}

unsigned long delta(struct Clock *c) {
    // Call split to get time difference
    unsigned long elapsed_time = split(c);
    // Reset previous time
    c->previous_time = getCurrentTime();
    // If elapsed time is zero or positive, return elapsed time
    return elapsed_time;
}
