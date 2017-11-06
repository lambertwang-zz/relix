#include "screen.h"
#include "../constants.h"

#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

pthread_mutex_t screen_lock;

// SIGWINCH is called when the window is resized.
void handle_winch(int sig) {
    pthread_mutex_lock(&screen_lock);
    initScreen();
    // signal(SIGWINCH, SIG_IGN);
    pthread_mutex_unlock(&screen_lock);
}

// Clears the active screen buffer
void clearScreen(struct Screen *scr) {
    int i;
    for (i = 0; i < screen.ts.lines * screen.ts.cols; i++) {
        screen.pixelBuffer[i].fg = 16;
        screen.pixelBuffer[i].bg = 16;
        screen.pixelBuffer[i].c_bg = COLOR_BLANK;
        screen.pixelBuffer[i].c_bg = COLOR_BLANK;
        screen.pixelBuffer[i].chr = ' ';
        screen.pixelBuffer[i].id = -1;
    }

}

void setCamera(Point loc) {
    screen.camera = loc;
    screen.camera_bounds.top = loc.y - (screen.ts.lines >> 1);
    screen.camera_bounds.bottom = screen.camera_bounds.top + screen.ts.lines;
    screen.camera_bounds.left = loc.x - (screen.ts.cols >> 1);
    screen.camera_bounds.right = screen.camera_bounds.left + screen.ts.cols;
}

struct Screen *initScreen() {
    static int first_init = 1;

    // If re-init, free buffers
    if (!first_init) {
        free(screen.pixelBuffer);
        free(screen.prevPixelBuffer);
    }

    if (first_init) {
        pthread_mutex_init(&screen_lock, NULL);

        screen.times_init = 0;
        first_init= 0;
        signal(SIGWINCH, handle_winch);
        // Set auto flush
        // setbuf(stdout, NULL);

        // Hide the cursor
        printf("\e[?25l");
        // Bold text
        printf("\e[1m");
    }

    screen.times_init++;

    // Clear terminal
    printf("\033[2J");

    getTermSize(&screen.ts);

    screen.margin_x = 0;
    screen.margin_y = 0;

    if (screen.ts.lines > MAX_HEIGHT) {
        screen.margin_y = (screen.ts.lines - MAX_HEIGHT) / 2;
        screen.ts.lines = MAX_HEIGHT;
    }
    if (screen.ts.cols> MAX_WIDTH) {
        screen.margin_x = (screen.ts.cols- MAX_WIDTH) / 2;
        screen.ts.cols= MAX_WIDTH;
    }

    setCamera((Point){0, 0});
    
    screen.pixelBuffer = malloc(sizeof(struct Pixel) * screen.ts.cols * screen.ts.lines);
    screen.prevPixelBuffer = malloc(sizeof(struct Pixel) * screen.ts.cols * screen.ts.lines);

    clearScreen(&screen);

    return 0;
}

int closeScreen() {
    free(screen.pixelBuffer);
    free(screen.prevPixelBuffer);

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
    return 0;
}

int swapScreen() {
    int i, j;
    unsigned int index;
    // Refers to preceding pixel in the row
    int prevFg = -1;
    int prevBg = -1;

    unsigned char fg;
    unsigned char bg;
    char chr;

    pthread_mutex_lock(&screen_lock);

    // Line-buffer
    char *buffer = malloc(sizeof(char) * 24 * screen.ts.cols);
    unsigned int charsPrinted;

    int unchangedPixels;

    for (j = 0; j < screen.ts.lines; j++) {
        prevFg = -1;
        prevBg = -1;
        unchangedPixels = 0;
        charsPrinted = sprintf(buffer, "\e[%d;%dH", screen.margin_y + j + 1, screen.margin_x);
        for (i = 0; i < screen.ts.cols; i++) {
            index = i + j * screen.ts.cols;

            fg = screen.pixelBuffer[index].fg;
            bg = screen.pixelBuffer[index].bg;
            chr = screen.pixelBuffer[index].chr;
            
            // Check if pixel was unchanged from last frame
            if (fg == screen.prevPixelBuffer[index].fg &&
                bg == screen.prevPixelBuffer[index].bg &&
                chr == screen.prevPixelBuffer[index].chr) {
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

    memcpy(screen.prevPixelBuffer, screen.pixelBuffer, sizeof(Pixel) * screen.ts.cols * screen.ts.lines);

    clearScreen(&screen);

    pthread_mutex_unlock(&screen_lock);

    fwrite("\e[0m", sizeof(char), 5, stdout);
    fflush(stdout);

    free(buffer);
    
    return 0;
}

