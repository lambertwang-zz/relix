#include "screen.h"

// Library
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Engine
#include "constants.h"
#include "log/log.h"

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
        closeScreen(screen);
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
    
    size_t screen_size = sizeof(Pixel) * screen->ts.cols * screen->ts.lines;
    screen->light_buffer = malloc(sizeof(Color) * screen->ts.cols * screen->ts.lines);
    screen->pixel_buffer = malloc(screen_size);
    screen->current_pixel_buffer = malloc(screen_size);
    screen->prev_pixel_buffer = malloc(screen_size);

    for (i = 0; i < screen->ts.lines * screen->ts.cols; i++) {
        screen->light_buffer[i] = COLOR_BLANK;
        screen->current_pixel_buffer[i] = PIXEL_NULL;
        screen->pixel_buffer[i] = PIXEL_BLANK;

        // screen->prev_pixel_buffer[i].__bg = -1;
        // screen->prev_pixel_buffer[i].__fg = -1;
        screen->prev_pixel_buffer[i].bg = COLOR_UNDEFINED;
        screen->prev_pixel_buffer[i].fg = COLOR_UNDEFINED;
    }

    if (screen_manager._line_buffer != NULL) {
        free(screen_manager._line_buffer);
        screen_manager._line_buffer = NULL;
    }

    /**
     * Line-buffer
     * TODO: Clear still-reachable memory block (not a leak)
     * Max size for each char: \e[38;5;100;100;100m (20 * 2 for color codes)
     * ~40 bytes for rgb ansi color codes
     * ~4 bytes for utf-8 unicode
     * ~44 bytes max per char
     */
    screen_manager._line_buffer = malloc(sizeof(char) * 48 * screen->ts.cols);
}

// Clears a screen buffer
void clearScreen(Screen *screen) {
    writeLog(LOG_SCREEN_V, "screen::clearScreen(): Clearing screen.");
    int i;

    for (i = 0; i < screen->ts.lines * screen->ts.cols; i++) {
        screen->light_buffer[i] = COLOR_BLANK;
        screen->pixel_buffer[i] = PIXEL_BLANK;
    }
}

int closeScreen(Screen *screen) {
    writeLog(LOG_SCREEN, "screen::closeScreen(): Closing screen.");
    free(screen->light_buffer);
    free(screen->pixel_buffer);
    free(screen->current_pixel_buffer);
    free(screen->prev_pixel_buffer);

    if (screen_manager._line_buffer != NULL) {
        free(screen_manager._line_buffer);
        screen_manager._line_buffer = NULL;
    }


    return 0;
}

Pixel getPixel(Screen *s, Point p) {
    if (p.x < 0 || p.y < 0 || p.x >= s->ts.cols || p.y >= s->ts.lines) {
        return PIXEL_BLANK;
    }
    return s->pixel_buffer[p.x + p.y * s->ts.cols];
}
