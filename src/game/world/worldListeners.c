// Library
#include <stdlib.h>
#include <string.h>

// Engine
#include "event/event.h"
#include "object/objectManager.h"
#include "input/input.h"

// Game
#include "world.h"
#include "../map/map.h"
#include "../relix.h"


#include "log/log.h"

int worldDoorListener(Object *o, Event *ev) {
    writeLog(LOG_WORLD, "worldListeners::worldDoorListener(): Received door event.");
    World *world = o->data;
    Point p = ((DoorEvent *)ev->data)->p;
    if (p.x < 0 || p.y < 0 || 
        p.x >= world->current_map->width || 
        p.y >= world->current_map->height) {
        return 1;
    }

    Tile *tile = &world->current_map->tiles[p.x + p.y * world->current_map->width];

    if (tile->type == TILE_DOOR) {
        if (tile->solid) {
            tile->solid = SOFT;
            strcpy(tile->p.chr, "_");
        }
    }

    return 0;
}

Action worldPlayerHandler(World *world, Point p) {
    if (world->current_map == NULL) {
        return (Action){ACTION_IMPASSIBLE, (Point){0, 0, 0}, NULL};
    }
    if (p.x < 0 || p.y < 0 || 
        p.x >= world->current_map->width || 
        p.y >= world->current_map->height) {
        return (Action){ACTION_IMPASSIBLE, p, NULL};
    }

    // First check for interactions between other objects
    Array collisions;
    initArray(&collisions);
    getObjAt(&collisions, p, OBJ_SOLID);

    Object *other;
    if (collisions.count > 0) {
        writeLog(LOG_WORLD, "worldListeners::worldPlayerHandler(): Collided with something.");
        // Find collision object with highest depth
        other = getDataArray(&collisions, 0);
        unsigned int i;
        for (i = 1; i < collisions.count; i++) {
            Object *next = getDataArray(&collisions, i);
            if (next->pix.depth >= other->pix.depth) {
                other = next;
            }
        }
        closeArray(&collisions);
        return getDefaultAction(world->player, other, p);
    }

    closeArray(&collisions);

    // Next check for interactions with the map
    Tile tile = world->current_map->tiles[p.x + p.y * world->current_map->width];

    switch (tile.type) {
        case TILE_HALL:
        case TILE_ROOM:
        case TILE_OPEN:
            return (Action){ACTION_MOVE, p, NULL};
        case TILE_WALL:
            return (Action){ACTION_IMPASSIBLE, p, NULL};
        case TILE_DOOR:
            if (tile.solid) {
                return (Action){ACTION_DOOR, p, NULL};
            }
            return (Action){ACTION_MOVE, p, NULL};
        default:
            break;
    }

    return (Action){ACTION_IMPASSIBLE, p, NULL};
}

int worldKeyboardPlayerActive(Object *o, Event *ev) {
    World *world = o->data;
    KeyboardEvent k_ev = *(KeyboardEvent *)ev->data;
    Point target = world->player->pos;

    Event ev_tick;
    ev_tick.id = EVENT_TICK_PLAYER;
    ev_tick.data = malloc(sizeof(TickEvent));
    TickEvent *data = ev_tick.data;
    data->act.code = ACTION_NONE;

    switch (k_ev.type) {
        case KEYBOARD_NORMAL:
            switch (k_ev.value) {
                case 'K':
                case 'k':
                    target.y--;
                    data->act.code = ACTION_MOVE;
                    break;
                case ARROW_DOWN:
                case 'J':
                case 'j':
                    target.y++;
                    data->act.code = ACTION_MOVE;
                    break;
                case ARROW_LEFT:
                case 'H':
                case 'h':
                    target.x--;
                    data->act.code = ACTION_MOVE;
                    break;
                case ARROW_RIGHT:
                case 'L':
                case 'l':
                    target.x++;
                    data->act.code = ACTION_MOVE;
                    break;
                case ' ':
                    ((TickEvent *) ev_tick.data)->act = (Action){ACTION_LIGHT, target, NULL};
                    break;
                case '1':
                case '2':
                case '3':
                    free(ev_tick.data);
                    generateMap(world->current_map, k_ev.value - '1', 200, 70);
                    return 0;
            }
            break;
        case KEYBOARD_ESCAPE:
            switch (k_ev.value) {
                case ARROW_UP:
                    target.y--;
                    data->act.code = ACTION_MOVE;
                    break;
                case ARROW_DOWN:
                    target.y++;
                    data->act.code = ACTION_MOVE;
                    break;
                case ARROW_LEFT:
                    target.x--;
                    data->act.code = ACTION_MOVE;
                    break;
                case ARROW_RIGHT:
                    target.x++;
                    data->act.code = ACTION_MOVE;
                    break;
            }
            break;
        default:
            free(ev_tick.data);
            return 0;
    }

    switch (data->act.code) {
        case ACTION_MOVE:
            data->act = worldPlayerHandler(world, target);
            break;
        default:
            free(ev_tick.data);
            return 0;
    }

    sendEvent(ev_tick);
    
    return 0;
}

int worldKeyboardListener(Object *o, Event *ev) {
    return worldKeyboardPlayerActive(o, ev);
}

