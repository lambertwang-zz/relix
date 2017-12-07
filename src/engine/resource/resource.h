#ifndef __RESOURCE_H__
#define __RESOURCE_H__

// Engine
#include "constants.h"
#include "list/array.h"
#include "utility/dice.h"

typedef struct ResourceEntry {
    char tag[LABEL_MED];
    void *data;
} ResourceEntry;

typedef struct ResourceManager {
    Array resource_store;
} ResourceManager;

int registerResource(ResourceManager *res, char tag[LABEL_SHORT], void *data);
int retrieveResource(ResourceManager *res, char tag[LABEL_SHORT]);

#endif

