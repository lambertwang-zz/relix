#include "light.h"

// Engine
#include "render/render.h"

// Game
#include "../vision/vision.h"

Screen *tmp_screen;
Color tmp_color;

int illuminate(Map *map, Point p, int range, float distance) {
    float l = 1.0 - distance / (float) range;
    Point rel_pos = (Point){
            p.x - tmp_screen->camera_bounds.left,
            p.y - tmp_screen->camera_bounds.top
    };

    Color tmp = scaleColor(tmp_color, l);
    putLight(tmp_screen, rel_pos.x, rel_pos.y, tmp);
    map->tiles[p.x + p.y * map->width].seen = 1;

    return 0;
}
// Uses the diamond walls algorithm
int pointLight(Screen *s, Map *map, Point origin, Color c, int range) {
    tmp_screen = s;
    tmp_color = c;

    illuminate(map, origin, 1, 0);
    int i;
    for (i = 0; i < 8; i++) {
        computeDiamondWalls(map, i, origin, range, 1, (Point){1, 1}, (Point){1, 0}, &illuminate);
    }
    return 0;
}

