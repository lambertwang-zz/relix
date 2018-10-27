#include "screen.h"

// Library
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#if defined __linux__
#include <pthread.h>
#elif defined _WIN32 || defined _WIN64
#include <windows.h>
#else
#error "unknown platform"
#endif

#include "utility/utility.h"
#include "log/log.h"

static int abort_output_thread_flag;

#if defined __linux__
static pthread_t output_thread;
#elif defined _WIN32 || defined _WIN64
static DWORD output_thread_id;
static HANDLE output_thread;
#endif

// SIGWINCH is called when the window is resized.
// void handle_winch(int sig) {
void handle_winch() {
    resize_flag = 1;
}

void *outputThreadWork() {
    writeLog(LOG_SCREEN, "screenManager::outputThreadWork(): Starting output thread.");
    while (!abort_output_thread_flag) {
        renderScreens();
    }
    writeLog(LOG_SCREEN, "screenManager::outputThreadWork(): Exiting output thread.");
    return 0;
}

void initScreenManager() {
    writeLog(LOG_SCREEN, "screenManager::initScreenManager(): Initializing screen manager.");

#if defined __linux__
    sem_init(&screen_manager.writes_allowed, 0, 1);
    sem_init(&screen_manager.reads_allowed, 0, 0);
#elif defined _WIN32 || defined _WIN64
    screen_manager.writes_allowed = CreateSemaphore(NULL, 1, 1, NULL);
    screen_manager.reads_allowed = CreateSemaphore(NULL, 0, 1, NULL);
#endif

    resize_flag = 0;
    abort_output_thread_flag = 0;

#if defined __linux__
    signal(SIGWINCH, handle_winch);
#endif

    screen_manager._line_buffer = NULL;
    screen_manager.main_screen.times_init = 0;
    initScreen(&screen_manager.main_screen);

    // Change the code page to Unicode/65001
    #if defined _WIN32 || defined _WIN64
        system("CHCP 65001");
    #endif

    // Set auto flush
    // setbuf(stdout, NULL);
    // Hide the cursor
    printf("\e[?25l");
    // Bold text
    printf("\e[1m");
    // Clear terminal
    printf("\033[2J");

#if defined __linux__
    pthread_create(&output_thread, NULL, outputThreadWork, NULL);
#elif defined _WIN32 || defined _WIN64
    output_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) outputThreadWork, NULL, 0, &output_thread_id);
#endif

    writeLog(LOG_SCREEN, "screenManager::initScreenManager(): Successfully initialized screen manager.");
}

void closeScreenManager() {
    writeLog(LOG_SCREEN, "screenManager::closeScreenManager(): Closing the screen manager.");
    // Safely terminate the output thread
    // There's no way that the engine can write to the buffer anymore, so wake
    // up the output thread to flush the last frame and check the exit flag.
    abort_output_thread_flag = 1;
#if defined __linux__
    sem_post(&screen_manager.reads_allowed);
    pthread_join(output_thread, NULL);
#elif defined _WIN32 || defined _WIN64
    ReleaseSemaphore(screen_manager.reads_allowed, 1, NULL);
    WaitForSingleObject(output_thread, INFINITE);
#endif

    closeScreen(&screen_manager.main_screen);

#if defined __linux__
    sem_destroy(&screen_manager.reads_allowed);
    sem_destroy(&screen_manager.writes_allowed);
#elif defined _WIN32 || defined _WIN64
    CloseHandle(screen_manager.reads_allowed);
    CloseHandle(screen_manager.writes_allowed);
#endif

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
    writeLog(LOG_SCREEN, "screenManager::closeScreenManager(): Successfully closed the screen manager.");
}

 renderScreens() {
 int i, j;
 Screen *screen = &screen_manager.main_screen;

    // Wait for the engine to finish preparing this frame
#if defined __linux__
    sem_wait(&screen_manager.reads_allowed);
#elif defined _WIN32 || defined _WIN64
    WaitForSingleObject(screen_manager.reads_allowed, INFINITE);
#endif

    // Resizing can only happen when the engine isn't touching the buffer
    if (resize_flag) {
        resize_flag = 0;

        initScreen(&screen_manager.main_screen);

        // Clear terminal
        printf("\033[2J");

        // Hide the cursor
        printf("\e[?25l");
    }

    // Copy the prepared frame into our thread's internal buffer
    // This will result in a blank frame if the window was resized
    writeLog(LOG_SCREEN_V, "screenManager::renderScreens(): Copying prepared frame to screen buffer.");
    memcpy(screen->current_pixel_buffer, screen->pixel_buffer, sizeof(Pixel) * screen->ts.cols * screen->ts.lines);

    // Let the engine know that it's safe to continue rendering
#if defined __linux__
    sem_post(&screen_manager.writes_allowed);
#elif defined _WIN32 || defined _WIN64
    ReleaseSemaphore(screen_manager.writes_allowed, 1, NULL);
#endif

    unsigned int index;

    // Legacy 8-bit colors
    // Refers to preceding pixel in the row
    // int prevFg, prevBg;
    // unsigned char fg, bg;
    Color prevFg, prevBg;
    Color fg, bg;
    char *chr;

    unsigned int charsPrinted;

    int unchangedPixels;

    for (j = 0; j < screen->ts.lines; j++) {
        prevFg = COLOR_UNDEFINED;
        prevBg = COLOR_UNDEFINED;
        unchangedPixels = 0;

        // Move caret to start of next line
        charsPrinted = sprintf(screen_manager._line_buffer, "\e[%d;%dH", screen->margin_y + j + 1, screen->margin_x);
        for (i = 0; i < screen->ts.cols; i++) {
            index = i + j * screen->ts.cols;

            // fg = screen->current_pixel_buffer[index].__fg;
            // bg = screen->current_pixel_buffer[index].__bg;
            fg = screen->current_pixel_buffer[index].fg;
            bg = screen->current_pixel_buffer[index].bg;
            chr = screen->current_pixel_buffer[index].chr;
            
            // Check if pixel was unchanged from last frame
            // if (fg == screen->prev_pixel_buffer[index].__fg &&
            //     bg == screen->prev_pixel_buffer[index].__bg &&
            if (!compareColor(fg, screen->prev_pixel_buffer[index].fg) &&
                !compareColor(bg, screen->prev_pixel_buffer[index].bg) &&
                !strcmp(chr, screen->prev_pixel_buffer[index].chr)) {
                unchangedPixels++;
                continue;
            }

            // If preceding pixels are unchanged, skip characters
            if (unchangedPixels > 0) {
                charsPrinted += sprintf(screen_manager._line_buffer + charsPrinted, "\e[%dC", unchangedPixels);
                unchangedPixels = 0;
            }

            if (compareColor(prevFg, fg)) {
                // charsPrinted += sprintf(buffer + charsPrinted, "\e[38;5;%dm", fg.r);
                charsPrinted += sprintf(screen_manager._line_buffer + charsPrinted, 
                        "\e[38;2;%d;%d;%dm", fg.r, fg.g, fg.b);
                prevFg = fg;
            }
            if (compareColor(prevBg, bg)) {
                // charsPrinted += sprintf(buffer + charsPrinted, "\e[48;5;%dm", bg.r);
                charsPrinted += sprintf(screen_manager._line_buffer + charsPrinted, 
                        "\e[48;2;%d;%d;%dm", bg.r, bg.g, bg.b);
                prevBg = bg;
            }
            
            if (chr[0] != '\0') {
                charsPrinted += sprintf(screen_manager._line_buffer + charsPrinted, "%s", chr); 
            } else {
                // charsPrinted += sprintf(screen_manager._line_buffer + charsPrinted, " "); 
                charsPrinted += sprintf(screen_manager._line_buffer + charsPrinted, "║"); 
            }
        }

        fwrite(screen_manager._line_buffer, sizeof(char), charsPrinted, stdout);
    }

    // Copy buffers
    memcpy(screen->prev_pixel_buffer, screen->current_pixel_buffer, sizeof(Pixel) * screen->ts.cols * screen->ts.lines);

    fwrite("\e[0m", sizeof(char), 5, stdout);
    fflush(stdout);

    return 0;
}
