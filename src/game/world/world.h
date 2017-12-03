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

World world;

void initWorld();

struct Action moveToTile(struct Object *obj, struct Point p);

int getDefaultAction(struct Object *obj, struct Object *target);

int worldKeyboardListener(struct Object *o, struct Event ev);

int openDoor(Point p);

#endif

