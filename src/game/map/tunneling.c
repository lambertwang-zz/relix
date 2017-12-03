#include "map.h"

// Engine
#include "utility/random.h"

#define MAX_ROOM_SIZE 24
#define MIN_ROOM_SIZE 10
#define MAX_ROOMS 2048
#define MAX_ROOM_DENSITY 0.20

void fill_room(Map *map, Rect room) {
    int i, j;

    for (j = room.top + 1; j < room.bottom; j++) {
        for (i = room.left + 1; i < room.right; i++) {
            struct Tile *tile = &map->tiles[i + j * map->width];
            putRoom(tile);
        }
    }
}

struct Tile *vert_line(struct Map *map, int x, int y1, int y2, struct Tile *prev) {
    int i;

    int incr = (y2 - y1) & 0x80000000 ? -1 : 1;
    int start = y1 + incr;
    int end = y2 + incr;
    for (i = start; i != end; i += incr) {
        struct Tile *tile = &map->tiles[x + i * map->width];
        if (prev == tile) {
            continue;
        }
        struct Tile *left = NULL;
        if (x - 1 >= 0) {
            left = &map->tiles[x - 1 + i * map->width];
        }
        struct Tile *right = NULL;
        if (x + 1 < map->width) {
            right = &map->tiles[x + 1 + i * map->width];
        }
        
        if (prev == NULL) {
            if (tile->type != TILE_ROOM) {
                putHall(tile);
            }
            if (left->type != TILE_WALL || right->type != TILE_WALL) {
                prev = tile;
            }
        } else  {
            if (left->type == TILE_WALL && right->type == TILE_WALL) {
                if (tile->type != TILE_ROOM) {
                    putHall(tile);
                }
                if (prev->type != TILE_ROOM) {
                    putHall(prev);
                }
                prev = NULL;
            } else {
                prev = tile;
            }
        }
    }
    return prev;
}

struct Tile *horiz_line(struct Map *map, int y, int x1, int x2, struct Tile *prev) {
    int i;

    int incr = (x2 - x1) & 0x80000000 ? -1 : 1;
    int start = x1 + incr;
    int end = x2 + incr;
    for (i = start; i != end; i += incr) {
        struct Tile *tile = &map->tiles[i + y * map->width];
        if (prev == tile) {
            continue;
        }
        struct Tile *left = NULL;
        if (y - 1 >= 0) {
            left = &map->tiles[i + (y - 1) * map->width];
        }
        struct Tile *right = NULL;
        if (y + 1 < map->width) {
            right = &map->tiles[i + (y + 1) * map->width];
        }
        
        if (prev == NULL) {
            if (tile->type != TILE_ROOM) {
                putHall(tile);
            }
            if (left->type != TILE_WALL || right->type != TILE_WALL) {
                prev = tile;
            }
        } else  {
            if (left->type == TILE_WALL && right->type == TILE_WALL) {
                if (tile->type != TILE_ROOM) {
                    putHall(tile);
                }
                if (prev->type != TILE_ROOM) {
                    putHall(prev);
                }
                prev = NULL;
            } else {
                prev = tile;
            }
        }
    }
    return prev;
}

void placeDoors(struct Map *map, Rect room) {
    int i;
    for (i = room.left + 1; i < room.right; i++) {
        if (room.top >= 0) {
            struct Tile *top = &map->tiles[i + (room.top) * map->width];
            if (top->type == TILE_HALL) {
                putDoor(top);
            }
        }
        if (room.bottom < map->height) {
            struct Tile *bottom = &map->tiles[i + room.bottom * map->width];
            if (bottom->type == TILE_HALL) {
                putDoor(bottom);
            }
        }
    }
    for (i = room.top + 1; i < room.bottom; i++) {
        if (room.left >= 0) {
            struct Tile *left= &map->tiles[room.left + i * map->width];
            if (left->type == TILE_HALL) {
                putDoor(left);
            }
        }
        if (room.right< map->width) {
            struct Tile *right = &map->tiles[room.right + i * map->width];
            if (right->type == TILE_HALL) {
                putDoor(right);
            }
        }
    }
}

void mapTunneling(struct Map *map) {
    map->type = MAP_DUNGEON;
    int i, j;

    int open_space = 0;
    int room_count = 0;
    Rect rooms[MAX_ROOMS];
    for (i = 0; i < MAX_ROOMS; i++) {
        // Generate random dimensions and random location
        int w = drandom_i(MIN_ROOM_SIZE, MAX_ROOM_SIZE),
            h = drandom_i(MIN_ROOM_SIZE, MAX_ROOM_SIZE) >> 1;
        int x = drandom_i(0, map->width - w - 1),
            y = drandom_i(0, map->height - h - 1);
        Rect new_room = (Rect){y, y + h, x, x + w};

        // Check room for collisions
        int room_valid = 1;
        for (j = 0; j < room_count; j++) {
            if (rectInRect(new_room, rooms[j])) {
                room_valid = 0;
            }
        }
        if (room_valid) {
            fill_room(map, new_room);
            rooms[room_count] = new_room;
            open_space += h * w;
            if (room_count > 0) {
                int x1 = (rooms[i - 1].left + rooms[i - 1].right) / 2,
                    y1 = (rooms[i - 1].top + rooms[i - 1].bottom) / 2;
                if (room_count == 1) {
                    map->player_start = (Point){x1, y1};
                }
            }

            room_count++;


            if (open_space > MAX_ROOM_DENSITY * map->width * map->height) {
                break;
            }
        }
    }

    // Connect rooms
    for (i = 1; i < room_count; i++) {
        int x1 = (rooms[i - 1].left + rooms[i - 1].right) / 2,
            y1 = (rooms[i - 1].top + rooms[i - 1].bottom) / 2,
            x2 = (rooms[i].left + rooms[i].right) / 2,
            y2 = (rooms[i].top + rooms[i].bottom) / 2;
        if (drandom_i(0, 2)) {
            horiz_line(map, y2, x1, x2, vert_line(map, x1, y1, y2, NULL));
        } else {
            vert_line(map, x2, y1, y2, horiz_line(map, y1, x1, x2, NULL));
        }
    }
    for (i = 0; i < room_count; i++) {
        placeDoors(map, rooms[i]);
    }
}

