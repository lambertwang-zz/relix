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
    Array store;

    // delete data callback
    void (*deleteData)(void *);
} ResourceManager;

void initResourceManager(ResourceManager *manager, void (*deleteData)(void *));
void closeResourceManager(ResourceManager *res);

int registerResource(ResourceManager *res, String *tag, void *data);
void *retrieveResource(ResourceManager *res, String *tag);

#endif

