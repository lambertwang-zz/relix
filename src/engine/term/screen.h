#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "color.h"
#include "term.h"
#include "../geometry/geometry.h"

struct Screen {
    struct TermSize ts;

    int margin_x;
    int margin_y;

    Point camera;
    Rect camera_bounds;

    struct Pixel *pixelBuffer;

    // Refers to previous frame
    struct Pixel *prevPixelBuffer;

    int times_init;
};

struct Screen screen;

struct Screen *initScreen();
int closeScreen();

int swapScreen();

#endif

