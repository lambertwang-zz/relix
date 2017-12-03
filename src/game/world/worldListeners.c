// Library
#include <stdlib.h>

// Engine
#include "event/event.h"
#include "object/objectManager.h"
#include "input/input.h"

// Game
#include "world.h"
#include "../relix.h"


#include "log/log.h"

int worldKeyboardPlayerActive(Object *o, Event ev) {
    KeyboardEvent k_ev = *(KeyboardEvent *)ev.data;
    Point target = world.player->pos;
    switch (k_ev.type) {
        case KEYBOARD_NORMAL:
            switch (k_ev.value) {
                case 'K':
                case 'k':
                    target.y--;
                    break;
                case ARROW_DOWN:
                case 'J':
                case 'j':
                    target.y++;
                    break;
                case ARROW_LEFT:
                case 'H':
                case 'h':
                    target.x--;
                    break;
                case ARROW_RIGHT:
                case 'L':
                case 'l':
                    target.x++;
                    break;
            }
            break;
        case KEYBOARD_ESCAPE:
            switch (k_ev.value) {
                case ARROW_UP:
                    target.y--;
                    break;
                case ARROW_DOWN:
                    target.y++;
                    break;
                case ARROW_LEFT:
                    target.x--;
                    break;
                case ARROW_RIGHT:
                    target.x++;
                    break;
            }
            break;
        default:
            return 0;
    }

    Event ev_tick;
    ev_tick.id = EVENT_TICK_PLAYER;
    ev_tick.data = malloc(sizeof(TickEvent));
    ((TickEvent *) ev_tick.data)->act = moveToTile(world.player, target);
    sendEvent(ev_tick);
    
    return 0;
}

int worldKeyboardListener(Object *o, Event ev) {
    return worldKeyboardPlayerActive(o, ev);
}

