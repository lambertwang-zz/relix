#include "screen.h"

// Library
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Engine
#include "constants.h"

void setCamera(Screen *screen, Point loc) {
    screen->camera = loc;
    screen->camera_bounds.top = loc.y - (screen->ts.lines >> 1);
    screen->camera_bounds.bottom = screen->camera_bounds.top + screen->ts.lines;
    screen->camera_bounds.left = loc.x - (screen->ts.cols >> 1);
    screen->camera_bounds.right = screen->camera_bounds.left + screen->ts.cols;
}

void initScreen(Screen *screen) {
    static int screen_id_iterator = 0;
    int i;

    // If re-init, free buffers
    if (screen->times_init > 0) {
        free(screen->light_buffer);
        free(screen->pixel_buffer);
        free(screen->prev_pixel_buffer);
    } else {
        screen->id = screen_id_iterator++;
    }

    screen->times_init++;

    getTermSize(&screen->ts);

    screen->margin_x = 0;
    screen->margin_y = 0;

    if (screen->ts.lines > MAX_HEIGHT) {
        screen->margin_y = (screen->ts.lines - MAX_HEIGHT) / 2;
        screen->ts.lines = MAX_HEIGHT;
    }
    if (screen->ts.cols> MAX_WIDTH) {
        screen->margin_x = (screen->ts.cols- MAX_WIDTH) / 2;
        screen->ts.cols= MAX_WIDTH;
    }

    setCamera(screen, (Point){0, 0, 0});
    
    screen->light_buffer = malloc(sizeof(Color) * screen->ts.cols * screen->ts.lines);
    screen->pixel_buffer = malloc(sizeof(Pixel) * screen->ts.cols * screen->ts.lines);
    screen->prev_pixel_buffer = malloc(sizeof(Pixel) * screen->ts.cols * screen->ts.lines);

    clearScreen(screen);

    for (i = 0; i < screen->ts.lines * screen->ts.cols; i++) {
        screen->prev_pixel_buffer[i].bg = -1;
        screen->prev_pixel_buffer[i].fg = -1;
        screen->prev_pixel_buffer[i].chr = -1;
    }
}

// Clears a screen buffer
void clearScreen(Screen *screen) {
    int i;

    for (i = 0; i < screen->ts.lines * screen->ts.cols; i++) {
        screen->light_buffer[i] = COLOR_BLANK;
        screen->pixel_buffer[i] = PIXEL_NULL;
    }
}

int closeScreen(Screen *screen) {
    free(screen->light_buffer);
    free(screen->pixel_buffer);
    free(screen->prev_pixel_buffer);

    return 0;
}

