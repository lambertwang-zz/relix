#ifndef __MAP_H__
#define __MAP_H__

#include "term/color.h"
#include "objects/object.h"

// Map generation algorithm types
#define TUNNELING_ALG 1

// Tile attributes
#define SOFT 0
#define SOLID 1

struct Map {
    struct Tile *tiles;
    int width;
    int height;

    // Algorithm specific data for the map
    void *data;
};

struct Tile {
    Pixel p;
    int solid;
};

int render_map(const struct Object *o);
void close_map(struct Object *o);

void initMap();
void generate_map(struct Map *map, 
                int alg, 
                unsigned int width, 
                unsigned int height);


void map_tunneling(struct Map *map);

#endif

