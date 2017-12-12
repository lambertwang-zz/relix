#include "map.h"

#include "utility/random.h"
#include "log/log.h"

#include <stdlib.h>
#include <string.h>

#define STEPS 6

// Count living neighbors
// Dead cells are room, living are wall
int ringNeighbors(struct Map *map, int x, int y, int range) {
    int i, j, count = 0;

    if (x - range >= 0) {
        for (j = y - range; j <= y + range; j++) {
            if (j >= 0 && j < map->height) {
                if (map->tiles[(x - range) + j * map->width].type != TILE_ROOM) {
                    count++;
                }
            }
        }
    }
    if (x + range < map->width) {
        for (j = y - range; j <= y + range; j++) {
            if (j >= 0 && j < map->height) {
                if (map->tiles[(x + range) + j * map->width].type != TILE_ROOM) {
                    count++;
                }
            }
        }
    }
    if (y - range >= 0) {
        for (i = x + 1 - range; i < x + range; i++) {
            if (i >= 0 && i < map->width) {
                if (map->tiles[i + (y - range) * map->width].type != TILE_ROOM) {
                    count++;
                }
            }
        }
    }
    if (y + range < map->height) {
        for (i = x + 1 - range; i < x + range; i++) {
            if (i >= 0 && i < map->width) {
                if (map->tiles[i + (y + range) * map->width].type != TILE_ROOM) {
                    count++;
                }
            }
        }
    }

    return count;
}

int neighbors(struct Map *map, int x, int y, int range) {
    int i, j, count = 0;
    
    for (j = y - range; j <= y + range; j++) {
        for (i = x - range; i <= x + range; i++) {
            if (i >= 0 && j >= 0 && i < map->width && j < map->height) {
                if (map->tiles[i + j * map->width].type != TILE_ROOM) {
                    count++;
                }
            }
        }
    }

    return map->tiles[x + y * map->width].type == TILE_ROOM ? count : count - 1;
}

int check_tile(Map *map, int x, int y) {
    if (x < 0 || y < 0 || x >= map->width || y >= map->height) {
        return 0;
    }
    if (map->tiles[x + y * map->width].type == TILE_ROOM) {
        map->tiles[x + y * map->width].type = TILE_OPEN;
        return 1 +
            check_tile(map, x + 1, y) +
            check_tile(map, x - 1, y) +
            check_tile(map, x, y + 1) +
            check_tile(map, x, y - 1);
    }
    return 0;
}

int fill_all_others(Map *map, Point center) {
    return check_tile(map, center.x, center.y);
}

void mapCellular(struct Map *map) {
    strcpy(map->type, MAP_CAVE);

    int i, j, s, t;

    int open_space = 0;
    Tile *swap = malloc(sizeof(Tile) * map->width * map->height);

    t = 0;
    int max_iter = 20;
    int init_space_req = 0.5;
    do { 
        t++;
        // Randomly initialize dead and alive cells
        for (i = 0; i < map->width * map->height; i++) {
            if (drandom_f() < 0.60) {
                map->tiles[i].type = TILE_ROOM;
            } else {
                map->tiles[i].type = TILE_WALL;
            }
        }
        memcpy(swap, map->tiles, sizeof(Tile) * map->width * map->height);

        // Simulate
        //
        for (s = 0; s < 5; s++) {
            for (j = 0; j < map->height; j++) {
                for (i = 0; i < map->width; i++) {
                    struct Tile *tile = &swap[i + j * map->width];
                    int n = neighbors(map, i, j, 1);
                    int n2 = neighbors(map, i, j, 2);
                    if (n >= 5 || n2 <= 2) {
                        tile->type = TILE_ROOM;
                    } else {
                        tile->type = TILE_WALL;
                    }
                }
            }
            memcpy(map->tiles, swap, sizeof(Tile) * map->width * map->height);
        }

        for (s = 0; s < 10; s++) {
            for (j = 0; j < map->height; j++) {
                for (i = 0; i < map->width; i++) {
                    struct Tile *tile = &swap[i + j * map->width];
                    int n = neighbors(map, i, j, 1);
                    if (n >= 5) {
                        open_space++;
                        tile->type = TILE_ROOM;
                    } else {
                        tile->type = TILE_WALL;
                    }
                }
            }
            memcpy(map->tiles, swap, sizeof(Tile) * map->width * map->height);
        }

        // Pick a random open space and (anti) flood-fill from there
        Point center;
        do {
            center = (Point){drandom_i(0, map->width - 1), drandom_i(0, map->height - 1), 0};
        } while (map->tiles[center.x + center.y * map->width].type != TILE_ROOM);
        map->player_start = center;
        open_space = fill_all_others(map, center);
    } while ((float)open_space / (float)(map->width * map->height) < init_space_req - .2 * t && t < max_iter);

    free(swap);

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

