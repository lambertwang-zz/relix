#include "log/log.h"

// Game
#include "../src/game/relix.h"
#include "../src/game/asset/asset.h"


int main() {
    initLog();

    addLogLevel(LOG_LOAD_V);

    initResources();

    closeLog();

    return 0;
}

