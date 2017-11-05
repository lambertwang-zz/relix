
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
    // refresh();
    pthread_mutex_unlock(&screen_lock);
}

struct Screen *initScreen() {
    static int first_init = 1;

    if (!first_init) {
        free(screen.charBuffer);
        free(screen.backCharBuffer);
        free(screen.pixelBuffer);
        free(screen.backPixelBuffer);
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
    
    screen.pixelBuffer = malloc(sizeof(struct Pixel) * screen.ts.cols * screen.ts.lines);
    screen.backPixelBuffer = malloc(sizeof(struct Pixel) * screen.ts.cols * screen.ts.lines);

    screen.charBuffer = malloc(sizeof(char) * screen.ts.cols * screen.ts.lines);
    screen.backCharBuffer = malloc(sizeof(char) * screen.ts.cols * screen.ts.lines);

    int i, j;
    for (i = 0; i < screen.ts.lines; i++) {
        for (j = 0; j < screen.ts.cols; j++) {
            screen.charBuffer[j + i * screen.ts.cols] = (i % 26) + 65;
        }
    }


    return 0;
}

int closeScreen() {
    free(screen.charBuffer);
    free(screen.backCharBuffer);
    free(screen.pixelBuffer);
    free(screen.backPixelBuffer);

    // Reset colors
    printf("\e[39m\e[49m");
    // Show cursor
    printf("\e[?25h");
    // Clear screen
    printf("\e[2J");
    // Move cursor to top
    printf("\e[1;1H");
    fflush(stdout);
    return 0;
}

int swapScreen() {
    int i, j;
    unsigned int index;
    unsigned char prevFg = -1;
    unsigned char prevBg = -1;
    unsigned char fg;
    unsigned char bg;
    char chr;

    pthread_mutex_lock(&screen_lock);
    char *buffer = malloc(sizeof(char) * 24 * screen.ts.cols);
    unsigned int charsPrinted;

    int unchangedPixels = 0;

    for (j = 0; j < screen.ts.lines; j++) {
        charsPrinted = sprintf(buffer, "\e[%d;%dH", screen.margin_y + j + 1, screen.margin_x);
        for (i = 0; i < screen.ts.cols; i++) {
            index = i + j * screen.ts.cols;

            fg = screen.pixelBuffer[index].fg;
            bg = screen.pixelBuffer[index].bg;
            chr = screen.charBuffer[index];
            
            if (fg == screen.backPixelBuffer[index].fg &&
                bg == screen.backPixelBuffer[index].bg &&
                chr == screen.backCharBuffer[index]) {
                unchangedPixels++;
                continue;
            }
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
        if (unchangedPixels) {
            charsPrinted += sprintf(buffer + charsPrinted, "\e[%dC", unchangedPixels);
            unchangedPixels = 0;
        }
        fwrite(buffer, sizeof(char), charsPrinted, stdout);
    }

    memcpy(screen.backCharBuffer, screen.charBuffer, sizeof(char) * screen.ts.cols * screen.ts.lines);
    memcpy(screen.backPixelBuffer, screen.pixelBuffer, sizeof(Pixel) * screen.ts.cols * screen.ts.lines);

    pthread_mutex_unlock(&screen_lock);

    fwrite("\e[0m", sizeof(char), 5, stdout);
    fflush(stdout);

    free(buffer);
    
    return 0;
}

int putPixel(int x, int y, unsigned char c) {
    unsigned int index = x + y * screen.ts.cols;
    // screen.pixelBuffer[index].bg = c;
    return 0;
}
int putPixelRgb(int x, int y, Color c) {
    unsigned int index = x + y * screen.ts.cols;
    screen.pixelBuffer[index].bg = rgbToTerm(c);
    screen.pixelBuffer[index].fg = rgbToTerm(c);
    return 0;
}

