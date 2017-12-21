#ifndef __RENDER_H__
#define __RENDER_H__

#include "term/screen.h"

/**
 * Function Prefix:
 *  o: insert a specific object id
 * Fucntion Suffix:
 *  L: Ignore lighting buffer
 *  A: Use alpha compositing
 *
 *  By default, a rendering function will respect lighting and will ignore color alpha values.
 *  Foreground characters will always ignore alpha values, even with the 'A' function suffix.
 */

// string and rect rendering is reserved for UI rendering.
// These functions will set the depthbuffer to 1024
int rawPutString(Screen *s, int id, int start, int end, Point pos, String *str, Color fg, Color bg, int no_light);
int putString(Screen *s, Point pos, String *str, Color fg, Color bg);
int putStringL(Screen *s, Point pos, String *str, Color fg, Color bg);
int oputString(Screen *s, int id, Point pos, String *str, Color fg, Color bg);

int putRect(Screen *s, int x, int y, Rect rect, Color bg);
int putRectL(Screen *s, int x, int y, Rect rect, Color bg);
int oputRect(Screen *s, int id, int x, int y, Rect rect, Color bg);
int oputRectL(Screen *s, int id, int x, int y, Rect rect, Color bg);

// Normal rendering functions
int putPixel(Screen *s, int x, int y, Pixel p);
// Ignore lighting
int putPixelL(Screen *s, int x, int y, Pixel p);
int putPixelA(Screen *s, int x, int y, Pixel p);

// Unsafe function. Writes directly to the pixel buffer without any bounds-checking.
int putPixelRgb(Screen *s, int x, int y, Color c);


int putLight(Screen *s, int x, int y, Color l);
// Linear point light
int putPointLight(Screen *s, Point p, Color l, int radius);

// TODO:
// Normal point light

// Half normal point light

#endif

