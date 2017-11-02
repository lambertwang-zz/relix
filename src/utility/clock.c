/*
 * Clock class
 */

#include "clock.h"
#include "utility.h"

int initClock(struct Clock *c) {
    // Initialize time to current time
    c->previous_time = getCurrentTime();
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