#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "color.h"
#include "term.h"

#define MAX_WIDTH 1920
#define MAX_HEIGHT 600

struct Screen {
    struct TermSize ts;
    int margin_x;
    int margin_y;
    char *charBuffer;
    char *backCharBuffer;

    struct Pixel *pixelBuffer;
    struct Pixel *backPixelBuffer;

    int times_init;
};

struct Screen screen;

struct Screen *initScreen();
int closeScreen();

int swapScreen();

int putPixel(int x, int y, unsigned char c);
int putPixelRgb(int x, int y, Color c);

#endif

