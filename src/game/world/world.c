// Library
#include <stdlib.h>
#include <string.h>

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
    return renderMap(((World *) o->data)->current_map, s);
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

    closeTree(&world->map_tree);

    closeDefault(o);
}

void initWorld() {
    // Init map data
    world = malloc(sizeof(World));
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
    createPlayerPanel();
}

Action getDefaultAction(Object *obj, Object *other, Point p) {
    writeLog(LOG_WORLD, "world::getDefaultAction(): Finding encounter.");
    if (obj->id == other->id) {
        writeLog(LOG_WORLD, "world::getDefaultAction(): Object encountered self.");
        return (Action){ACTION_NONE, p, NULL};
    }
    if (!strcmp(obj->type->s, TYPE_PLAYER)) {
        if (!strcmp(other->type->s, TYPE_MONSTER)) {
            writeLog(LOG_WORLD, "world::getDefaultAction(): Player encountered monster.");
            return (Action){ACTION_ATTACK, p, other};
        }
    }
    // if (stringCompare(other->type
    return (Action){ACTION_NONE, p, NULL};
}

