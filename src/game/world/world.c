// Library
#include <stdlib.h>

// Engine
#include "constants.h"
#include "log/log.h"
#include "object/objectManager.h"

// Game
#include "world.h"
#include "../map/map.h"
#include "../relix.h"
#include "../player/player.h"

int renderWorld(Object *o, Screen *s) {
    if (world.current_map == NULL) {
        return 0;
    }
    return renderMap(world.current_map, s);
}

void closeWorld(Object *o) {
    Iterator *it;
    it = initIterator(&world.map_tree);
    while (!done(it)) {
        Map *map = getNext(it)->data;
        clearMap(map);
        free(map);
    }
    closeIterator(it);

    closeArray(&world.collisions);
    closeTree(&world.map_tree);

    closeDefault(o);
}

void initWorld() {
    initArray(&world.collisions);
    initTree(&world.map_tree);
    world.current_map = NULL;

    // Register events with the worldmanager
    registerEvent(EVENT_MAP);
    registerEvent(EVENT_TICK_PLAYER);
    registerEvent(EVENT_TICK_OTHER);

    struct Object *o_world = malloc(sizeof(struct Object));

    initObject(o_world);

    o_world->render = &renderWorld;
    o_world->close = &closeWorld;

    listenEvent(o_world, EVENT_KEYBOARD, &worldKeyboardListener);

    addObject(o_world);

    world.current_map = malloc(sizeof(struct Map));
    initMap(world.current_map);
    insert(&world.map_tree, world.current_map, world.current_map->id);
    
    world.player = addPlayer(world.current_map->player_start);
}

Action moveToTile(struct Object *obj, Point p) {
    if (world.current_map == NULL) {
        return (Action){ACTION_IMPASSIBLE};
    }
    if (p.x < 0 || p.y < 0 || 
        p.x >= world.current_map->width || 
        p.y >= world.current_map->height) {
        return (Action){ACTION_IMPASSIBLE};
    }

    Tile tile = world.current_map->tiles[p.x + p.y * world.current_map->width];

    switch (tile.type) {
        case TILE_HALL:
        case TILE_ROOM:
        case TILE_OPEN:
            return (Action){ACTION_MOVE, p};
        case TILE_WALL:
            return (Action){ACTION_IMPASSIBLE, p};
        case TILE_DOOR:
            if (tile.solid) {
                return (Action){ACTION_DOOR, p};
            }
            return (Action){ACTION_MOVE, p};
        default:
            break;
    }

    getObjAt(&world.collisions, p, OBJ_SOLID);

    return (Action){ACTION_IMPASSIBLE};
}

int getDefaultAction(struct Object *obj, struct Object *target) {
    return 0;
}

int openDoor(Point p) {
    if (p.x < 0 || p.y < 0 || 
        p.x >= world.current_map->width || 
        p.y >= world.current_map->height) {
        return 1;
    }

    Tile *tile = &world.current_map->tiles[p.x + p.y * world.current_map->width];

    if (tile->type == TILE_DOOR) {
        if (tile->solid) {
            tile->solid = SOFT;
            tile->p.chr = '_';
        }
    }

    return 0;
}

