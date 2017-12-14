#ifndef __RESOURCE_H__
#define __RESOURCE_H__

// Engine
#include "constants.h"
#include "list/array.h"
#include "utility/dice.h"
#include "string/string.h"

typedef struct ResourceEntry {
    String *tag;
    void *data;
} ResourceEntry;

typedef struct ResourceManager {
    Array resource_store;
} ResourceManager;

int registerResource(ResourceManager *res, String *tag, void *data);
int retrieveResource(ResourceManager *res, String *tag);

#endif

