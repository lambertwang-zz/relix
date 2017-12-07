#include "log/log.h"

// Game
#include "../src/game/relix.h"
#include "../src/game/resource/resource.h"


int main(int argc, char **argv) {
    initLog();

    addLogLevel(LOG_LOAD_V);

    initResources();

    closeLog();

    return 0;
}

