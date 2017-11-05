#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "../geometry/geometry.h"

struct Object {
    unsigned int id;

    char chr;

    Point loc;
};

void initObject(struct Object *o);

#endif

