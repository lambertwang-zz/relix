#ifndef __WORLD_H__
#define __WORLD_H__

// Engine
#include "object/object.h"

// Game
#include "../action/action.h"

typedef struct World {
    struct Map *current_map;
    struct Tree map_tree;

    struct Array collisions;

    struct Object *player;
} World;

typedef struct TickEvent {
    struct Action act;
} TickEvent;

typedef struct DoorEvent {
    struct Point p;
} DoorEvent;

World *getWorldData();
void initWorld();

int getDefaultAction(Object *obj, Object *target);

int worldDoorListener(Object *o, Event *ev);
// int worldPlayerListener(Object *o, Event ev);
int worldKeyboardListener(Object *o, Event *ev);

#endif

