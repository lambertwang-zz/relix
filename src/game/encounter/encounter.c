#include "encounter.h"

// Engine
#include "log/log.h"

// Game
#include "../relix.h"

void addEncounter(Map *map, Rect loc, int challenge) {
    int open_space = (loc.right - loc.left) * (loc.bottom - loc.top);

    int i, j;
    for (j = loc.top; i < loc.bottom; j++) {
        for (i = loc.left; i < loc.right; i++) {
            open_space -= isSolid(map, (Point){i, j, 0});
        }
    }

    writeLog(LOG_MAP, "encounter::addEncounter(): Open space: %d", open_space);
    

}

