#ifndef __RENDER_H__
#define __RENDER_H__

#include "term/screen.h"

int oputString(Screen *s, int id, int x, int y, char *str, Color fg, Color bg);
int oputRect(Screen *s, int id, int x, int y, Rect rect, Color bg);

int putString(Screen *s, int x, int y, char *str, Color fg, Color bg);
int putRect(Screen *s, int x, int y, Rect rect, Color bg);

int putPixel(Screen *s, int x, int y, Pixel p);
int putPixelL(Screen *s, int x, int y, Pixel p);
int putPixelA(Screen *s, int x, int y, Pixel p);

int putPixelRgb(Screen *s, int x, int y, Color c);

int putLight(Screen *s, int x, int y, Color l);
// Linear point light
int putPointLight(Screen *s, Point p, Color l, int radius);

// TODO:
// Normal point light

// Half normal point light

#endif

