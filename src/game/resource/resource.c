#include "resource.h"

// Engine
#include "log/log.h"

// Game
#include "../relix.h"

int initResources() {
    if (loadMonsterResources() == 0) {
        writeLog(LOG_LOAD, "relix::loadResources(): ERROR: Unable to load any monster resources.");
        return 1;
    }
    return 0;
}

