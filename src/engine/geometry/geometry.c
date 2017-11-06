#include "geometry.h"

int pointInRect(Point p, Rect r) {
    return p.y >= r.top && p.y < r.bottom && p.x >= r.left && p.x < r.right;
}

