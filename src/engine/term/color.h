#ifndef __COLOR_H__
#define __COLOR_H__

#include <limits.h>

#define COLOR_BLANK (Color){0, 0, 0, 1.0}
#define COLOR_BLACK (Color){0, 0, 0, 1.0}
#define COLOR_EMPTY (Color){0, 0, 0, 0.0}
#define COLOR_WHITE (Color){255, 255, 255, 1.0}
#define COLOR_RED (Color){255, 0, 0, 1.0}
#define COLOR_GREEN (Color){0, 255, 0, 1.0}
#define COLOR_BLUE (Color){0, 0, 255, 1.0}

#define PIXEL_NULL (Pixel){16, 16, COLOR_EMPTY, COLOR_EMPTY, ' ', -1, INT_MIN}
#define PIXEL_BLANK (Pixel){16, 16, COLOR_BLANK, COLOR_BLANK, ' ', -1, 0}

typedef struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    // Alpha is ignored for character rendering
    // a = [0..1]
    float a;
} Color;

typedef struct Pixel {
    unsigned char fg;
    unsigned char bg;

    Color c_fg;
    Color c_bg;

    char chr;
    int id; // Object id at screen location
    int depth; // Depthbuffer
} Pixel;

Color scaleColor(Color c, float f);

// Alpha is ignored when converting hsl/rgv to term color code
unsigned char rgbToTerm(const Color c);
unsigned char hslToTerm(Color c);

// Composite a over b
Color alphaComposite(Color a, Color b);

Color rgbToHsl(Color c);
Color hslToRgb(Color c);

#endif

