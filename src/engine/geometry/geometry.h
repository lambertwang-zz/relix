#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

typedef struct Point {
    int x;
    int y;
    int z;
} Point;

// Note: top < bottom, left < right
typedef struct Rect {
    int top;
    int bottom;
    int left;
    int right;
} Rect;

int pointInRect(Point p, Rect r);

#endif

