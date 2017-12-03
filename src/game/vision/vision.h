#ifndef __VISION_H__
#define __VISION_H__

#include "../map/map.h"

int getOctanct(Point a, Point b);

int computeDiamondWalls(
    Map *map,
    int octant,
    Point origin,
    int range, 
    int x, 
    Point top, 
    Point bottom, 
    int (* illuminate)(Map *, Point, Point, int, float));
#endif

