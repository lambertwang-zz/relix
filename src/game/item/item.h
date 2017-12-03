#ifndef __ITEM_H__
#define __ITEM_H__

#define ITEM_NAME_MAX_LEN 256

#define ITEM_WEAPON 1
#define ITEM_WEARABLE 2
#define ITEM_CONSUMABLE 3

typedef struct Item {
    int id;
    int type;
    char name[ITEM_NAME_MAX_LEN];

    void *data;
} Item;

#define SLOT_ONEHAND 1
#define SLOT_OFFHAND 2
#define SLOT_TWOHAND 3

#define SLOT_HEAD 16
#define SLOT_BODY 17
#define SLOT_LEGS 18
#define SLOT_ARMS 19
#define SLOT_HANDS 20

#define SLOT_NECK 32
#define SLOT_RING 33
#define SLOT_WAIST 34

typedef struct Weapon {
    int range;
    int slot;

    struct Damage *dam;
    int damage_count;
} Weapon;

typedef struct Wearable {
    int slot;

    struct Defense *def;
    int def_count;
} Wearable;

typedef struct Consumable {
} Consumable;

#define ELEM_TRUE 0
#define ELEM_PHYS 1
#define ELEM_FIRE 2
#define ELEM_ICE 3
#define ELEM_LIGHTNING 4
#define ELEM_HOLY 5

typedef struct Damage {
    int element;
    int min_damage;
    int max_damage;
} Damage;

typedef struct Defense {
    int element;
    int defense;
} Defense;

#endif

