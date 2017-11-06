#include "render.h"
#include "term/screen.h"

int oputString(int id, int x, int y, char *str, Color fg, Color bg) {
    int i;
    Pixel p = (Pixel){rgbToTerm(fg), rgbToTerm(bg), fg, bg, ' ', id};
    for (i = 0; i + x < screen.ts.cols; i++) {
        if (str[i] == '\0') {
            break;
        }
        p.chr = str[i];
        putPixelA(x + i, y, p);
    }
    return i;
}

int oputRect(int id, int x, int y, Rect rect, Color bg) {
    int i, j;
    Pixel p = (Pixel){0, rgbToTerm(bg), COLOR_BLANK, bg, ' ', id};
    for (j = rect.top + y < 0 ? 0 : rect.top + y; j < rect.bottom + y && j < screen.ts.lines; j++) {
        for (i = rect.left + x < 0 ? 0 : rect.left + x; i < rect.right + x && i < screen.ts.cols; i++) {
            putPixelA(i, j, p);
        }
    }
    return i;
}

// Assumes p color values are precomputed
int putPixel(int x, int y, Pixel p) {
    unsigned int index = x + y * screen.ts.cols;
    if (screen.pixelBuffer[index].depth > p.depth) {
        return 1;
    }
    screen.pixelBuffer[index] = p;
    return 0;
}

// Perform alpha compositing
// Ignores p.bg
int putPixelA(int x, int y, Pixel p) {
    unsigned int index = x + y * screen.ts.cols;

    if (screen.pixelBuffer[index].depth > p.depth) {
        return 1;
    }

    // BG pixel is always opaque
    p.c_bg = alphaComposite(p.c_bg, screen.pixelBuffer[index].c_bg);
    p.bg = rgbToTerm(p.c_bg);
    screen.pixelBuffer[index] = p;
    return 0;
}

// Only draw background
int putPixelABg(int x, int y, Pixel p) {
    unsigned int index = x + y * screen.ts.cols;

    // BG pixel is always opaque
    p.c_bg = alphaComposite(p.c_bg, screen.pixelBuffer[index].c_bg);
    p.bg = rgbToTerm(p.c_bg);
    screen.pixelBuffer[index] = p;
    return 0;
}

int putPixelRgb(int x, int y, Color c) {
    unsigned int index = x + y * screen.ts.cols;
    screen.pixelBuffer[index].bg = rgbToTerm(c);
    screen.pixelBuffer[index].fg = rgbToTerm(c);
    return 0;
}

int putString(int x, int y, char *str, Color fg, Color bg) {
    return oputString(-1, x, y, str, fg, bg);
}

int putRect(int x, int y, Rect rect, Color bg) {
    return oputRect(-1, x, y, rect, bg);
}

