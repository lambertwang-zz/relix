#include "term.h"

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

int getTermSize(struct TermSize *ts) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    ts->lines = w.ws_row;
    ts->cols = w.ws_col;

    return 0;
}
