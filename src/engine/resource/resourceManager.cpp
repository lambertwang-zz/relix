#include "resource.h"

// Library
#include <stdlib.h>

// Engine
#include "log/log.h"

namespace rlx {

ResourceManager::ResourceManager() {
    store = new Array<ResourceEntry>();
}

ResourceManager::~ResourceManager() {
    unsigned int i;
    for (i = 0; i < store->getCount(); i++) {
        delete store->get(i);
    }

    delete store;
}

int ResourceManager::registerResource(ResourceEntry *entry) {
    if (retrieveResource(entry->getTag()) != NULL) {
        writeLog(LOG_RESOURCE,
            "resourceManager::registerResource(): Error: Cannot register "
            "resource. Resource with tag %s already exists.",
            entry->getTag()->getBuffer());

        return 1;
    }

    store->push(entry);
    return 0;
}

ResourceEntry *ResourceManager::retrieveResource(String *tag) {
    unsigned int i;
    for (i = 0; i < store->getCount(); i++) {
        ResourceEntry *entry = store->get(i);
        if (stringCompare(entry->getTag(), tag) == 0) {
            return entry;
        }
    }

    return NULL;
}

}
