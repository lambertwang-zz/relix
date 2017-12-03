// Library
#include <stdlib.h>

// Engine
#include "event/event.h"
#include "object/objectManager.h"
#include "input/input.h"

// Game
#include "world.h"
#include "../map/map.h"
#include "../relix.h"


#include "log/log.h"

int worldKeyboardPlayerActive(Object *o, Event ev) {
    KeyboardEvent k_ev = *(KeyboardEvent *)ev.data;
    Point target = world.player->pos;

    Event ev_tick;
    ev_tick.id = EVENT_TICK_PLAYER;
    ev_tick.data = malloc(sizeof(TickEvent));

    switch (k_ev.type) {
        case KEYBOARD_NORMAL:
            switch (k_ev.value) {
                case 'K':
                case 'k':
                    target.y--;
                    ((TickEvent *) ev_tick.data)->act = moveToTile(world.player, target);
                    break;
                case ARROW_DOWN:
                case 'J':
                case 'j':
                    target.y++;
                    ((TickEvent *) ev_tick.data)->act = moveToTile(world.player, target);
                    break;
                case ARROW_LEFT:
                case 'H':
                case 'h':
                    target.x--;
                    ((TickEvent *) ev_tick.data)->act = moveToTile(world.player, target);
                    break;
                case ARROW_RIGHT:
                case 'L':
                case 'l':
                    target.x++;
                    ((TickEvent *) ev_tick.data)->act = moveToTile(world.player, target);
                    break;
                case ' ':
                    ((TickEvent *) ev_tick.data)->act = (Action){ACTION_LIGHT, target};
                    break;
                case '1':
                case '2':
                case '3':
                    free(ev_tick.data);
                    generateMap(world.current_map, k_ev.value - '1', 200, 70);
                    return 0;
            }
            break;
        case KEYBOARD_ESCAPE:
            switch (k_ev.value) {
                case ARROW_UP:
                    target.y--;
                    ((TickEvent *) ev_tick.data)->act = moveToTile(world.player, target);
                    break;
                case ARROW_DOWN:
                    target.y++;
                    ((TickEvent *) ev_tick.data)->act = moveToTile(world.player, target);
                    break;
                case ARROW_LEFT:
                    target.x--;
                    ((TickEvent *) ev_tick.data)->act = moveToTile(world.player, target);
                    break;
                case ARROW_RIGHT:
                    target.x++;
                    ((TickEvent *) ev_tick.data)->act = moveToTile(world.player, target);
                    break;
            }
            break;
        default:
            return 0;
    }

    sendEvent(ev_tick);
    
    return 0;
}

int worldKeyboardListener(Object *o, Event ev) {
    return worldKeyboardPlayerActive(o, ev);
}

