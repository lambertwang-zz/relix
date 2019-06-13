#ifndef __RESOURCE_H__
#define __RESOURCE_H__

// Engine
#include "constants.h"
#include "list/array.h"
#include "utility/dice.h"
#include "string/string.h"
#include "manager/manager.h"

namespace rlx {
    class ResourceEntry {
    private:
        String *tag;
    public:
        virtual ~ResourceEntry() {
            delete tag;
        }

        String *getTag() {
            return tag;
        }
    };

    class ResourceManager: public Manager {
    private:
        Array<ResourceEntry> *store;
    public:
        ResourceManager();
        ~ResourceManager();

        int registerResource(ResourceEntry *entry);
        ResourceEntry *retrieveResource(String *tag);
    };
}

#endif

