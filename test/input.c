#include "log/log.h"
#include "input/input.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


void terminate(int a) {
    closeInput();
    exit(0);
}

int main(int argc, char **argv) {
    initLog();
    initInput();

    addLogLevel(LOG_INPUT_V);

    signal(SIGINT, terminate);

    struct timespec sleep_time;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 16666 * 1000;

    while (1) {
        readInput();
        nanosleep(&sleep_time, NULL);
    }

    terminate(0);

    return 0;
}
