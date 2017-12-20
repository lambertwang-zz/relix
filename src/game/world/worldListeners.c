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
    writeLog(10, "received door event");
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

    getObjAt(&world->collisions, p, OBJ_SOLID);

    return (Action){ACTION_IMPASSIBLE};
}

int worldKeyboardPlayerActive(Object *o, Event *ev) {
    World *world = getWorldData();
    KeyboardEvent k_ev = *(KeyboardEvent *)ev->data;
    Point target = world->player->pos;

    Event ev_tick;
    ev_tick.id = EVENT_TICK_PLAYER;
    ev_tick.data = malloc(sizeof(TickEvent));

    switch (k_ev.type) {
        case KEYBOARD_NORMAL:
            switch (k_ev.value) {
                case 'K':
                case 'k':
                    target.y--;
                    ((TickEvent *) ev_tick.data)->act = worldPlayerHandler(world, target);
                    break;
                case ARROW_DOWN:
                case 'J':
                case 'j':
                    target.y++;
                    ((TickEvent *) ev_tick.data)->act = worldPlayerHandler(world, target);
                    break;
                case ARROW_LEFT:
                case 'H':
                case 'h':
                    target.x--;
                    ((TickEvent *) ev_tick.data)->act = worldPlayerHandler(world, target);
                    break;
                case ARROW_RIGHT:
                case 'L':
                case 'l':
                    target.x++;
                    ((TickEvent *) ev_tick.data)->act = worldPlayerHandler(world, target);
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
                    ((TickEvent *) ev_tick.data)->act = worldPlayerHandler(world, target);
                    break;
                case ARROW_DOWN:
                    target.y++;
                    ((TickEvent *) ev_tick.data)->act = worldPlayerHandler(world, target);
                    break;
                case ARROW_LEFT:
                    target.x--;
                    ((TickEvent *) ev_tick.data)->act = worldPlayerHandler(world, target);
                    break;
                case ARROW_RIGHT:
                    target.x++;
                    ((TickEvent *) ev_tick.data)->act = worldPlayerHandler(world, target);
                    break;
            }
            break;
        default:
            return 0;
    }

    sendEvent(ev_tick);
    
    return 0;
}

int worldKeyboardListener(Object *o, Event *ev) {
    return worldKeyboardPlayerActive(o, ev);
}

