#ifndef __COLOR_H__
#define __COLOR_H__

typedef struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Color;

typedef struct Pixel {
    unsigned char fg;
    unsigned char bg;

} Pixel;

unsigned char rgbToTerm(Color c);
Color rgbToHsl(Color c);
Color hslToRgb(Color c);

#endif

