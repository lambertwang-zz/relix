#include "game/game.h"
#include "term/screen.h"
#include "log/log.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "list/tree.h"

void terminate(int a) {
    closeScreen();
    closeLog();
    exit(0);
}

int main(int argc, char **argv) {
    initLog();
    signal(SIGINT, terminate);

    initScreen();

    loop();

    terminate(0);
    return 0;
}
