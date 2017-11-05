#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "../geometry/geometry.h"
#include "../term/color.h"

struct Object {
    unsigned int id;

    Point pos;
    Pixel pix;

    int (*render)(const struct Object *);
    int (*update)(const struct Object *);

    int (**event_listeners)(struct Object *);
    int events_size;
};

int render_default(const struct Object *o);

int listenEvent(struct Object *o, int ev_id, int (*listener)(struct Object *));
void initObject(struct Object *o);
void closeObject(struct Object *o);

#endif

