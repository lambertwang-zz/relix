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

#include "utility/random.h"

// Game
#include "../relix.h"
#include "../map/map.h"
#include "../world/world.h"
#include "../character/character.h"
#include "../light/light.h"

/*
struct TorchData {
    int range;
    Color c;
    float factor;
};
*/

int renderTorchLight(Object *o, Screen *s) {
    // struct TorchData *data = o->data;

    pointLight(s, getWorldData()->current_map, o->pos, 
        // scaleColor(data->c, data->factor),
        (Color){160, 160, 80, 1.0},
        10);
        // data->range);
/*
    if (nrandom_f() < 0.05) {
        data->factor = nrandom_f() * 0.2 + 0.8;
    }

    if (nrandom_f() < 0.05) {
        data->range = nrandom_i(7, 10);
    }
    */
    return 1;
}

int lightMapListener(struct Object *o, Event *ev) {
    removeObject(o);
    return 0;
}

void dropLight(Point target) {
    struct Object *o = createObject();
    o->pos = target;
    o->pos.z = 8;
    /*

    struct TorchData *data = malloc(sizeof(struct TorchData));
    o->data = data;
    data->range = 8;
    data->c = (Color){192, 192, 90};
    data->factor = 1.0;
    */
    

    o->pix.fg = (Color){255, 255, 135, 1.0},

    o->renderLight = &renderTorchLight;

    listenEvent(o, EVENT_MAP, &lightMapListener);

    addObject(o);
}

int playerMapListener(Object *o, Event *ev) {
    MapEvent m_ev = *(MapEvent*)ev->data;
    
    o->pos.x = m_ev.map->player_start.x;
    o->pos.y = m_ev.map->player_start.y;

    return 0;
}

int playerListener(Object *o, Event *ev) {
    TickEvent *tick = ev->data;
    Event action;
    if (tick->act.code >= 0) {
        switch (tick->act.code) {
            case ACTION_MOVE:
                o->pos = tick->act.target;
                break;
            case ACTION_DOOR:
                action.id = EVENT_OPEN_DOOR;
                action.data = malloc(sizeof(DoorEvent));
                ((DoorEvent *) action.data)->p = tick->act.target;
                sendEvent(action);
                break;
            case ACTION_LIGHT:
                dropLight(tick->act.target);
                break;
            case ACTION_ATTACK:
                attackCharacter(o, tick->act.other);
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
    struct Object *player = createObject();

    sputf(player->type, TYPE_PLAYER);
    strcpy(player->pix.chr, "@");
    player->pix.fg = (Color){255, 32, 192, 1.0};

    player->pos = start;
    player->pos.z = PLAYER_DEPTH;

    player->data = createCharData();

    // CharacterData *data = player->data;

    player->update = &update_player;
    player->renderLight = &renderPlayerLight;
    player->close = &closePlayer;

    // listenEvent(player, EVENT_KEYBOARD, &playerKeyboardListener);
    listenEvent(player, EVENT_MAP, &playerMapListener);
    listenEvent(player, EVENT_TICK_PLAYER, &playerListener);

    addObject(player);

    return player;
}

