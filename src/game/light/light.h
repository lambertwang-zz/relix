#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "../map/map.h"

int pointLight(struct Screen *s, struct Map *map, struct Point origin, struct Color l, int range);

#endif

