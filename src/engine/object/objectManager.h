#ifndef __OBJECT_MANAGER_H__
#define __OBJECT_MANAGER_H__

// Engine
#include "object.h"

struct ObjectManager object_manager;

struct ObjectManager {
    Tree insert_queue;
    Tree object_list;
    Tree remove_queue; 

    Tree **event_listeners;
    int event_count;
};

int sendEvent(Event ev);
int registerListener(const Object *obj, int ev_id);
int unregisterListener(const Object *obj, int ev_id);
int registerEvent(int ev_id);

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

