#ifndef __RENDER_H__
#define __RENDER_H__

#include "term/color.h"
#include "geometry/geometry.h"
#include "objects/object.h"

int oputString(int id, int x, int y, char *str, Color fg, Color bg);
int oputRect(int id, int x, int y, Rect rect, Color bg);

int putPixel(int x, int y, Pixel p);
int putPixelA(int x, int y, Pixel p);
int putPixelRgb(int x, int y, Color c);

int putString(int x, int y, char *str, Color fg, Color bg);
int putRect(int x, int y, Rect rect, Color bg);

#endif

