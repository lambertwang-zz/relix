#include "player.h"

// Library
#include <stdlib.h>

// Engine
#include "object/objectManager.h"

// Game
#include "../relix.h"
#include "../world/world.h"
#include "../light/light.h"
#include "../character/character.h"

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
    ev = ev;
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

void playerAttack(Object *player, Object *other) {
    int damage = attackCharacter(player, other);

    Event action;
    action.id = EVENT_MESSAGE;
    action.data = malloc(sizeof(String **));
    *((String **) action.data) = new String();
    sputf(*((String **) action.data), "Attacked %s for %d damage!", ((CharacterData *) other->data)->name->s, damage);
    sendEvent(action);

    // Set char info to target
    Event char_panel;
    char_panel.id = EVENT_CHAR_INFO;
    char_panel.data = malloc(sizeof(Object **));
    *((Object **) char_panel.data) = other;
    sendEvent(char_panel);
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
                playerAttack(o, tick->act.other);
                break;
            default:
                break;
        }
    }
    return 0;
}

