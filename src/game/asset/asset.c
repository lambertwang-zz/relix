#include "asset.h"

// Library
#include <string.h>

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

int loadStringToEntry(char *dest, JsonNode *node) {
    if (node == NULL || node->type != JSON_STRING) {
        writeLog(LOG_LOAD, "monster_load::loadStringToEntry(): ERROR: Expected string property.");
        return 1;
    }

    strncpy(dest, node->data, LABEL_MED);

    return 0;
}

int loadDiceToEntry(Dice *dest, JsonNode *node) {
    if (node == NULL || node->type != JSON_STRING) {
        writeLog(LOG_LOAD, "monster_load::loadStringToEntry(): ERROR: Expected dice property.");
        return 1;
    }

    *dest = parseDice(node->data);

    return 0;
}

int loadCharToEntry(char *dest, JsonNode *node) {
    if (node == NULL || node->type != JSON_STRING) {
        writeLog(LOG_LOAD, "monster_load::loadCharToEntry(): ERROR: Expected char property.");
        return 1;
    }

    *dest = ((char *)node->data)[0];

    return 0;
}

int loadIntToEntry(int *dest, JsonNode *node) {
    if (node == NULL || node->type != JSON_INT) {
        writeLog(LOG_LOAD, "monster_load::loadIntToEntry(): ERROR: Expected int property.");
        return 1;
    }

    *dest = *((int *)node->data);

    return 0;
}
