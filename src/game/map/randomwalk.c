#include "map.h"

// Library
#include <stdlib.h>
#include <string.h>

// Engine
#include "utility/random.h"
#include "log/log.h"

int walk(struct Map *map, Point *next) {
    int ret = 0;
    if (map->tiles[next->x + next->y * map->width].type == TILE_WALL) {
        ret = 1;
    }
    map->tiles[next->x + next->y * map->width].type = TILE_OPEN;

    Point add;
    while (1) {
        add = (Point){next->x, next->y};
        int north = 10 + ((next->y > map->height * .75) ? 4: 0);
        int south = 10 + ((next->y < map->height * .25) ? 4: 0);
        int east = 19 + ((next->x < map->width * .25) ? 7: 0);
        int west = 19 + ((next->x > map->width * .75) ? 7: 0);
        // int west = 1900;
        int dir = drandom_i(0, north + south + east + west);
        if (dir < north) {
            add.y = add.y - 1;
        } else if (dir < north + south) {
            add.y = add.y + 1;
        } else if (dir < north + south + east) {
            add.x = add.x + 1;
        } else {
            add.x = add.x - 1;
        }
        if (add.x >= 0 && add.y >= 0 && add.x < map->width && add.y < map->height) {
            *next = add;
            break;
        }
    }
    return ret;
}

void mapRandomwalk(struct Map *map) {
    strcpy(map->type, MAP_CAVE);
    int i;

    Point next = (Point){map->width / 2, map->height / 2};
    int open_space = 0;
    int iter = 0;
    int max_iter = map->width * map->height * 4;

    while (open_space < map->width * map->height * 0.2 && iter < max_iter) {
        iter++;
        open_space += walk(map, &next);
    }
    map->player_start = next;

    // Color map appropriately
    for (i = 0; i < map->width * map->height; i++) {
        struct Tile *tile = &map->tiles[i];

        switch (tile->type) {
            case TILE_OPEN:
                putRoom(tile);
                break;
            case TILE_ROOM:
            case TILE_WALL:
                putWall(tile);
                break;
        }
    }
}

