#ifndef __CLOCK_H__
#define __CLOCK_H__

struct Clock {
    unsigned long previous_time;
};

char *timeString();
int initClock(struct Clock *c);

unsigned long split(struct Clock *c);

unsigned long delta(struct Clock *c);

#endif

