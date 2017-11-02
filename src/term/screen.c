
#include "screen.h"
#include "term.h"

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

struct Screen *getScreen() {
    static struct Screen screen;
    return &screen;
}

struct Screen *initScreen() {
    static int first_init = 1;
    struct Screen *screen = getScreen();
    screen->times_init++;

    if (!first_init) {
        free(screen->charBuffer);
        free(screen->backCharBuffer);
    }

    if (first_init) {
        pthread_mutex_init(&screen_lock, NULL);
        first_init= 0;
        signal(SIGWINCH, handle_winch);
        // Hide the cursor
        setbuf(stdout, NULL);
        printf("\e[?25l");
    }

    printf("\e[2J");

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
    
    screen->charBuffer = malloc(sizeof(char) * screen->ts.cols * screen->ts.lines);
    screen->backCharBuffer = malloc(sizeof(char) * screen->ts.cols * screen->ts.lines);

    int i, j;
    for (i = 0; i < screen->ts.lines - 1; i++) {
        for (j = 0; j < screen->ts.cols; j++) {
            screen->charBuffer[j + i * screen->ts.cols] = (i % 26) + 65;
        }
    }


    return 0;
}

int swapScreen() {
    pthread_mutex_lock(&screen_lock);
    struct Screen *screen = getScreen();

    // Move caret to beginning of screen
    //

    int i, j;
    // printf("\e[%d;0H", screen->margin_y);
    // fwrite("\e[0;0H", sizeof(char), 6, stdout);

    // fwrite(screen->charBuffer, sizeof(char), screen->ts.cols * screen->ts.lines, stdout);
    for (j = 0; j < screen->ts.lines; j++) {
        // for (i = 0; i < screen->margin_x; i++) {
        //     putchar(' ');
        // }
        printf("\e[%d;%dH", screen->margin_y + j, screen->margin_x);
        fwrite(&screen->charBuffer[j * screen->ts.cols], sizeof(char), screen->ts.cols, stdout);
        // fwrite("\e[E", sizeof(char), 3, stdout);
    }
    pthread_mutex_unlock(&screen_lock);

    // fflush(stdout);
    return 0;
}

