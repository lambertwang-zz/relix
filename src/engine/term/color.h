#ifndef __COLOR_H__
#define __COLOR_H__

#define COLOR_BLANK (Color){0, 0, 0, 1.0}
#define COLOR_EMPTY (Color){0, 0, 0, 0.0}
#define COLOR_WHITE (Color){255, 255, 255, 1.0}
#define COLOR_RED (Color){255, 0, 0, 1.0}
#define COLOR_GREEN (Color){0, 255, 0, 1.0}
#define COLOR_BLUE (Color){0, 0, 255, 1.0}

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
    int id;
} Pixel;

// Alpha is ignored when converting hsl/rgv to term color code
unsigned char rgbToTerm(Color c);

// Composite a over b
Color alphaComposite(Color a, Color b);

Color rgbToHsl(Color c);
Color hslToRgb(Color c);

#endif

