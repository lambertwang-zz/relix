#include "character.h"

// Library
#include <stdlib.h>

// Engine
// #include "log/log.h"

// Game
#include "../item/item.h"

int attackCharacter(Object *aggressor, Object *victim) {
    CharacterData *attacker = aggressor->data,
                  *defender = victim->data;

    int result = 0;

    if (attacker->equipment != NULL) {
        result = rollDamage(attacker->equipment, defender->equipment);
    }
    if (result <= 0) {
        result = 1;
    }

    defender->health -= result;
    return result;
}

