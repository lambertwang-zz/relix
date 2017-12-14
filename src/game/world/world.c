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
#include "../component/component.h"

static World *world;

World *getWorldData() {
    return world;
}

int renderWorld(Object *o, Screen *s) {
    if (world->current_map == NULL) {
        return 0;
    }
    return renderMap(world->current_map, s);
}

void closeWorld(Object *o) {
    Iterator *it;

    it = initIterator(&world->map_tree);
    while (!done(it)) {
        Map *map = getNext(it)->data;
        closeMap(map);
        free(map);
    }
    closeIterator(it);

    closeArray(&world->collisions);
    closeTree(&world->map_tree);

    closeDefault(o);
}

void initWorld() {
    // Init map data
    world = malloc(sizeof(World));
    initArray(&world->collisions);
    initTree(&world->map_tree);

    // Register events with the worldmanager
    registerEvent(EVENT_MAP);
    registerEvent(EVENT_TICK_PLAYER);
    registerEvent(EVENT_TICK_OTHER);
    registerEvent(EVENT_OPEN_DOOR);

    struct Object *o_world = createObject();

    o_world->render = &renderWorld;
    o_world->close = &closeWorld;
    o_world->pos.z = WORLD_DEPTH;
    o_world->data = world;

    listenEvent(o_world, EVENT_KEYBOARD, &worldKeyboardListener);
    // listenEvent(o_world, EVENT_PLAYER_ACTION, &worldPlayerListener);
    listenEvent(o_world, EVENT_OPEN_DOOR, &worldDoorListener);

    addObject(o_world);

    // Create level and add player
    world->current_map = malloc(sizeof(struct Map));
    initMap(world->current_map);
    insert(&world->map_tree, world->current_map, world->current_map->id);
    
    world->player = addPlayer(world->current_map->player_start);

    // Initialize UI
}

int getDefaultAction(struct Object *obj, struct Object *target) {
    return 0;
}

