#ifndef __CLOCK_H__
#define __CLOCK_H__

typedef struct Clock {
    unsigned long previous_time;
} Clock;

char *timeString();
int initClock(Clock *c);

unsigned long split(Clock *c);

unsigned long delta(Clock *c);

#endif

