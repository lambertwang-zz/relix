#include "resource.h"

// Library
#include <dirent.h>
#include <string.h>

// Engine
#include "constants.h"
#include "log/log.h"

// Game
#include "../relix.h"
#include "parser.h"

int loadMonster(FILE *file) {

    return 0;
}

int loadMonsterFile(FILE *file) {
    RNode *node = parseFile(file);
    int i = 0;
    for (i = 0; i < node->values.count; i++) {
        writeLog(LOG_LOAD, "monster_load::loadMonsterFile(): Label: '%s'", ((NodeVal *)getDataArray(&node->values, i))->label);
    }
    closeRNode(node);
    return 0;
}

int loadMonsterResources() {
    char monster_dir[LABEL_LONG];
    strncpy(monster_dir, ASSET_DIR, LABEL_LONG);
    strncat(monster_dir, MONSTER_DIR, LABEL_LONG - strlen(monster_dir));
    int dir_len = strlen(monster_dir);

    struct dirent *p_dirent;
    DIR *p_dir;

    p_dir = opendir(monster_dir);
    if (p_dir == NULL) {
        writeLog(LOG_LOAD, "monster_load::loadMonsterResources(): ERROR: Cannot open directory '%s'", monster_dir);
        return 1;
    }

    char *file_ext;

    while ((p_dirent = readdir(p_dir)) != NULL) {
        file_ext = strrchr(p_dirent->d_name, '.');
        if (file_ext == NULL || strcmp(file_ext + 1, "txt")) {
            continue;
        }

        strncpy(monster_dir + dir_len, p_dirent->d_name, LABEL_LONG - dir_len);
        FILE *mon_file = fopen(monster_dir, "r");
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
