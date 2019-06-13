#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

namespace rlx {
    typedef struct Point {
        double x;
        double y;
        double z;
    } Point;

    typedef struct Line {
        Point a;
        Point b;
    } Line;

    // Note: top < bottom, left < right
    typedef struct Rect {
        double top;
        double bottom;
        double left;
        double right;
    } Rect;

    int pointInRect(Point p, Rect r);
    int rectInRect(Rect a, Rect b);
}

#endif

