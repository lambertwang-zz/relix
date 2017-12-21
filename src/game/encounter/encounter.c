#include "encounter.h"

// Engine
#include "log/log.h"
#include "object/objectManager.h"

// Game
#include "../monster/monster.h"
#include "../asset/asset.h"
#include "../relix.h"

void addEncounter(Map *map, Rect loc, int challenge) {
    int open_space = (loc.right - loc.left) * (loc.bottom - loc.top);

    int i, j;
    for (j = loc.top; j < loc.bottom; j++) {
        for (i = loc.left; i < loc.right; i++) {
            open_space -= isSolid(map, (Point){i, j, 0});
        }
    }

    ResourceManager *monster_res = getMonsterRes();

    if (monster_res->store.count > 0) {
        MonsterFamily *family = ((ResourceEntry *)getDataArray(&monster_res->store, 0))->data;
        if (family->entries.count > 0) {
            MonsterEntry *entry = getDataArray(&family->entries, 0);
            Object *monster = createMonster(entry);
            monster->pos = (Point){loc.left + 1, loc.top + 1, 0};
            addObject(monster);
        }
    }
}

