#include "map.h"

#include "objects/objectManager.h"
#include "log/log.h"
#include "render/render.h"
#include "input/input.h"
#include "constants.h"
#include "term/screen.h"

#include <stdlib.h>

#include "../player/player.h"
#include "../relix.h"

int keyboardListener(struct Object *o, Event ev) {
    KeyboardEvent k_ev = *(KeyboardEvent *)ev.data;
    switch (k_ev.type) {
        case KEYBOARD_NORMAL:
            if (k_ev.value >= '1' && k_ev.value <= '9') {
                generate_map(o->data, k_ev.value - '1', 200, 70);
            }
            return 0;
        default:
            return 0;
    }
    
    return 0;
}

int render_map(struct Object *o) {
    struct Map *map = o->data;
    if (map == NULL || map->tiles == NULL) {
        return 0;
    }
    int i, j;
    for (i = 0; i < map->height; i++) {
        for (j = 0; j < map->width; j++) {
            struct Tile tile = map->tiles[j + i * map->width];
            putPixelA(j - screen.camera_bounds.left, i - screen.camera_bounds.top, tile.p);
        }
    }
    return 1;
}

void close_map(struct Object *o) {
    struct Map *map = o->data;
    if (map != NULL && map->tiles != NULL) {
        free(map->tiles);
    }
    if (map != NULL && map->data != NULL) {
        free(map->data);
    }
    close_default(o);
}

void initMap() {
    // Register events with the worldmanager
    registerEvent(EVENT_MAP);

    struct Object *o_map = malloc(sizeof(struct Object));

    initObject(o_map);

    listenEvent(o_map, EVENT_KEYBOARD, &keyboardListener);
    o_map->render = &render_map;
    o_map->close = &close_map;

    addObject(o_map);

    struct Map *map = malloc(sizeof(struct Map));
    
    o_map->data = map;
    map->tiles = NULL;
    map->width = -1;
    map->height = -1;

    map->data = NULL;

    addPlayer();
    generate_map(map, TUNNELING_ALG, 200, 70);
}

void generate_map(struct Map *map, 
                int alg, 
                unsigned int width, 
                unsigned int height) {
    if (map->tiles != NULL) {
        free(map->tiles);
    }
    map->tiles = malloc(sizeof(struct Tile) * width * height);
    map->width = width;
    map->height = height;
    map->player_start = (Point){0, 0};

    int i, j;
    for (i = 0; i < map->height; i++) {
        for (j = 0; j < map->width; j++) {
            map->tiles[j + i * map->width].solid = SOLID;
            map->tiles[j + i * map->width].type = TILE_WALL;
            map->tiles[j + i * map->width].p = PIXEL_NULL;
        }
    }

    switch (alg) {
        case TUNNELING_ALG:
            map_tunneling(map);
            break;
        case CELLULAR_ALG:
            map_cellular(map);
            break;
        case RANDOMWALK_ALG:
            map_randomwalk(map);
        default:
            break;
    }
    writeLog(10, "starting player at %d %d", map->player_start.x, map->player_start.y);
    // player->pos.x = map->player_start.x;
    // player->pos.y = map->player_start.y;
}

