#ifndef __SCREEN_H__
#define __SCREEN_H__


#include "term.h"

#define MAX_WIDTH 140
#define MAX_HEIGHT 55

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct Pixel {
    struct Color col; 
    char chr;
};

struct Screen {
    struct TermSize ts;
    int margin_x;
    int margin_y;
    char *charBuffer;
    char *backCharBuffer;

    struct Color *colorBuffer;
    struct Color *backColorBuffer;

    int times_init;
};

struct Screen *getScreen();

struct Screen *initScreen();

int swapScreen();

#endif

