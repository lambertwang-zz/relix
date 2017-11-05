#include "../src/log/log.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "../src/input/input.h"

void terminate(int a) {
    closeInput();
    exit(0);
}

int main(int argc, char **argv) {
    initLog();
    initInput();
    signal(SIGINT, terminate);

    while (1) {
        readInput();
    }

    terminate(0);

    return 0;
}
