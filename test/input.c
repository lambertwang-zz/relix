#include "log/log.h"
#include "input/input.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


void end() {
    closeInput();
    closeLog();
    exit(0);
}

int main() {
    initLog();
    initInput();

    addLogLevel(LOG_INPUT_V);

    signal(SIGINT, end);

    struct timespec sleep_time;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 16666 * 1000;

    while (1) {
        readInput();
        nanosleep(&sleep_time, NULL);
    }

    end();

    return 0;
}
