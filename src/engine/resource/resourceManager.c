#include "resource.h"

// Library
#include <stdlib.h>

// Engine
#include "log/log.h"

void initResourceManager(ResourceManager *manager, void (*deleteData)(void *)) {
    initArray(&manager->store);
    manager->deleteData = deleteData;
}

void closeResourceManager(ResourceManager *res) {
    unsigned int i;
    for (i = 0; i < res->store.count; i++) {
        ResourceEntry *entry = getDataArray(&res->store, i);
        res->deleteData(entry->data);
        deleteString(entry->tag);
        free(entry);
    }

    closeArray(&res->store);
}

int registerResource(ResourceManager *res, String *tag, void *data) {
    if (retrieveResource(res, tag) != NULL) {
        writeLog(LOG_RESOURCE, "resourceManager::registerResource(): Error: Cannot register resource. Resource with tag %s already exists.", tag->s);
        return 1;
    }

    ResourceEntry *new_entry = malloc(sizeof(ResourceEntry));
    new_entry->tag = createString();
    stringCopy(new_entry->tag, tag);
    new_entry->data = data;

    push(&res->store, new_entry);
    return 0;
}

void *retrieveResource(ResourceManager *res, String *tag) {
    unsigned int i;
    for (i = 0; i < res->store.count; i++) {
        ResourceEntry *entry = getDataArray(&res->store, i);
        if (stringCompare(entry->tag, tag) == 0) {
            return entry->data;
        }
    }

    return NULL;
}

