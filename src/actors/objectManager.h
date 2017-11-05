#ifndef __OBJECT_MANAGER_H__
#define __OBJECT_MANAGER_H__

#include "object.h"
#include "../events/events.h"
#include "../list/tree.h"

struct ObjectManager object_manager;

struct ObjectManager {
    struct Tree insert_queue;
    struct Tree object_list;
    struct Tree remove_queue; 
};

int sendEvent(const Event *ev);
int registerListener(const struct Object *obj, int ev_id);
int registerEvent(int ev_id);

int initObjects();

#endif

