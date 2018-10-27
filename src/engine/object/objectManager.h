#ifndef __OBJECT_MANAGER_H__
#define __OBJECT_MANAGER_H__

// Engine
#include "object.h"

struct ObjectManager object_manager;

struct ObjectManager {
    // TODO: Enable storing and retrieving game states
    Tree insert_queue;
    Tree object_list;
    Tree remove_queue; 

    Tree event_listeners;

    int (*global_listener)(Event *);
};

int registerEvent(int ev_id);
int sendEvent(Event ev);

// Register event listeners for specific objects
int registerListener(const Object *obj, int ev_id);
int unregisterListener(const Object *obj, int ev_id);

// Register the global event listener.
// Only one global event listener may be set.
// The global event listener listens to all events.
void registerGlobalListener(int (*listener)(Event *));
void clearGlobalListener();

int initObjects();
int closeObjects();
void queueClear(void (*next)());

int addObject(struct Object *obj);
int removeObject(struct Object *obj);

int updateObjects();
int renderObjectLights();
int renderObjects();

// Exhaustive search
int getObjAt(Array *array, Point p, int solid);

#endif

