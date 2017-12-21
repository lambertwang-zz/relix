#ifndef __ASSET_H__
#define __ASSET_H__

// Engine
#include "constants.h"
#include "list/array.h"
#include "utility/dice.h"
#include "term/color.h"
#include "resource/parser.h"
#include "resource/resource.h"

#define MONSTER_DIR "monsters/"

// Monster json keys
#define TOK_FAMILY "family"
#define TOK_MIN_LEVEL "min_level"
#define TOK_MAX_LEVEL "max_level"
#define TOK_DEFAULT_CHAR "default_char"

#define TOK_ENTRIES "entries"

#define TOK_MON_NAME "name"
#define TOK_MON_ ""
#define TOK_MON_LEVEL "level"
#define TOK_MON_CHALLENGE "challenge"
#define TOK_MON_HD "hit_dice"
#define TOK_MON_NAT_WEAPON "natural_weapon"
#define TOK_MON_NAT_DAMAGE "natural_damage"
#define TOK_MON_LOOT_QUAL "loot_quality"
#define TOK_MON_COLOR "color"
#define TOK_MON_CHR "char"

typedef struct MonsterFamily {
    struct Array entries;
    // name
    String *name;
    // min_level
    int min_level;
    // max_level
    int max_level;
    // default_char
    String *default_chr;
} MonsterFamily;

typedef struct MonsterEntry {
    // name
    String *name;
    // level
    int level;
    // hit_dice
    struct Dice hit_dice;
    // natural_weapon
    struct Dice nat_weapon;
    // natural_damage
    int nat_damage;
    // loot_quality
    int loot_qual;
    // color
    Color color;
    // char
    String *chr;
} MonsterEntry;

int initResources();

int loadStringToEntry(String *dest, JsonNode *node);
int loadDiceToEntry(Dice *dest, JsonNode *node);
int loadIntToEntry(int *dest, JsonNode *node);

ResourceManager *getMonsterRes();

void initMonsterResources();
void closeMonsterResources();
int loadMonsterResources();

#endif

