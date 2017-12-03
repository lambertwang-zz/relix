#ifndef __MONSTER_H__
#define __MONSTER_H__

// Engine
#include "constants.h"
#include "list/array.h"
#include "utility/dice.h"

#define MONSTER_DIR "monsters/"

typedef struct MonsterFamily {
    char label[LABEL_MED];
    struct Array entries;
} MonsterFamily;

typedef struct MosnterEntry {
    char name[LABEL_MED];
    int min_level;
    int max_level;
    struct Dice hit_dice;
} MonsterEntry;

int initResources();
int loadMonsterResources();

#endif

