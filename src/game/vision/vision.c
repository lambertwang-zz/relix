#include "vision.h"

// Library
#include <math.h>

int vecGrEq(Point a, Point b) {
    return a.y * b.x >= a.x * b.y;
}

int vecLtEq(Point a, Point b) {
    return a.y * b.x <= a.x * b.y;
}

int vecGr(Point a, Point b) {
    return a.y * b.x > a.x * b.y;
}

// top and bottom angles = [1.0, 0.0]
int computeDiamondWalls(
    Map *map,
    int octant,
    Point origin,
    int range, 
    int x, 
    Point top, 
    Point bottom, 
    int (* illuminate)(Map *, Point, Point, int, float)) {
    for(x = x; x <= range; x++) {
        int top_y;
        if(top.x == 1) {
            // Top slope is 1
            top_y = x;
        } else {
            // get the tile that the top vector enters from the left
            top_y = ((x * 2 - 1) * top.y + top.x) / (top.x * 2); 
            int ay = (top_y * 2 + 1) * top.x;

            if (isSolidOct(map, (Point){x, top_y}, octant, origin)) {
                // if the top tile is a wall...
                if(vecGrEq(top, (Point){x * 2, ay})) {
                    // but the top vector misses the wall and passes into the tile above, move up
                    top_y++; 
                }
            }
            else {
                // the top tile is not a wall
                if(vecGr(top, (Point){x * 2 + 1, ay})) {
                    // so if the top vector passes into the tile above, move up
                    top_y++; 
                }
            }
        }

        int bottom_y = bottom.y == 0 ? 0 : ((x*2-1) * bottom.y + bottom.x) / (bottom.x*2);
        int wasOpaque = -1; // 0:false, 1:true, -1:not applicable
        int y;
        for(y = top_y; y >= bottom_y; y--) {
            int tx = origin.x;
            int ty = origin.y;
            switch(octant) {
                case 0: tx += x; ty -= y; break;
                case 1: tx += y; ty -= x; break;
                case 2: tx -= y; ty -= x; break;
                case 3: tx -= x; ty -= y; break;
                case 4: tx -= x; ty += y; break;
                case 5: tx -= y; ty += x; break;
                case 6: tx += y; ty += x; break;
                case 7: tx += x; ty += y; break;
            }
            
            float distance = sqrt(x * x + y * y);
            int inRange = (range < 0 || distance <= (float) range) ? 1 : 0;
            // NOTE: use the following line instead to make the algorithm symmetrical
            // if (inRange && (y != top.y || vecGrEq(top, (Point){x, y})) && (y != bottom_y || vecLtEq(bottom, (Point){x, y}))) {
            if (inRange) {
                int result = illuminate(map, origin, (Point){tx, ty}, range, distance); 
                if (result) {
                    return result;
                }
            }

            int isOpaque = (!inRange || isSolid(map, (Point){tx, ty})) ? 1 : 0;
            // if y == top_y or y == bottom_y, make sure the sector actually intersects the wall tile. if not, don't consider
            // it opaque to prevent the code below from moving the top vector up or the bottom vector down
            if (isOpaque &&
                ((y == top_y && vecLtEq(top, (Point){x * 2, y * 2 - 1}) && !isSolidOct(map, (Point){x, y - 1}, octant, origin)) ||
                (y == bottom_y && vecGrEq(bottom, (Point){x * 2, y * 2 + 1}) && !isSolidOct(map, (Point){x, y + 1}, octant, origin)))) {
                isOpaque = 0;
            }

            if (x != range) {
                if (isOpaque) {
                    if (wasOpaque == 0)  {
                        // if we found a transition from clear to opaque, this sector is done in this column, so
                        // adjust the bottom vector upwards and continue processing it in the next column.
                        // (x*2-1, y*2+1) is a vector to the top-left corner of the opaque block
                        if(!inRange || y == bottom_y) { 
                            // don't recurse unless necessary
                            bottom = (Point){x * 2, y * 2 + 1}; 
                            break; 
                        } else {
                            int result = computeDiamondWalls(map, octant, origin, range, x+1, top, (Point){x * 2, y * 2 + 1}, illuminate);
                            if (result) {
                                return result;
                            }
                        }
                    }
                    wasOpaque = 1;
                } else {   
                    // adjust the top vector downwards and continue if we found a transition from opaque to clear 
                    // (x*2+1, y*2+1) is the top-right corner of the clear tile (i.e. the bottom-right of the opaque tile)
                    if(wasOpaque > 0) {
                        top = (Point){x * 2, y*2+1};
                    }
                    wasOpaque = 0;
                }
            }
        }

        if(wasOpaque != 0) {
            // if the column ended in a clear tile, continue processing the current sector
            break;
        }
    }
    return 0;
}

