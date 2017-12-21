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
    Slot *slot = getWeapon(&attacker->equipment);

    Weapon damage;

    if (slot->item == NULL) {
        unsigned int i;
        for (i = 0; i < ELEM_COUNT; i++) {
            damage.dam[i] = NULL_DICE;
        }
        damage.dam[ELEM_PHYS] = (Dice){1, 4, 0};
    } else {
        damage = *(Weapon *)slot->item->data;
    }
    int result = rollDamage(damage, &defender->equipment);

    defender->health -= result;

    return result;
}

