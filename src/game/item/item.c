#include "item.h"

// Library
#include <string.h>
#include <stdlib.h>

#include "log/log.h"

int rollDamage(Array *attack_equip, Array *def_equip) {
    unsigned int i, j;
    int def_sum[ELEM_COUNT];
    for (i = 0; i < ELEM_COUNT; i++) {
        def_sum[i] = 0;
    }

    if (def_equip != NULL) {
        for (i = 0; i <= def_equip->count; i++) {
            Slot *slot = getDataArray(def_equip, i);
            if (slot->item != NULL && slot->item->type == TYPE_ARMOR) {
                Armor *defense = slot->item->data;
                for (j = 0; j < ELEM_COUNT; j++) {
                    def_sum[j] += defense->def[j];
                }
            }
        }
    }
    
    int result = 0;
    for (j = 0; j < attack_equip->count; j++) {
        Slot *slot = getDataArray(attack_equip, j);
        if (slot->item != NULL && slot->item->type == TYPE_WEAPON) {
            for (i = 0; i < ELEM_COUNT; i++) {
                int roll = rollDice(((Weapon *) slot->item->data)->dam[i]);
                // TOP SECRET DAMAGE FORMULA
                if (roll > 0) {
                    result += (roll * roll) / (roll + def_sum[i]);
                }
            }
        }
    }

    return result;
}

Array *createEquipment(const int *slots) {
    Array *equip = malloc(sizeof(Array));
    initArray(equip);

    unsigned int i = 0;
    while (slots[i] >= 0) {
        Slot *slot = malloc(sizeof(Slot));
        slot->item = NULL;
        slot->type = slots[i++];
        push(equip, slot);
    }

    return equip;
}

void deleteEquipment(Array *equip) {
    if (equip != NULL) {
        unsigned int i;
        for (i = 0; i < equip->count; i++) {
            free(getDataArray(equip, i));
        }

        closeArray(equip);
        free(equip);
    }
}

int elementStrToInt(char *src) {
    if (strcmp(src, ELEM_TRUE_STR)) {
        return ELEM_TRUE;
    }
    if (strcmp(src, ELEM_PHYS_STR)) {
        return ELEM_PHYS;
    }
    if (strcmp(src, ELEM_FIRE_STR)) {
        return ELEM_FIRE;
    }
    if (strcmp(src, ELEM_ICE_STR)) {
        return ELEM_ICE;
    }
    if (strcmp(src, ELEM_LIGHTNING_STR)) {
        return ELEM_LIGHTNING;
    }
    if (strcmp(src, ELEM_HOLY_STR)) {
        return ELEM_HOLY;
    }
    return -1;
}

int elementIntToStr(int src, char *dest) {
    switch (src) {
        case ELEM_TRUE:
            strcpy(dest, ELEM_TRUE_STR);
            return 0;
        case ELEM_PHYS:
            strcpy(dest, ELEM_PHYS_STR);
            return 0;
        case ELEM_FIRE:
            strcpy(dest, ELEM_FIRE_STR);
            return 0;
        case ELEM_ICE:
            strcpy(dest, ELEM_ICE_STR);
            return 0;
        case ELEM_LIGHTNING:
            strcpy(dest, ELEM_LIGHTNING_STR);
            return 0;
        case ELEM_HOLY:
            strcpy(dest, ELEM_HOLY_STR);
            return 0;
        default:
            return 1;
    }
}

