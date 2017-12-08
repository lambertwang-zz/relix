#include "map.h"

int isSolid(Map *map, Point p) {
    if (p.x < 0 || p.y < 0 || p.x >= map->width || p.y >= map->height) {
        return 1;
    }

    return map->tiles[p.x + p.y * map->width].solid;
}

int isSolidOct(Map *map, Point p, int octant, Point origin) {
    int nx = origin.x; int ny = origin.y;
    switch (octant) {
        case 0: nx += p.x; ny -= p.y; break;
        case 1: nx += p.y; ny -= p.x; break;
        case 2: nx -= p.y; ny -= p.x; break;
        case 3: nx -= p.x; ny -= p.y; break;
        case 4: nx -= p.x; ny += p.y; break;
        case 5: nx -= p.y; ny += p.x; break;
        case 6: nx += p.y; ny += p.x; break;
        case 7: default: nx += p.x; ny += p.y; break;
    }

    return isSolid(map, (Point){nx, ny, 0});
}

