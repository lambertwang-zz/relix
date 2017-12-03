#ifndef __MAP_H__
#define __MAP_H__

// Engine
#include "render/render.h"
#include "object/object.h"

// Map generation algorithm types
#define TUNNELING_ALG 0
#define CELLULAR_ALG 1
#define RANDOMWALK_ALG 2

// Map environment types
#define MAP_DUNGEON 1
#define MAP_CAVE 4

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
#define OPEN_COLOR (Color){128, 128, 128, 1.0};
// #define ROOM_COLOR (Color){128, 128, 128, 1.0};
#define ROOM_COLOR (Color){135, 135, 175, 1.0};
// #define HALL_COLOR (Color){192, 192, 192, 1.0};
// #define HALL_COLOR (Color){64, 64, 64, 1.0};
// #define HALL_COLOR (Color){135, 135, 175, 1.0};
#define HALL_COLOR (Color){128, 128, 128, 1.0};
#define DOOR_COLOR (Color){95, 95, 0, 1.0};

#define SEEN_COLOR (Color){24, 24, 24, 1.0}

typedef struct Tile {
    struct Pixel p;
    int solid;
    int type;
    int seen;
} Tile;

typedef struct Map {
    int id;

    int type;

    struct Tile *tiles;
    int width;
    int height;

    // Algorithm specific data for the map
    void *data;

    struct Point player_start;
} Map;

typedef struct MapEvent {
    struct Map *map;
} MapEvent;

int renderMap(struct Map *map, struct Screen *s);
void clearMap(Map *o);
void closeMap(Object *o);

void initMapObj();
void initMap(Map *map);
void generateMap(Map *map, 
                int alg, 
                unsigned int width, 
                unsigned int height);

// Tile generation functions
void putWall(struct Tile *tile);
void putOpen(struct Tile *tile);
void putRoom(struct Tile *tile);
void putHall(struct Tile *tile);
void putDoor(struct Tile *tile);

// Map generation algorithms
void mapTunneling(Map *map);
void mapCellular(Map *map);
void mapRandomwalk(Map *map);

// Map Utility Functions
int isSolid(Map *map, Point p);
int isSolidOct(Map *map, Point p, int octant, Point origin);

#endif

