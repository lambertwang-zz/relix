#include "render/render.h"

#include "../light/light.h"
#include "../world/world.h"

#include "player.h"

int renderPlayerLight(Object *o, Screen *s) {
    pointLight(s, world.current_map, o->pos, 
        // (Color){255, 255, 135},
        (Color){192, 192, 90},
        10);
    return 1;
}

