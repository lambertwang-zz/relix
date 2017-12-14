#ifndef __COLOR_H__
#define __COLOR_H__

// Library
#include <limits.h>

// Engine
#include "string/string.h"

#define COLOR_BLANK (Color){0, 0, 0, 1.0}
#define COLOR_BLACK (Color){0, 0, 0, 1.0}
#define COLOR_EMPTY (Color){0, 0, 0, 0.0}
#define COLOR_WHITE (Color){255, 255, 255, 1.0}
#define COLOR_RED (Color){255, 0, 0, 1.0}
#define COLOR_GREEN (Color){0, 255, 0, 1.0}
#define COLOR_BLUE (Color){0, 0, 255, 1.0}

#define PIXEL_NULL (Pixel){16, 16, COLOR_EMPTY, COLOR_EMPTY, NULL, -1, INT_MIN}
#define PIXEL_BLANK (Pixel){16, 16, COLOR_BLANK, COLOR_BLANK, NULL, -1, 0}
#define PIXEL_WHITE (Pixel){231, 231, COLOR_EMPTY, COLOR_WHITE, NULL, -1, 0}

typedef struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    // Alpha is ignored for character rendering
    // a = [0..1]
    float a;
} Color;

typedef struct Pixel {
    // Are ignored if passed to a rendering function.
    unsigned char __fg;
    unsigned char __bg;

    Color c_fg;
    Color c_bg;

    String *chr;
    int id; // Object id at screen location
    // Depth 1024 is reserved for UI elements
    int depth; // Depthbuffer
} Pixel;

void copyPixel(Pixel *dest, const Pixel *src);

Color scaleColor(Color c, float f);

// Alpha is ignored when converting hsl/rgv to term color code
unsigned char rgbToTerm(const Color c);
unsigned char hslToTerm(Color c);

// Composite a over b
Color alphaComposite(Color a, Color b);

Color rgbToHsl(Color c);
Color hslToRgb(Color c);

#endif

