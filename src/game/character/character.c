#include "character.h"

// Library
#include <stdlib.h>

// Game
#include "../item/item.h"

#include "log/log.h"

CharacterData *createCharData() {
    CharacterData *data = malloc(sizeof(CharacterData));

    data->name = createString();
    data->max_health = 0;
    data->health = 0;

    data->level = 1;

    initEquipment(&data->equipment);
    initTree(&data->inventory);

    return data;
}

void closeCharData(CharacterData *data) {
    deleteString(data->name);
    closeEquipment(&data->equipment);
    closeTree(&data->inventory);
}

