#include "player.h"

// Library
#include <stdlib.h>
#include <string.h>

// Engine
#include "constants.h"
#include "input/input.h"
#include "log/log.h"
#include "object/objectManager.h"
#include "game/game.h"

// Game
#include "../relix.h"
#include "../map/map.h"
#include "../world/world.h"
#include "../character/character.h"

void dropLight(Point target) {
    struct Object *o = malloc(sizeof(struct Object));
    initObject(o);
    o->pos = target;
    o->pos.z = 8;
    o->pix.chr = '1';

    o->pix.c_fg = (Color){255, 255, 135},

    o->renderLight = &renderPlayerLight;

    addObject(o);
}

int playerMapListener(struct Object *o, Event ev) {
    MapEvent m_ev = *(MapEvent*)ev.data;
    
    o->pos.x = m_ev.map->player_start.x;
    o->pos.y = m_ev.map->player_start.y;

    return 0;
}

int playerListener(Object *o, Event ev) {
    TickEvent *tick = ev.data;
    if (tick->act.code >= 0) {
        switch (tick->act.code) {
            case ACTION_MOVE:
                o->pos = tick->act.target;
                break;
            case ACTION_DOOR:
                openDoor(tick->act.target);
                break;
            case ACTION_LIGHT:
                dropLight(tick->act.target);
                break;
            default:
                break;
        }
    }
    return 0;
}

int update_player(Object *o) {
    setCamera(&screen_manager.main_screen, o->pos);
    return 0;
}

void closePlayer(Object *o) {
    CharacterData *data = o->data;

    closeCharData(data);

    closeDefault(o);
}

struct Object *addPlayer(Point start) {
    struct Object *player = malloc(sizeof(struct Object));

    initObject(player);

    strcpy(player->type, TYPE_PLAYER);
    player->pix.chr = '@';
    player->pix.c_fg = (Color){192, 32, 128};

    player->pos = start;
    player->pos.z = 10;

    player->data = malloc(sizeof(CharacterData));

    CharacterData *data = player->data;
    initCharData(data);

    player->update = &update_player;
    player->renderLight = &renderPlayerLight;
    player->close = &closePlayer;

    // listenEvent(player, EVENT_KEYBOARD, &playerKeyboardListener);
    listenEvent(player, EVENT_MAP, &playerMapListener);
    listenEvent(player, EVENT_TICK_PLAYER, &playerListener);

    addObject(player);

    return player;
}

