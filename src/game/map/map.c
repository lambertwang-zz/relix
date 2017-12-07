// Library
#include <stdlib.h>

// Engine
#include "object/objectManager.h"
#include "log/log.h"
#include "input/input.h"
#include "constants.h"
#include "term/screen.h"

// Game
#include "map.h"
#include "../relix.h"

static MapResourceManager map_resources;

int initMapResources() {
    initArray(&map_resources.map_types);
    return 0;
}

int keyboardListener(struct Object *o, Event ev) {
    KeyboardEvent k_ev = *(KeyboardEvent *)ev.data;
    switch (k_ev.type) {
        case KEYBOARD_NORMAL:
            if (k_ev.value >= '1' && k_ev.value <= '9') {
                generateMap(o->data, k_ev.value - '1', 200, 70);
            }
            return 0;
        default:
            return 0;
    }
    
    return 0;
}

int renderMap(Map *map, Screen *s) {
    if (map == NULL || map->tiles == NULL) {
        return 0;
    }
    int i, j;
    for (i = 0; i < map->height; i++) {
        for (j = 0; j < map->width; j++) {
            struct Tile tile = map->tiles[j + i * map->width];
            Point rel_pos =  {
                j - s->camera_bounds.left,
                i - s->camera_bounds.top
            };
            if (tile.seen) {
                if (tile.type == TILE_WALL) {
                    putPixelL(s, rel_pos.x, rel_pos.y, tile.p);
                    continue;
                }
                if(putPixelA(s, rel_pos.x, rel_pos.y, tile.p) == 3) { // TODO: Define error codes
                    // Pixel not rendered due to no lighting
                    putPixelRgb(s, rel_pos.x, rel_pos.y, SEEN_COLOR);
                }
            }

        }
    }
    return 1;
}

void clearMap(Map *map) {
    if (map != NULL && map->tiles != NULL) {
        free(map->tiles);
    }
    if (map != NULL && map->data != NULL) {
        free(map->data);
    }
}

void closeMap(struct Object *o) {
    struct Map *map = o->data;
    clearMap(map);
    closeDefault(o);
}

void initMapObj() {
    struct Object *o_map = createObject();

    listenEvent(o_map, EVENT_KEYBOARD, &keyboardListener);
    o_map->close = &closeMap;

    addObject(o_map);

    struct Map *map = malloc(sizeof(struct Map));
    o_map->data = map;
    initMap(map);
}
    
void initMap(Map *map) {
    static int map_id_iterator = 0;
    map->id = map_id_iterator++;
    map->type[0] = '\0';
    map->tiles = NULL;
    map->width = -1;
    map->height = -1;

    map->data = NULL;

    generateMap(map, TUNNELING_ALG, 200, 70);
}

void generateMap(struct Map *map, 
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

    int i;
    for (i = 0; i < map->height * map->width; i++) {
        putWall(&map->tiles[i]);
    }

    switch (alg) {
        case TUNNELING_ALG:
            mapTunneling(map);
            break;
        case CELLULAR_ALG:
            mapCellular(map);
            break;
        case RANDOMWALK_ALG:
            mapRandomwalk(map);
        default:
            break;
    }

    writeLog(LOG_MAP, "map::generateMap(): Starting player at X = %d, Y = %d", map->player_start.x, map->player_start.y);

    Event ev;
    ev.id = EVENT_MAP;
    ev.data = malloc(sizeof(MapEvent));
    ((MapEvent *) ev.data)->map= map;
    sendEvent(ev);
}

