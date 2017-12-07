#ifndef __SCREEN_H__
#define __SCREEN_H__

// Engine
#include "color.h"
#include "term.h"
#include "geometry/geometry.h"
#include "list/tree.h"

typedef struct Screen {
    int id;

    TermSize ts;

    int margin_x;
    int margin_y;

    Point camera;
    Rect camera_bounds;

    Color *light_buffer;
    Pixel *pixel_buffer;

    // Refers to previous frame
    Pixel *prev_pixel_buffer;

    int times_init;
} Screen;

typedef struct ScreenManager {
    Screen main_screen;

    Tree screen_tree;
} ScreenManager;

ScreenManager screen_manager;

void initScreenManager();
void closeScreenManager();
int renderScreens();

void setCamera(Screen *screen, Point loc);

void initScreen(Screen *screen);
void clearScreen(Screen *screen);
int closeScreen(Screen *screen);

int swapScreen();

#endif

