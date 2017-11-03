#include "color.h"

unsigned char rgbToTerm(Color c) {
    int r_t = c.r / 43;
    int g_t = c.g / 43;
    int b_t = c.b / 43;
    return 16 + b_t + g_t * 6 + r_t * 36;
}

Color rgbToHsl(Color c) {
    return c;
}

Color hslToRgb(Color hsl) {
    Color rgb;
    unsigned char region, remainder, p, q, t;
    if (hsl.g == 0) {
        rgb.r = hsl.b;
        rgb.g = hsl.b;
        rgb.b = hsl.b;
        return rgb;
    }
    region = hsl.r / 43;
    remainder = (hsl.r - (region * 43)) * 6; 
    p = (hsl.b * (255 - hsl.g)) >> 8;
    q = (hsl.b * (255 - ((hsl.g * remainder) >> 8))) >> 8;
    t = (hsl.b * (255 - ((hsl.g * (255 - remainder)) >> 8))) >> 8;
    switch (region) {
    case 0:
        rgb.r = hsl.b; rgb.g = t; rgb.b = p;
        break;
    case 1:
        rgb.r = q; rgb.g = hsl.b; rgb.b = p;
        break;
    case 2:
        rgb.r = p; rgb.g = hsl.b; rgb.b = t;
        break;
    case 3:
        rgb.r = p; rgb.g = q; rgb.b = hsl.b;
        break;
    case 4:
        rgb.r = t; rgb.g = p; rgb.b = hsl.b;
        break;
    default:
        rgb.r = hsl.b; rgb.g = p; rgb.b = q;
        break;
    }
    return rgb;
}

