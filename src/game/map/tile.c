#include "map.h"

void initTile(Tile *tile) {
    tile->seen = 0;

    Pixel *pix = &tile->p;
    pix->bg = rgbToTerm(pix->c_bg);
    pix->fg = rgbToTerm(pix->c_fg);
    pix->depth = 0;
    pix->chr = ' ';
}

void putWall(struct Tile *tile) {
    tile->solid = SOLID;
    tile->type = TILE_WALL;

    tile->p.c_bg = WALL_COLOR;
    tile->p.c_fg = COLOR_BLANK;
    initTile(tile);
}

void putOpen(struct Tile *tile) {
    tile->solid = SOFT;
    tile->type = TILE_OPEN;

    tile->p.c_bg = OPEN_COLOR;
    tile->p.c_fg = COLOR_BLANK;
    initTile(tile);
}

void putRoom(struct Tile *tile) {
    tile->solid = SOFT;
    tile->type = TILE_ROOM;

    tile->p.c_bg = ROOM_COLOR;
    tile->p.c_fg = COLOR_BLANK;
    initTile(tile);
}

void putHall(struct Tile *tile) {
    tile->solid = SOFT;
    tile->type = TILE_HALL;

    tile->p.c_bg = HALL_COLOR;
    tile->p.c_fg = COLOR_BLANK;
    initTile(tile);
}

void putDoor(struct Tile *tile) {
    tile->solid = SOLID;
    tile->type = TILE_DOOR;

    tile->p.c_bg = DOOR_COLOR;
    tile->p.c_fg = COLOR_BLACK;
    initTile(tile);

    tile->p.chr = '|';
}

