#include "map.h"

#include "utility/random.h"

#include <stdlib.h>

#define MAX_ROOM_SIZE 15
#define MIN_ROOM_SIZE 8
#define MAX_ROOMS 2048
#define MAX_ROOM_DENSITY 0.06

void fill_room(struct Map *map, Rect room) {
    int i, j;

    for (j = room.top; j < room.bottom; j++) {
        for (i = room.left; i < room.right; i++) {
            struct Tile *tile = &map->tiles[i + j * map->width];
            tile->solid = SOLID;
        }
    }
}

void fill_line(struct Map *map, Line line) {
    int i, j;

    // Assume only vertical and horizontal lines
    for (j = line.y1 < line.y2 ? line.y1 : line.y2; j <= (line.y1 < line.y2 ? line.y2 : line.y1); j++) {
        for (i = line.x1 < line.x2 ? line.x1 : line.x2; i <= (line.x1 < line.x2 ? line.x2 : line.x1); i++) {
            struct Tile *tile = &map->tiles[i + j * map->width];
            tile->solid = SOLID;
        }
    }
}

void map_tunneling(struct Map *map) {
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
                int x1 = (rooms[room_count - 1].left + rooms[room_count - 1].right) / 2,
                    y1 = (rooms[room_count - 1].top + rooms[room_count - 1].bottom) / 2,
                    x2 = (rooms[room_count].left + rooms[room_count].right) / 2,
                    y2 = (rooms[room_count].top + rooms[room_count].bottom) / 2;
                if (drandom_i(0, 1)) {
                    fill_line(map, (Line){x1, y1, x1, y2});
                    fill_line(map, (Line){x2, y2, x1, y2});
                } else {
                    fill_line(map, (Line){x1, y1, x2, y1});
                    fill_line(map, (Line){x2, y2, x2, y1});
                }
            }

            room_count++;


            if (open_space > MAX_ROOM_DENSITY * map->width * map->height) {
                break;
            }
        }
    }
    for (i = 0; i < map->height; i++) {
        for (j = 0; j < map->width; j++) {
            struct Tile *tile = &map->tiles[j + i * map->width];
            if (tile->solid) {
                Pixel *pix = &map->tiles[j + i * map->width].p;
                pix->c_bg = (Color){128, 128, 128, 1.0};
                pix->bg = rgbToTerm(pix->c_bg);
                pix->depth = 0;
                pix->chr = ' ';
            }
        }
    }
}
/*
'''
This version of the tunneling algorithm is essentially
identical to the tunneling algorithm in the Complete Roguelike
Tutorial using Python, which can be found at
http://www.roguebasin.com/index.php?title=Complete_Roguelike_Tutorial,_using_python%2Blibtcod,_part_1

Requires random.randint() and the Rect class defined below.
'''
# TODO: raise an error if any necessary classes are missing
def generateLevel(self, mapWidth, mapHeight):
num_rooms = 0
for r in range(self.MAX_ROOMS):
# random width and height
w = random.randint(self.ROOM_MIN_SIZE,self.ROOM_MAX_SIZE)
h = random.randint(self.ROOM_MIN_SIZE,self.ROOM_MAX_SIZE)
# random position within map boundr
x = random.randint(0, MAP_WIDTH - w -1)
y = random.randint(0, MAP_HEIGHT - h -1)

new_room = Rect(x, y, w, h)
# check for overlap with previous rooms
failed = False
for other_room in rooms:
if new_room.intersect(other_room):
failed = True
break
if not failed:
self.createRoom(new_room)
(new_x, new_y) = new_room.center()

if num_rooms != 0:
# all rooms after the first one
# connect to the previous room
#center coordinates of the previous room
(prev_x, prev_y) = rooms[num_rooms-1].center()
# 50% chance that a tunnel will start horizontally
if random.randint(0,1) == 1:
self.createHorTunnel(prev_x, new_x, prev_y)
self.createVirTunnel(prev_y, new_y, new_x)
else: # else it starts virtically
self.createVirTunnel(prev_y, new_y, prev_x)
self.createHorTunnel(prev_x, new_x, new_y)
# append the new room to the list
rooms.append(new_room)
num_rooms += 1

return self.level
def createRoom(self, room):
# set all tiles within a rectangle to 0
for x in range(room.x1 + 1, room.x2):
for y in range(room.y1+1, room.y2):
self.level[x][y] = 0
def createHorTunnel(self, x1, x2, y):
for x in range(min(x1,x2),max(x1,x2)+1):
self.level[x][y] = 0
def createVirTunnel(self, y1, y2, x):
for y in range(min(y1,y2),max(y1,y2)+1):
self.level[x][y] = 0
*/
