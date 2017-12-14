#include "asset.h"

// Library
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

// Engine
#include "constants.h"
#include "log/log.h"

// Game
#include "../relix.h"
#include "../item/item.h"

MonsterEntry *createEmptyMonsterEntry() {
    MonsterEntry *new_entry = malloc(sizeof(MonsterEntry));
    new_entry->name = createString();
    new_entry->level = -1;
    new_entry->challenge = -1;
    new_entry->hit_dice = NULL_DICE;
    new_entry->nat_weapon = NULL_DICE;
    new_entry->nat_damage = ELEM_PHYS;
    new_entry->loot_qual = 100;

    new_entry->chr = createString();

    return new_entry;
}

void fillEmptyMonsterEntry(MonsterEntry *entry, MonsterFamily *family) {
    if (entry->challenge == -1) {
        entry->challenge = entry->level;
    }

    if (entry->chr->len == 0) {
        stringCopy(entry->chr, family->default_chr);
    }
}

MonsterEntry *parseMonsterEntry(JsonNode *monster_json) {
    unsigned int i;

    MonsterEntry *new_entry = malloc(sizeof(MonsterEntry));

    Array *monster_props = monster_json->data;
    for (i = 0; i < monster_props->count; i++) {
        JsonObjProp *prop = getDataArray(monster_props, i);

        if (!strcmp(prop->key->s, TOK_MON_NAME)) {
            loadStringToEntry(new_entry->name, prop->value);
            continue;
        }
        if (!strcmp(prop->key->s, TOK_MON_LEVEL)) {
            loadIntToEntry(&new_entry->level, prop->value);
            continue;
        }
        if (!strcmp(prop->key->s, TOK_MON_CHALLENGE)) {
            loadIntToEntry(&new_entry->challenge, prop->value);
            continue;
        }
        if (!strcmp(prop->key->s, TOK_MON_HD)) {
            loadDiceToEntry(&new_entry->hit_dice, prop->value);
            continue;
        }
        if (!strcmp(prop->key->s, TOK_MON_NAT_WEAPON)) {
            loadDiceToEntry(&new_entry->nat_weapon, prop->value);
            continue;
        }
        if (!strcmp(prop->key->s, TOK_MON_NAT_DAMAGE)) {
            if (prop->value->type == JSON_STRING) {
                new_entry->nat_damage = elementStrToInt(prop->value->data);
            }
            continue;
        }
        if (!strcmp(prop->key->s, TOK_MON_LOOT_QUAL)) {
            loadIntToEntry(&new_entry->loot_qual, prop->value);
            continue;
        }
        if (!strcmp(prop->key->s, TOK_MON_COLOR)) {
            if (prop->value->type == JSON_ARRAY) {
                int *r = getDataArray(prop->value->data, 0),
                    *g = getDataArray(prop->value->data, 0),
                    *b = getDataArray(prop->value->data, 0);
                new_entry->color = (Color){
                    r == NULL ? 0 : *r,
                    g == NULL ? 0 : *g,
                    b == NULL ? 0 : *b,
                    1.0
                };
            }
            continue;
        }
        if (!strcmp(prop->key->s, TOK_MON_CHR)) {
            loadStringToEntry(new_entry->chr, prop->value);
            continue;
        }
    }

    return new_entry;
}

int closeMonsterFamily(MonsterFamily *family) {
    unsigned int i;

    for (i = 0; i < family->entries.count; i++) {
        free(getDataArray(&family->entries, i));
    }

    closeArray(&family->entries);
    free(family);
    return 0;
}

int loadMonsterFile(FILE *file) {
    JsonNode *node = parseFile(file);

    if (node->type != JSON_OBJ) {
        writeLog(LOG_LOAD, "monster_load::loadMonsterFile(): ERROR: Expected Json Object in file.");
        return 0;
    }
    unsigned int i;

    MonsterFamily *family = malloc(sizeof(MonsterFamily));
    initArray(&family->entries);

    JsonObjData *data = node->data;
    for (i = 0; i < data->props.count; i++) {
        JsonObjProp *prop = getDataArray(&data->props, i);
        writeLog(LOG_LOAD_V, "monster_load::loadMonsterFile(): Parsing token '%s'.", prop->key);
        if (!strcmp(prop->key->s, TOK_FAMILY)) {
            if (loadStringToEntry(family->name, prop->value)) {
                writeLog(LOG_LOAD, "monster_load::loadMonsterFile(): ERROR: Cannot parse '%s'.", TOK_FAMILY);
            }
            continue;
        }
        if (!strcmp(prop->key->s, TOK_MIN_LEVEL)) {
            if (loadIntToEntry(&family->min_level, prop->value)) {
                writeLog(LOG_LOAD, "monster_load::loadMonsterFile(): ERROR: Cannot parse '%s'.", TOK_MIN_LEVEL);
            }
            continue;
        }
        if (!strcmp(prop->key->s, TOK_MAX_LEVEL)) {
            if (loadIntToEntry(&family->max_level, prop->value)) {
                writeLog(LOG_LOAD, "monster_load::loadMonsterFile(): ERROR: Cannot parse '%s'.", TOK_MAX_LEVEL);
            }
            continue;
        }
        if (!strcmp(prop->key->s, TOK_DEFAULT_CHAR)) {
            if (loadStringToEntry(family->default_chr, prop->value)) {
                writeLog(LOG_LOAD, "monster_load::loadMonsterFile(): ERROR: Cannot parse '%s'.", TOK_DEFAULT_CHAR);
            }
            continue;
        }
    }

    JsonNode *entries = getObjValue(node, TOK_ENTRIES);

    if (entries == NULL || entries->type != JSON_ARRAY) {
        writeLog(LOG_LOAD, "monster_load::loadMonsterFile(): ERROR: Expected array property with key %s.", TOK_ENTRIES);
        closeJsonNode(node);
        closeMonsterFamily(family);
        return 0;
    }

    Array *monster_objs = entries->data;
    writeLog(LOG_LOAD, "monster_load::loadMonsterFile(): Found %d monster entries.", monster_objs->count);
    for (i = 0; i < monster_objs->count; i++) {
        MonsterEntry *new_entry = parseMonsterEntry(getDataArray(monster_objs, i));
        fillEmptyMonsterEntry(new_entry, family);
        push(&family->entries, new_entry);
    }

    closeJsonNode(node);
    writeLog(LOG_LOAD, "monster_load::loadMonsterFile(): Loaded %d monster entries.", family->entries.count);
    return family->entries.count;
}

int loadMonsterResources() {
    String *monster_dir = createString();
    sputf(monster_dir, "%s%s", ASSET_DIR, MONSTER_DIR);

    struct dirent *p_dirent;
    DIR *p_dir;

    p_dir = opendir(monster_dir->s);
    if (p_dir == NULL) {
        writeLog(LOG_LOAD, "monster_load::loadMonsterResources(): ERROR: Cannot open directory '%s'", monster_dir);
        return 1;
    }
    writeLog(LOG_LOAD_V, "monster_load::loadMonsterResources(): Attempting to open directory '%s'", monster_dir);

    char *file_ext;

    while ((p_dirent = readdir(p_dir)) != NULL) {
        file_ext = strrchr(p_dirent->d_name, '.');
        writeLog(LOG_LOAD, "'%s'", file_ext + 1);
        writeLog(LOG_LOAD, "'%d'", strcmp(file_ext + 1, "json"));
        if (file_ext == NULL || strcmp(file_ext + 1, "json")) {
            if (strcmp(p_dirent->d_name, ".") && strcmp(p_dirent->d_name, "..")) {
                writeLog(LOG_LOAD, "monster_load::loadMonsterResources(): WARN: Found non json file '%s'", p_dirent->d_name);
            }
            continue;
        }

        sputf(monster_dir, "%s%s%s", ASSET_DIR, MONSTER_DIR, p_dirent->d_name);
        FILE *mon_file = fopen(monster_dir->s, "r");
        if (mon_file == NULL) {
            writeLog(LOG_LOAD, "monster_load::loadMonsterResources(): ERROR: Cannot open file '%s'", monster_dir);
            continue;
        }
        writeLog(LOG_LOAD, "monster_load::loadMonsterResources(): Open file '%s'", monster_dir);
        loadMonsterFile(mon_file);
        fclose(mon_file);
    }
    closedir (p_dir);

    return 0;
}
