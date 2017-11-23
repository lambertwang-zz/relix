#include "term.h"

#include <stdio.h>
#include <unistd.h>

#if defined __linux__
#include <sys/ioctl.h>
#elif defined _WIN32 || defined _WIN64
#include <windows.h>
#else
#error "unknown platform"
#endif

int getTermSize(struct TermSize *ts) {
#if defined __linux__
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    ts->lines = w.ws_row;
    ts->cols = w.ws_col;
#elif defined _WIN32 || defined _WIN64
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    ts->cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    ts->lines = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
#error "unknown platform"
#endif
    return 0;
}
