#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "../geometry/geometry.h"

unsigned int id_iterator;

struct Object {
    unsigned int id;

    char chr;

    Point loc;

};

#endif

