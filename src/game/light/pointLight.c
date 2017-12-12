#include "light.h"

// Library
#include "stdlib.h"

// Engine
#include "render/render.h"
#include "log/log.h"

// Game
#include "../vision/vision.h"

static Color tmp_color;
static float *tmp_seen_buffer;

// Marks tiles as seen and sets their entry in the seen buffer to their distance from source
int illuminate(Map *map, Point origin, Point p, int range, float distance) {
    Point rel_pos = (Point){
        range + p.x - origin.x,
        range + p.y - origin.y,
        0
    };

    if (rel_pos.x < 0 || rel_pos.y < 0 || rel_pos.x >= range * 2 || rel_pos.y >= range * 2) {
        return 0;
    }

    tmp_seen_buffer[rel_pos.x + rel_pos.y * range * 2] = distance;
    if (p.x >= 0 && p.y >= 0 && p.x < map->width && p.y < map->height) {
        map->tiles[p.x + p.y * map->width].seen = 1;
    }

    return 0;
}

// Renders the lighting
void postFillLight(Screen *s, Point origin, int range) {
    float l;
    int i, j, index;
    Point rel_pos;
    Color tmp;

    for (j = 0; j < range * 2; j++) {
        for (i = 0; i < range * 2; i++) {
            index = i + j * range * 2;
            if (tmp_seen_buffer[index] < 0) {
                continue;
            }
            l = 1.0 - tmp_seen_buffer[index] / (float) range;
            rel_pos = (Point){
                i + origin.x - range - s->camera_bounds.left,
                j + origin.y - range - s->camera_bounds.top,
                0
            };
            tmp = scaleColor(tmp_color, l);
            putLight(s, rel_pos.x, rel_pos.y, tmp);
        }
    }

}

// Uses the diamond walls algorithm
int pointLight(Screen *s, Map *map, Point origin, Color c, int range) {
    int i;

    tmp_color = c;

    tmp_seen_buffer = malloc(sizeof(float) * range * range * 4);
    for (i = 0; i < range * range * 4; i++) {
        tmp_seen_buffer[i] = -1;
    }

    illuminate(map, origin, origin, range, 0.0);
    for (i = 0; i < 8; i++) {
        computeDiamondWalls(
            map, i, origin, range, 1, 
            (Point){1, 1, 0}, 
            (Point){1, 0, 0}, 
            &illuminate);
    }

    postFillLight(s, origin, range);

    free(tmp_seen_buffer);

    return 0;
}

