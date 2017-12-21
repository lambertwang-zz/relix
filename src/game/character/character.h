#ifndef __CHARACTER_H__
#define __CHARACTER_H__

// Engine
#include "list/tree.h"
#include "string/string.h"
#include "object/object.h"

typedef struct CharacterData {
    String *name;

    int max_health;
    int health;

    int level;

    // Equipped on person
    Tree equipment;
    // In bag
    Tree inventory;
} CharacterData;

CharacterData *createCharData();
void closeCharData(CharacterData *data);

int attackCharacter(Object *aggressor, Object *victim);

#endif

