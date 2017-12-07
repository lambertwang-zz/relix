#ifndef __OBJECT_H__
#define __OBJECT_H__

// Engine
#include "event/event.h"
#include "term/screen.h"

#define TYPE_MAX_LEN 32

#define OBJ_SOLID 1
#define OBJ_ETHER 0 // Ethereal
#define OBJ_SOFT 0

typedef struct Object {
    unsigned int id;
    char type[32];

    Point pos;
    Pixel pix;

    // Whether or not the object should generate collisions
    int solid;

    int (*renderLight)(struct Object *, Screen *);
    int (*render)(struct Object *, Screen *);
    int (*update)(struct Object *);
    void (*close)(struct Object *);

    Tree event_listeners;
    // int (**event_listeners)(struct Object *m, Event ev);
    // int events_size;

    void *data;
} Object;

int renderLightDefault(Object *o, Screen *s);
int renderDefault(Object *o, Screen *s);
int updateDefault(Object *o);
void closeDefault(Object *o);

int listenEvent(Object *o, int ev_id, int (*listener)(Object *, Event ev));
Object *createObject();

#endif

