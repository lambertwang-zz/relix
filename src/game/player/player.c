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
#include "../item/item.h"

int playerMapListener(Object *o, Event *ev) {
    MapEvent m_ev = *(MapEvent*)ev->data;
    
    o->pos.x = m_ev.map->player_start.x;
    o->pos.y = m_ev.map->player_start.y;

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

    // Create and setchar data
    CharacterData *data = createCharData();
    player->data = data;

    int player_slots[8];
    player_slots[0] = SLOT_WEAPON;
    player_slots[1] = SLOT_WEAPON;
    player_slots[2] = SLOT_HEAD;
    player_slots[3] = SLOT_TORSO;
    player_slots[4] = SLOT_HANDS;
    player_slots[5] = SLOT_FEET;
    player_slots[6] = -1;

    data->equipment = createEquipment(player_slots);

    // Set callbacks and listeners
    player->update = &update_player;
    player->renderLight = &renderPlayerLight;
    player->close = &closePlayer;

    // listenEvent(player, EVENT_KEYBOARD, &playerKeyboardListener);
    listenEvent(player, EVENT_MAP, &playerMapListener);
    listenEvent(player, EVENT_TICK_PLAYER, &playerListener);

    addObject(player);

    return player;
}

