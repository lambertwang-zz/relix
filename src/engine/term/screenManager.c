#include "screen.h"

// Library
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <pthread.h>

#include "utility/utility.h"
#include "log/log.h"

static int resize_flag;
static int abort_output_thread_flag;
static pthread_t output_thread;

// SIGWINCH is called when the window is resized.
// void handle_winch(int sig) {
void handle_winch() {
    resize_flag = 1;
}

void *outputThreadWork() {
    writeLog(LOG_SCREEN, "screenManager::outputThreadWork output thread started");
    while (!abort_output_thread_flag) {
        renderScreens();
    }
    writeLog(LOG_SCREEN, "screenManager::outputThreadWork output thread exiting");
    return 0;
}

void initScreenManager() {
    // writes_allowed starts at 1 to allow the first frame to render
    sem_init(&screen_manager.writes_allowed, 0, 1);
    sem_init(&screen_manager.reads_allowed, 0, 0);
    resize_flag = 0;
    abort_output_thread_flag = 0;

#if defined __linux__
    signal(SIGWINCH, handle_winch);
#endif

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

    pthread_create(&output_thread, NULL, outputThreadWork, NULL);
}

void closeScreenManager() {
    // Safely terminate the output thread
    // There's no way that the engine can write to the buffer anymore, so wake
    // up the output thread to flush the last frame and check the exit flag.
    abort_output_thread_flag = 1;
    sem_post(&screen_manager.reads_allowed);
    pthread_join(output_thread, NULL);
    
    Iterator *it;
    it = initIterator(&screen_manager.screen_tree);
    while (!done(it)) {
        Screen *screen = getNext(it)->data;
        closeScreen(screen);
    }
    closeIterator(it);

    closeScreen(&screen_manager.main_screen);

    sem_destroy(&screen_manager.reads_allowed);
    sem_destroy(&screen_manager.writes_allowed);

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
    Screen *screen = &screen_manager.main_screen;

    // Wait for the engine to finish preparing this frame
    sem_wait(&screen_manager.reads_allowed);

    // Resizing can only happen when the engine isn't touching the buffer
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

    // Copy the prepared frame into our thread's internal buffer
    // This will result in a blank frame if the window was resized
    memcpy(screen->current_pixel_buffer, screen->pixel_buffer, sizeof(Pixel) * screen->ts.cols * screen->ts.lines);

    // Clear the shared buffers
    clearScreen(screen);

    // Let the engine know that it's safe to continue rendering
    sem_post(&screen_manager.writes_allowed);

    int i, j;
    unsigned int index;
    // Refers to preceding pixel in the row
    int prevFg, prevBg;

    unsigned char fg, bg;
    char chr;

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

            fg = screen->current_pixel_buffer[index].fg;
            bg = screen->current_pixel_buffer[index].bg;
            chr = screen->current_pixel_buffer[index].chr;
            
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

    memcpy(screen->prev_pixel_buffer, screen->current_pixel_buffer, sizeof(Pixel) * screen->ts.cols * screen->ts.lines);

    fwrite("\e[0m", sizeof(char), 5, stdout);
    fflush(stdout);

    return 0;
}
