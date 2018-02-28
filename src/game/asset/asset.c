#include "asset.h"

// Engine
#include "log/log.h"

// Game
#include "../relix.h"

int initResources() {
    initMonsterResources();
    if (loadMonsterResources() == 0) {
        writeLog(LOG_LOAD, "relix::loadResources(): ERROR: Unable to load any monster resources.");
        return 1;
    }
    return 0;
}

int loadStringToEntry(String *dest, JsonNode *node) {
    if (node == NULL || node->type != JSON_STRING) {
        writeLog(LOG_LOAD, "asset::loadStringToEntry(): ERROR: Expected string property.");
        return 1;
    }

    stringCopy(dest, node->data);

    return 0;
}

int loadDiceToEntry(Dice *dest, JsonNode *node) {
    if (node == NULL) {
        writeLog(LOG_LOAD, "asset::loadDiceToEntry(): ERROR: node is NULL.");
        return 1;
    }
    if (node->type != JSON_STRING) {
        writeLog(LOG_LOAD, "asset::loadDiceToEntry(): ERROR: Expected node to be string.");
        return 2;
    }

    *dest = parseDice(node->data);

    return 0;
}

int loadCharToEntry(char *dest, JsonNode *node) {
    if (node == NULL || node->type != JSON_STRING) {
        writeLog(LOG_LOAD, "asset::loadCharToEntry(): ERROR: Expected char property.");
        return 1;
    }

    *dest = ((char *)node->data)[0];

    return 0;
}

int loadIntToEntry(int *dest, JsonNode *node) {
    if (node == NULL || node->type != JSON_INT) {
        writeLog(LOG_LOAD, "asset::loadIntToEntry(): ERROR: Expected int property.");
        return 1;
    }

    *dest = *((int *)node->data);

    return 0;
}
