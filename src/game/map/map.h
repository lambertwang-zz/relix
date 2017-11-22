#ifndef __MAP_H__
#define __MAP_H__

#include "term/color.h"
#include "objects/object.h"

// Map generation algorithm types
#define TUNNELING_ALG 0
#define CELLULAR_ALG 1
#define RANDOMWALK_ALG 2

// Tile attributes
#define SOFT 0
#define SOLID 1

// Tile type
#define TILE_WALL 0
#define TILE_OPEN 1
#define TILE_ROOM 2
#define TILE_HALL 3
#define TILE_DOOR 8

// Tile colors
#define WALL_COLOR (Color){0, 0, 0, 1.0};
#define ROOM_COLOR (Color){128, 128, 128, 1.0};
// #define HALL_COLOR (Color){192, 192, 192, 1.0};
#define HALL_COLOR (Color){64, 64, 64, 1.0};
#define DOOR_COLOR (Color){128, 128, 64, 1.0};

struct Map {
    struct Tile *tiles;
    int width;
    int height;

    // Algorithm specific data for the map
    void *data;

    Point player_start;
};

struct Tile {
    Pixel p;
    int solid;
    int type;
};

typedef struct MapEvent {
    struct Map *map;
} MapEvent;

void close_map(struct Object *o);

void initMap();
void generate_map(struct Map *map, 
                int alg, 
                unsigned int width, 
                unsigned int height);


void map_tunneling(struct Map *map);
void map_cellular(struct Map *map);
void map_randomwalk(struct Map *map);

#endif

