#include "screen.h"

// Library
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "utility/utility.h"

static int resize_flag;

// SIGWINCH is called when the window is resized.
void handle_winch(int sig) {
    resize_flag = 1;
}

void initScreenManager() {
    resize_flag = 0;

    signal(SIGWINCH, handle_winch);

    screen_manager.main_screen.times_init = 0;
    initScreen(&screen_manager.main_screen);

    initTree(&screen_manager.screen_tree);

    // Set auto flush
    // setbuf(stdout, NULL);
    // Hide the cursor
    printf("\e[?25l");
    // Bold text
    printf("\e[1m");
    // Clear terminal
    printf("\033[2J");
}

void closeScreenManager() {
    Iterator *it;
    it = initIterator(&screen_manager.screen_tree);
    while (!done(it)) {
        Screen *screen = getNext(it)->data;
        closeScreen(screen);
    }
    closeIterator(it);

    closeScreen(&screen_manager.main_screen);

    // Reset colors
    printf("\e[39m\e[49m");
    // Show cursor
    printf("\e[?25h");
    // Clear screen
    printf("\e[2J");
    // Move cursor to top
    printf("\e[1;1H");
    // Reset text attributes
    printf("\e[0m");
    fflush(stdout);
}

int renderScreens() {
    if (resize_flag) {
        resize_flag = 0;
        Iterator *it;
        it = initIterator(&screen_manager.screen_tree);
        while (!done(it)) {
            Screen *screen= getNext(it)->data;
            initScreen(screen);
        }
        closeIterator(it);

        initScreen(&screen_manager.main_screen);

        // Clear terminal
        printf("\033[2J");
    }
    int i, j;
    unsigned int index;
    // Refers to preceding pixel in the row
    int prevFg, prevBg;

    unsigned char fg, bg;
    char chr;

    Screen *screen = &screen_manager.main_screen;

    // Line-buffer
    // TODO: Clear still-reachable memory block (not a leak)
    char *buffer = malloc(sizeof(char) * 24 * screen->ts.cols);
    unsigned int charsPrinted;

    int unchangedPixels;

    for (j = 0; j < screen->ts.lines; j++) {
        prevFg = -1;
        prevBg = -1;
        unchangedPixels = 0;

        // Move caret to start of next line
        charsPrinted = sprintf(buffer, "\e[%d;%dH", screen->margin_y + j + 1, screen->margin_x);
        for (i = 0; i < screen->ts.cols; i++) {
            index = i + j * screen->ts.cols;

            fg = screen->pixel_buffer[index].fg;
            bg = screen->pixel_buffer[index].bg;
            chr = screen->pixel_buffer[index].chr;
            
            // Check if pixel was unchanged from last frame
            if (fg == screen->prev_pixel_buffer[index].fg &&
                bg == screen->prev_pixel_buffer[index].bg &&
                chr == screen->prev_pixel_buffer[index].chr) {
                unchangedPixels++;
                continue;
            }

            // If preceding pixels are unchanged, skip characters
            if (unchangedPixels) {
                charsPrinted += sprintf(buffer + charsPrinted, "\e[%dC", unchangedPixels);
                unchangedPixels = 0;
            }

            if (prevFg != fg) {
                charsPrinted += sprintf(buffer + charsPrinted, "\e[38;5;%dm", fg);
                prevFg = fg;
            }
            if (prevBg != bg) {
                charsPrinted += sprintf(buffer + charsPrinted, "\e[48;5;%dm", bg);
                prevBg = bg;
            }
            charsPrinted += sprintf(buffer + charsPrinted, "%c", chr); 
        }

        fwrite(buffer, sizeof(char), charsPrinted, stdout);
    }

    free(buffer);

    memcpy(screen->prev_pixel_buffer, screen->pixel_buffer, sizeof(Pixel) * screen->ts.cols * screen->ts.lines);

    clearScreen(screen);

    fwrite("\e[0m", sizeof(char), 5, stdout);
    fflush(stdout);

    return 0;
}
