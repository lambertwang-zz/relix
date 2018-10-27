#include "render.h"

// Library
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Engine
#include "constants.h"

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min 
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

Color minColor(Color a, Color b) {
    return (Color){min(a.r, b.r), min(a.g, b.g), min(a.b, b.b), 1.0};
}

Color colorMultL(Color a, Color b) {
    return (Color){
        min(255, a.r * (((float) b.r) / 128.0)),
        min(255, a.g * (((float) b.g) / 128.0)),
        min(255, a.b * (((float) b.b) / 128.0)),
        1.0
    };
}

int absColor(Color a) {
    return a.r + a.g + a.b;
}

// Internal generic put pixel function
int _putPixel(Screen *s, int x, int y, Pixel p, int no_light) {
    if (x < 0 || y < 0 || y >= s->ts.lines || x >= s->ts.cols) {
        return 1;
    }
    unsigned int index = x + y * s->ts.cols;
    if (s->pixel_buffer[index].depth > p.depth) {
        return 2;
    }
    if (!no_light && absColor(s->light_buffer[index]) == 0) {
        return 3;
    }

    // BG pixel is always opaque
    p.bg = alphaComposite(p.bg, s->pixel_buffer[index].bg);
    p.fg = alphaComposite(p.fg, s->pixel_buffer[index].fg);
    if (!no_light) {
        // p.c_bg = minColor(p.c_bg, s->light_buffer[index]);
        // p.c_fg = minColor(p.c_fg, s->light_buffer[index]);
        p.bg = colorMultL(p.bg, s->light_buffer[index]);
        p.fg = colorMultL(p.fg, s->light_buffer[index]);
        /* Rendered characters ignore lighting.
        if (p.chr != NULL) {
            p.c_fg = colorMultL(p.c_fg, s->light_buffer[index]);
        }
        */
    }
    
    // Legacy 8-bit Colors
    // p.__bg = rgbToTerm(p.c_bg);
    // if (p.chr != NULL) {
        // p.__fg = rgbToTerm(p.c_fg);
    // }

    s->pixel_buffer[index] = p;

    return 0;
}

int putPixel(Screen *s, int x, int y, Pixel p) {
    p.bg.a = 1.0;

    return _putPixel(s, x, y, p, 0);
}

// Ignores lighting
int putPixelL(Screen *s, int x, int y, Pixel p) {
    return _putPixel(s, x, y, p, 1);
}

// Perform alpha compositing
// Ignores p.bg
int putPixelA(Screen *s, int x, int y, Pixel p) {
    return _putPixel(s, x, y, p, 0);
}

int rawPutString(Screen *s, int id, int start, int end, Point pos, String *str, Color fg, Color bg, int no_light) {
    int i = start;
    // Pixel p = (Pixel){0, 0, fg, bg, createString(), id, UI_DEPTH};
    Pixel p = (Pixel){fg, bg, " ", id, UI_DEPTH};
    String *tmp = createString();
    sgetc(tmp, str, i);
    while (tmp->s[0] != '\0' && i + pos.x - start < s->ts.cols && i < end) {
        sgetc(tmp, str, i);
        strncpy(p.chr, tmp->s, UNICODE_MAX);
        _putPixel(s, pos.x + i - start, pos.y, p, no_light);
        i++;
    }
    deleteString(tmp);
    return i;
}

int putString(Screen *s, Point pos, String *str, Color fg, Color bg) {
    return rawPutString(s, -1, 0, s->ts.cols, pos, str, fg, bg, 0);
}

int putStringL(Screen *s, Point pos, String *str, Color fg, Color bg) {
    return rawPutString(s, -1, 0, s->ts.cols, pos, str, fg, bg, 1);
}

int oputString(Screen *s, int id, Point pos, String *str, Color fg, Color bg) {
    return rawPutString(s, id, 0, s->ts.cols, pos, str, fg, bg, 0);
}

int oputStringL(Screen *s, int id, Point pos, String *str, Color fg, Color bg) {
    return rawPutString(s, id, 0, s->ts.cols, pos, str, fg, bg, 1);
}

int _putRect(Screen *s, int id, Rect rect, Color bg, int no_light) {
    int i, j;
    // Pixel p = (Pixel){0, 0, COLOR_BLANK, bg, NULL, id, UI_DEPTH};
    Pixel p = (Pixel){COLOR_BLANK, bg, " ", id, UI_DEPTH};
    for (j = rect.top < 0 ? 0 : rect.top; j < rect.bottom && j < s->ts.lines; j++) {
        for (i = rect.left < 0 ? 0 : rect.left; i < rect.right && i < s->ts.cols; i++) {
            _putPixel(s, i, j, p, no_light);
        }
    }
    return i;
}

int putRect(Screen *s, Rect rect, Color bg) {
    return _putRect(s, -1, rect, bg, 0);
}

int putRectL(Screen *s, Rect rect, Color bg) {
    return _putRect(s, -1, rect, bg, 1);
}

int oputRect(Screen *s, int id, Rect rect, Color bg) {
    return _putRect(s, id, rect, bg, 0);
}

int oputRectL(Screen *s, int id, Rect rect, Color bg) {
    return _putRect(s, id, rect, bg, 1);
}

// Very unsafe function
int putPixelRgb(Screen *s, int x, int y, Color c) {
    unsigned int index = x + y * s->ts.cols;
    s->pixel_buffer[index].bg = c;
    // s->pixel_buffer[index].__bg = rgbToTerm(s->pixel_buffer[index].c_bg);

    return 0;
}

// Expects screen coord
int putLight(Screen *s, int x, int y, Color l) {
    if (x < 0 || y < 0 || y >= s->ts.lines || x >= s->ts.cols) {
        return 1;
    }
    unsigned int index = x + y * s->ts.cols;

    Color *current_l = &s->light_buffer[index];

    current_l->r = min(255, (int) current_l->r + l.r);
    current_l->g = min(255, (int) current_l->g + l.g);
    current_l->b = min(255, (int) current_l->b + l.b);

    return 0;
}

// Expects world position
int putPointLight(Screen *s, Point p, Color l, int radius) {
    int i, j;
    Point rel_pos = (Point){
            p.x - s->camera_bounds.left,
            p.y - s->camera_bounds.top,
            0
    };

    for (j = -radius; j < radius; j++) {
        for (i = -radius; i < radius; i++) {
            Color tmp = scaleColor(l, max(1.0 - sqrt(i * i + j * j) / (float)radius, 0.0f));
            putLight(s, rel_pos.x + i, rel_pos.y + j, tmp);
        }
    }

    return 0;
}

