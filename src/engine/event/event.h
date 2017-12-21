#ifndef __EVENTS_H__
#define __EVENTS_H__

typedef struct Event {
    unsigned int id;
    // Set by objectManager on initialization
    // If handler sets to 0, event is not passed on to next objects.
    // Note: There is no ordering for non-ui objects 
    int stop_propagation;
    void *data;
} Event;

#endif

