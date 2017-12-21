#include "item.h"

// Library
#include <string.h>
#include <stdlib.h>

#include "log/log.h"

Slot *getWeapon(Tree *equip) {
    Slot *item = getData(equip, ITEM_WEAPON_ONEHAND);
    if (item == NULL) {
        item = getData(equip, ITEM_WEAPON_OFFHAND);
    }

    return item;
}

int rollDamage(Weapon weapon, Tree *def) {
    unsigned int i, j;
    int def_sum[ELEM_COUNT];
    for (i = 0; i < ELEM_COUNT; i++) {
        def_sum[i] = 0;
    }

    for (i = ITEM_HELMET; i <= ITEM_BOOTS; i++) {
        Slot *slot = getData(def, i);
        if (slot != NULL && slot->item != NULL) {
            Armor *defense = slot->item->data;
            for (j = 0; j < ELEM_COUNT; j++) {
                def_sum[j] += defense->def[j];
            }
        }
    }
    
    int result = 0;
    for (i = 0; i < ELEM_COUNT; i++) {
        int roll = rollDice(weapon.dam[i]);
        // TOP SECRET DAMAGE FORMULA
        if (roll > 0) {
            result += (roll * roll) / (roll + def_sum[i]);
        }
    }

    return result;
}

void initEquipment(Tree *tree) {
    initTree(tree);

    // Weapons
    Slot *slot = malloc(sizeof(Slot));
    slot->item = NULL;
    slot->id = ITEM_WEAPON_ONEHAND;
    insert(tree, slot, slot->id);

    slot = malloc(sizeof(Slot));
    slot->item = NULL;
    slot->id = ITEM_WEAPON_OFFHAND;
    insert(tree, slot, slot->id);

    // Armor
    slot = malloc(sizeof(Slot));
    slot->item = NULL;
    slot->id = ITEM_HELMET;
    insert(tree, slot, slot->id);

    slot = malloc(sizeof(Slot));
    slot->item = NULL;
    slot->id = ITEM_CHEST;
    insert(tree, slot, slot->id);

    slot = malloc(sizeof(Slot));
    slot->item = NULL;
    slot->id = ITEM_PANTS;
    insert(tree, slot, slot->id);

    slot = malloc(sizeof(Slot));
    slot->item = NULL;
    slot->id = ITEM_GLOVE;
    insert(tree, slot, slot->id);

    slot = malloc(sizeof(Slot));
    slot->item = NULL;
    slot->id = ITEM_BOOTS;
    insert(tree, slot, slot->id);

    // Accessories
    slot = malloc(sizeof(Slot));
    slot->item = NULL;
    slot->id = ITEM_RING;
    insert(tree, slot, slot->id);

    slot = malloc(sizeof(Slot));
    slot->item = NULL;
    slot->id = ITEM_RING_2;
    insert(tree, slot, slot->id);

    slot = malloc(sizeof(Slot));
    slot->item = NULL;
    slot->id = ITEM_NECK;
    insert(tree, slot, slot->id);

    slot = malloc(sizeof(Slot));
    slot->item = NULL;
    slot->id = ITEM_BELT;
    insert(tree, slot, slot->id);
}

void closeEquipment(Tree *tree) {
    Iterator *it = initIterator(tree);
    while (!done(it)) {
        Slot *slot = getNext(it)->data;
        free(slot);
    }
    closeIterator(it);

    closeTree(tree);
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

