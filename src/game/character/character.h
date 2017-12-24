#ifndef __CHARACTER_H__
#define __CHARACTER_H__

// Engine
#include "list/tree.h"
#include "string/string.h"
#include "object/object.h"

// Game
#include "../asset/asset.h"

typedef struct CharacterData {
    String *name;

    // Points to an existing resource entry.
    // Does not need to be freed
    MonsterEntry *entry;

    int max_health;
    int health;

    int level;

    // Equipped on person
    // Array of slots
    // Must be manually initialized
    Array *equipment;
    // In bag
    Array inventory;
} CharacterData;

CharacterData *createCharData();
void closeCharData(CharacterData *data);

int attackCharacter(Object *aggressor, Object *victim);

#endif

