#ifndef __CHARACTER_H__
#define __CHARACTER_H__

// Engine
#include "list/tree.h"

#define MAX_NAME_LEN 64

typedef struct CharacterData {
    int max_health;
    int health;

    int level;

    Tree inventory;

    char name[MAX_NAME_LEN];
} CharacterData;

void initCharData(CharacterData *data);
void closeCharData(CharacterData *data);

#endif

