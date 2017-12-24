#ifndef __ITEM_H__
#define __ITEM_H__

// Engine
#include "string/string.h"
#include "list/array.h"
#include "list/tree.h"
#include "utility/dice.h"

// Item types
#define TYPE_WEAPON 1
#define TYPE_ARMOR 2
#define TYPE_ACCESSORY 3

// Slot types
#define SLOT_WEAPON 1
#define SLOT_HEAD   8
#define SLOT_HANDS  9
#define SLOT_TORSO  10
#define SLOT_FEET   11

// Item subtypes
#define ITEM_WEAPON_ONEHAND 1
// onehand weapons can be wielded in the offhand
#define ITEM_WEAPON_OFFHAND 2
// twohand weapons require both the main and off hands to wield
#define ITEM_WEAPON_TWOHAND 3

#define ITEM_HELMET 16
#define ITEM_CHEST 17
#define ITEM_PANTS 18
#define ITEM_GLOVE 19
#define ITEM_BOOTS 20

#define ITEM_RING 32
// Rings can be wielded in ring_2
#define ITEM_RING_2 33
#define ITEM_NECK 34
#define ITEM_BELT 35

#define ITEM_CONSUMABLE 64

//
#define ELEM_COUNT          6
#define ELEM_TRUE           0
#define ELEM_TRUE_STR       "true"
#define ELEM_PHYS           1
#define ELEM_PHYS_STR       "physical"
#define ELEM_FIRE           2
#define ELEM_FIRE_STR       "fire"
#define ELEM_ICE            3
#define ELEM_ICE_STR        "ice"
#define ELEM_LIGHTNING      4
#define ELEM_LIGHTNING_STR  "lightning"
#define ELEM_HOLY           5
#define ELEM_HOLY_STR       "holy"

typedef struct Item {
    int id;
    // Type (armor, weapon, etc)
    int type;
    // one-handed, two-handed, head, boots, etc
    int subclass;

    String *name;

    // Whether or not is a natural weapon
    int is_natural;

    void *data;
} Item;

typedef struct Slot {
    // Corresponds to slot type
    int type;

    /**
     * TODO: Enable extensibility for slots
     * Custom slots, multi-handed creatures, etc
     */
    char label[16]; // Hard-coded labels

    Item *item;
} Slot;

typedef struct Weapon {
    int range;
    int slot;

    Dice dam[ELEM_COUNT];
} Weapon;

typedef struct Armor{
    int slot;

    int def[ELEM_COUNT];
} Armor;

typedef struct Consumable {
} Consumable;

int rollDamage(Array *attack_equip, Array *def_equip);

Array *createEquipment(const int *slots);
void deleteEquipment(Array *tree);

int elementStrToInt(char *src);
int elementIntToStr(int src, char *dest);

#endif

