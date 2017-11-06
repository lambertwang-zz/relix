#include "map.h"

#include <stdlib.h>

void map_tunneling(struct Map *p_map,
                unsigned int width,
                unsigned int height) {
    p_map = malloc(sizeof(struct Map));
    p_map->tiles = malloc(sizeof(struct Tile) * width * height);
    p_map->width = width;
    p_map->height = height;
}
/*
'''
This version of the tunneling algorithm is essentially
identical to the tunneling algorithm in the Complete Roguelike
Tutorial using Python, which can be found at
http://www.roguebasin.com/index.php?title=Complete_Roguelike_Tutorial,_using_python%2Blibtcod,_part_1

Requires random.randint() and the Rect class defined below.
'''
def __init__(self):
self.level = []
self.ROOM_MAX_SIZE = 15
self.ROOM_MIN_SIZE = 6
self.MAX_ROOMS = 30
# TODO: raise an error if any necessary classes are missing
def generateLevel(self, mapWidth, mapHeight):
# Creates an empty 2D array or clears existing array
self.level = [[1
for y in range(mapHeight)]
for x in range(mapWidth)]
rooms = []
num_rooms = 0
for r in range(self.MAX_ROOMS):
# random width and height
w = random.randint(self.ROOM_MIN_SIZE,self.ROOM_MAX_SIZE)
h = random.randint(self.ROOM_MIN_SIZE,self.ROOM_MAX_SIZE)
# random position within map boundries
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
