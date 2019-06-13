#include "geometry.h"

namespace rlx {

int pointInRect(Point p, Rect r) {
    return p.y >= r.top &&
        p.y < r.bottom &&
        p.x >= r.left &&
        p.x < r.right;

}

int rectInRect(Rect a, Rect b) {
    return a.left < b.right &&
        b.left < a.right &&
        a.top < b.bottom &&
        b.top < a.bottom;

}

}
