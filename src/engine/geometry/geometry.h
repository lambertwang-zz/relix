#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

typedef struct Point {
    int x;
    int y;
    int z;
} Point;

typedef struct Line {
    int x1;
    int y1;
    int x2;
    int y2;
} Line;

// Note: top < bottom, left < right
typedef struct Rect {
    int top;
    int bottom;
    int left;
    int right;
} Rect;

int pointInRect(Point p, Rect r);
int rectInRect(Rect a, Rect b);

#endif

