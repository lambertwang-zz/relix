#include "map.h"

#include "objects/objectManager.h"
#include "log/log.h"
#include "render/render.h"

#include <stdlib.h>

int render_map(const struct Object *o) {
    struct Map *map = o->data;
    if (map == NULL || map->tiles == NULL) {
        return 0;
    }
    int i, j;
    for (i = 0; i < map->height; i++) {
        for (j = 0; j < map->width; j++) {
            putPixel(j, i, map->tiles[j + i * map->width].p);
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
    struct Object *o_map = malloc(sizeof(struct Object));

    initObject(o_map);

    // listenEvent(o_1, EVENT_KEYBOARD, &keyboardListener);
    o_map->render = &render_map;
    o_map->close = &close_map;

    addObject(o_map);

    struct Map *map = malloc(sizeof(struct Map));
    
    o_map->data = map;
    map->tiles = NULL;
    map->width = -1;
    map->height = -1;

    map->data = NULL;

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

    int i, j;
    for (i = 0; i < map->height; i++) {
        for (j = 0; j < map->width; j++) {
            map->tiles[j + i * map->width].solid = SOFT;
        }
    }

    switch (alg) {
        case TUNNELING_ALG:
            map_tunneling(map);
            break;
        default:
            break;
    }
}

