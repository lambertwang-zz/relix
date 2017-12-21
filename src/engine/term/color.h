#ifndef __COLOR_H__
#define __COLOR_H__

// Library
#include <limits.h>

// Engine
#include "string/string.h"
#include "constants.h"

#define COLOR_BLANK (Color){0, 0, 0, 1.0}
#define COLOR_BLACK (Color){0, 0, 0, 1.0}
#define COLOR_EMPTY (Color){0, 0, 0, 0.0}
#define COLOR_WHITE (Color){255, 255, 255, 1.0}
#define COLOR_RED (Color){255, 0, 0, 1.0}
#define COLOR_GREEN (Color){0, 255, 0, 1.0}
#define COLOR_BLUE (Color){0, 0, 255, 1.0}

// Always returns -1 when compared
#define COLOR_UNDEFINED (Color){0, 0, 0, -1.0}

// #define PIXEL_NULL (Pixel){16, 16, COLOR_EMPTY, COLOR_EMPTY, NULL, -1, INT_MIN}
// #define PIXEL_BLANK (Pixel){16, 16, COLOR_BLANK, COLOR_BLANK, NULL, -1, 0}
// #define PIXEL_WHITE (Pixel){231, 231, COLOR_EMPTY, COLOR_WHITE, NULL, -1, 0}
#define PIXEL_NULL (Pixel){COLOR_UNDEFINED, COLOR_UNDEFINED, " ", -1, INT_MIN}
#define PIXEL_BLANK (Pixel){COLOR_BLANK, COLOR_BLANK, " ", -1, 0}
#define PIXEL_WHITE (Pixel){COLOR_EMPTY, COLOR_WHITE, " ", -1, 0}

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
    // UPDATE: Completely Ignored. Game now renders 24 bit colors!
    // TODO: Also support 8 bit terminals
    // unsigned char __fg;
    // unsigned char __bg;

    Color fg;
    Color bg;

    // chr MUST be a character with 1 display width
    char chr[UNICODE_MAX];
    int id; // Object id at screen location
    // Depth 1024 is reserved for UI elements
    int depth; // Depthbuffer
} Pixel;

// Does not compare alpha
int compareColor(Color a, Color b);

Color scaleColor(Color c, float f);

// Alpha is ignored when converting hsl/rgv to term color code
// unsigned char rgbToTerm(const Color c);
// unsigned char hslToTerm(Color c);

// Composite a over b
Color alphaComposite(Color a, Color b);

Color rgbToHsl(Color c);
Color hslToRgb(Color c);

#endif

