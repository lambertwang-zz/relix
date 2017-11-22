#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "../events/events.h"
#include "../geometry/geometry.h"
#include "../term/color.h"

#define TYPE_MAX_LEN 32

struct Object {
    unsigned int id;
    char type[32];

    Point pos;
    Pixel pix;

    int (*render)(struct Object *);
    int (*update)(struct Object *);
    void (*close)(struct Object *);

    int (**event_listeners)(struct Object *m, const Event ev);
    int events_size;

    void *data;
};

int render_default(struct Object *o);
int update_default(struct Object *o);
void close_default(struct Object *o);

int listenEvent(struct Object *o, int ev_id, int (*listener)(struct Object *, const Event ev));
void initObject(struct Object *o);

#endif

