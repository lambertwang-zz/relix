#include "character.h"

void initCharData(CharacterData *data) {
    data->max_health = 0;
    data->health = 0;

    data->level = 1;

    initTree(&data->inventory);
}

void closeCharData(CharacterData *data) {
    closeTree(&data->inventory);
}

