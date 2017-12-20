#include "map.h"

// Library
#include <stdlib.h>
#include <string.h>

void initTile(Tile *tile) {
    tile->seen = 0;

    Pixel *pix = &tile->p;
    pix->depth = 0;
}

void putWall(struct Tile *tile) {
    tile->solid = SOLID;
    tile->type = TILE_WALL;

    tile->p.bg = WALL_COLOR;
    tile->p.fg = COLOR_BLANK;
    initTile(tile);
}

void putOpen(struct Tile *tile) {
    tile->solid = SOFT;
    tile->type = TILE_OPEN;

    tile->p.bg = OPEN_COLOR;
    tile->p.fg = COLOR_BLANK;
    initTile(tile);
}

void putRoom(struct Tile *tile) {
    tile->solid = SOFT;
    tile->type = TILE_ROOM;

    tile->p.bg = ROOM_COLOR;
    tile->p.fg = COLOR_BLANK;
    initTile(tile);
}

void putHall(struct Tile *tile) {
    tile->solid = SOFT;
    tile->type = TILE_HALL;

    tile->p.bg = HALL_COLOR;
    tile->p.fg = COLOR_BLANK;
    initTile(tile);
}

void putDoor(struct Tile *tile) {
    tile->solid = SOLID;
    tile->type = TILE_DOOR;

    tile->p.bg = DOOR_COLOR;
    tile->p.fg = COLOR_BLACK;
    initTile(tile);

    strncpy(tile->p.chr, "▓", UNICODE_MAX);
}

