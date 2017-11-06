#ifndef __MAP_H__
#define __MAP_H__

#include "term/color.h"

// Map generation algorithm types
#define TUNNELING_ALG 1

// Tile attributes
#define SOFT 0
#define SOLID 1

struct Map {
    struct Tile *tiles;
    int width;
    int height;
};

struct Tile {
    Pixel p;
    int solidity;
};

void initMap();
void cloaseMap();

void generate_map(struct Map *p_map, 
                int alg, 
                unsigned int width, 
                unsigned int height);

int render_map(struct Map *p_map);

void map_tunneling(struct Map *p_map,
                unsigned int width,
                unsigned int height);

#endif

