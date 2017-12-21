#include "color.h"

// Library
#include <stdlib.h>

#ifndef abs
#define abs(a) ((a) < 0 ? -(a) : (a))
#endif

int compareColor(Color a, Color b) {
    if (a.a < 0 || b.a < 0) {
        return -1;
    }
    return abs(a.r - b.r) +
           abs(a.g - b.g) +
           abs(a.b - b.b);
}

Color scaleColor(Color c, float f) {
    return (Color){c.r * f, c.g * f, c.b * f, c.a};
}

unsigned char rgbToTerm(const Color c) {
    int r_t = c.r <= 75 ? 0 : (c.r - 36) / 40;
    int g_t = c.g <= 75 ? 0 : (c.g - 36) / 40;
    int b_t = c.b <= 75 ? 0 : (c.b - 36) / 40;
    if (r_t == g_t && g_t == b_t) {
        int gray_t = (int)((c.r + c.g + c.b) / 29.43);
        switch (gray_t) {
            case 0:
                return 16;
            case 25:
                return 231;
            default:
                return 231 + gray_t;
        }
    }
    return 16 + b_t + g_t * 6 + r_t * 36;
}

unsigned char hslToTerm(Color c) {
    return rgbToTerm(hslToRgb(c));
}

Color alphaComposite(Color a, Color b) {
    if (a.a >= 1.0) {
        return a;
    }
    if (a.a <= 0.0) {
        return b;
    }
    if (b.a <= 0.0) {
        return a;
    }

    Color result;
    if (b.a >= 1.0) {
        result.r = a.r * a.a + b.r * (1.0 - a.a);
        result.g = a.g * a.a + b.g * (1.0 - a.a);
        result.b = a.b * a.a + b.b * (1.0 - a.a);
        result.a = a.a + b.a * (1.0 - a.a);
        return result;
    }
    result.r = (a.r * a.a + b.r * b.a * (1.0 - a.a)) / (a.a + b.a * (1.0 - a.a));
    result.g = (a.g * a.a + b.g * b.a * (1.0 - a.a)) / (a.a + b.a * (1.0 - a.a));
    result.b = (a.b * a.a + b.b * b.a * (1.0 - a.a)) / (a.a + b.a * (1.0 - a.a));
    result.a = a.a + b.a * (1.0 - a.a);
    return result;
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

