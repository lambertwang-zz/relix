#include "constants.h"
#include "log/log.h"
#include "objects/object.h"
#include "objects/objectManager.h"
#include "game/game.h"
#include "term/screen.h"

#include "input/input.h"

#include "stdlib.h"

#include "../relix.h"
#include "../map/map.h"

int playerKeyboardListener(struct Object *o, Event ev) {
    KeyboardEvent k_ev = *(KeyboardEvent *)ev.data;
    switch (k_ev.type) {
        case KEYBOARD_NORMAL:
            default:
                return 0;
        case KEYBOARD_ESCAPE:
            switch (k_ev.value) {
                case ARROW_UP:
                    o->pos.y--;
                    break;
                case ARROW_DOWN:
                    o->pos.y++;
                    break;
                case ARROW_LEFT:
                    o->pos.x--;
                    break;
                case ARROW_RIGHT:
                    o->pos.x++;
                    break;
            }
            return 0;
    }
    
    return 0;
}

int playerMapListener(struct Object *o, Event ev) {
    MapEvent m_ev = *(MapEvent*)ev.data;
    
    o->pos.x = m_ev.map->player_start.x;
    o->pos.y = m_ev.map->player_start.y;

    return 0;
}

int update_player(struct Object *o) {
    setCamera(o->pos);
    return 0;
}

struct Object *addPlayer() {
    struct Object *player = malloc(sizeof(struct Object));

    initObject(player);

    player->pix.chr = '@';
    player->pix.fg = 128;
    player->pos.z = 10;

    player->update = &update_player;

    listenEvent(player, EVENT_KEYBOARD, &playerKeyboardListener);
    listenEvent(player, EVENT_MAP, &playerMapListener);

    addObject(player);

    return player;
}
